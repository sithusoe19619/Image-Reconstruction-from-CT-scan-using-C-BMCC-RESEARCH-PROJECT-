#include <iostream>
#include <fstream>
#include <cmath>
#include "Display.h"

using namespace std;

const int N = 180;             // Image Size N x N
const int ANGLES = 180;    // Number of projection angles (0 to 179 degrees)
const double PI = 3.14159265358979323846;

// Prototypes
void loadImage(int[][N], string);
void createSinogram(int phantom[][N], int sinogram[][N]);
double bilinearInterpolation(int image[][N], double x, double y);
void back_projection(int sinogram[][N], int recon[][N]);

int main() {		
    int Pixels[N][N];
    int Sinogram[N][N];
    int Recon[N][N];   // reconstructed image (used for both phantoms)

    // ---------- Phantom 1 (Shapes) ----------
    cout << "\nProcessing Phantom 1 (Shapes)..." << endl;
    loadImage(Pixels, "shapesPhantom.txt");
    Display image01(Pixels, "Phantom_01");

    // Sinogram 1
    createSinogram(Pixels, Sinogram);
    Display sino01(Sinogram, "Sinogram_01");
    cout << "Sinogram 1 created successfully!" << endl << endl ;

    // Reconstruction 1 (raw back-projection)
    back_projection(Sinogram, Recon);
    Display recon01(Recon, "Reconstruction_01");
    cout << "Reconstruction 1 created successfully!" << endl << endl;

    // ---------- Phantom 2 (Head) ----------
    cout << "\nProcessing Phantom 2 (Head)..." << endl;
    loadImage(Pixels, "headPhantom.txt");
    Display image02(Pixels, "Phantom_02");

    // Sinogram 2
    createSinogram(Pixels, Sinogram);
    Display sino02(Sinogram, "Sinogram_02");
    cout << "Sinogram 2 created successfully!" << endl << endl ;

    // Reconstruction 2 (raw back-projection)
    back_projection(Sinogram, Recon);
    Display recon02(Recon, "Reconstruction_02");
    cout << "Reconstruction 2 created successfully!" << endl ;

    cout << "\nAll files generated successfully!" << endl;
    cout << "\nOpen the HTML files in your browser to view the results." << endl << endl;

    return 0;
}

// ---------------------------------------------------
// Load phantom image from file
// ---------------------------------------------------
void loadImage(int Gray[][N], string fileName) {
    ifstream file;
    file.open(fileName);
    if (!file) {
        cout << "File does not exist: " << fileName << endl;
        return;
    }
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            file >> Gray[row][col];
    file.close();
}

// ---------------------------------------------------
// Bilinear interpolation for sub-pixel sampling
// ---------------------------------------------------
double bilinearInterpolation(int image[][N], double x, double y) {
    // Boundary checks
    if (x < 0 || x >= N - 1 || y < 0 || y >= N - 1)
        return 0.0;
    
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    double dx = x - x0;
    double dy = y - y0;
    
    // Bilinear interpolation formula
    double value = (1 - dx) * (1 - dy) * image[y0][x0] +
                   dx       * (1 - dy) * image[y0][x1] +
                   (1 - dx) * dy       * image[y1][x0] +
                   dx       * dy       * image[y1][x1];
    
    return value;
}

// ---------------------------------------------------
// Create sinogram using Radon transform
// ---------------------------------------------------
void createSinogram(int phantom[][N], int sinogram[][N]) {
    double center = N / 2.0; // Center of the image
    
    // Initialize sinogram to zero
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            sinogram[i][j] = 0;
    
    // For each projection angle
    for (int angleIdx = 0; angleIdx < ANGLES; angleIdx++) {
        double theta = angleIdx * PI / ANGLES; // radians
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        
        // For each detector position (projection ray)
        for (int t = 0; t < N; t++) {
            double tPos = t - center; // Position relative to center
            double sum = 0.0;
            int count = 0;
            
            // Trace along the ray perpendicular to the detector
            // x = tPos * cosTheta - s * sinTheta + center
            // y = tPos * sinTheta + s * cosTheta + center
            for (double s = -center; s <= center; s += 0.5) {
                double x = tPos * cosTheta - s * sinTheta + center;
                double y = tPos * sinTheta + s * cosTheta + center;
                
                if (x >= 0 && x < N - 1 && y >= 0 && y < N - 1) {
                    sum += bilinearInterpolation(phantom, x, y);
                    count++;
                }
            }
            
            if (count > 0) {
                sinogram[angleIdx][t] = (int)(sum / count);
            }
        }
        
        // Progress indicator
        if ((angleIdx + 1) % 30 == 0) {
            cout << "  Sinogram progress: " << (angleIdx + 1)
                 << "/" << ANGLES << " angles" << endl;
        }
    }
    
    // Normalize sinogram for better visualization
    int maxVal = 0;
    for (int i = 0; i < ANGLES; i++) {
        for (int j = 0; j < N; j++) {
            if (sinogram[i][j] > maxVal)
                maxVal = sinogram[i][j];
        }
    }
    
    // Scale to 0-255 range for display
    if (maxVal > 0) {
        for (int i = 0; i < ANGLES; i++) {
            for (int j = 0; j < N; j++) {
                sinogram[i][j] = (sinogram[i][j] * 255) / maxVal;
            }
        }
    }
}

// ---------------------------------------------------
// Raw back-projection from sinogram to reconstructed image
// ---------------------------------------------------
void back_projection(int sinogram[][N], int recon[][N]) {
    double center = N / 2.0;

    // Accumulate in double for precision
    double accumulator[N][N];

    // Initialize accumulator
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            accumulator[row][col] = 0.0;

    // Pixel-driven back-projection
    for (int angleIdx = 0; angleIdx < ANGLES; angleIdx++) {
        double theta = angleIdx * PI / ANGLES;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {

                // Coordinates centered at 0
                double x0 = col - center; //Position relative to center
                double y0 = row - center; //Position relative to center

                // Detector coordinate t
                double tPos = x0 * cosTheta + y0 * sinTheta;
                double t = tPos + center;   // detector index

                if (t >= 0 && t <= N - 1) {
                    // 1D linear interpolation along detector axis
                    int lowIdx = (int)floor(t);
                    int highIdx = lowIdx + 1;
                    double weight = t - lowIdx;

                    double val = 0.0;
                    if (lowIdx >= 0 && highIdx < N) {
                        double lowValue = sinogram[angleIdx][lowIdx];
                        double highValue = sinogram[angleIdx][highIdx];
                        val = (1.0 - weight) * lowValue + weight * highValue;
                    } else if (lowIdx >= 0 && lowIdx < N) {
                        val = sinogram[angleIdx][lowIdx];
                    }

                    accumulator[row][col] += val;
                }
            }
        }

        if ((angleIdx + 1) % 30 == 0) {
            cout << "  Back-projection progress: " << (angleIdx + 1)
                 << "/" << ANGLES << " angles" << endl;
        }
    }

    // Average over number of angles
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            accumulator[row][col] /= (double)ANGLES;

    // Normalize 0–255 for Display
    double maxVal = 0.0;
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            if (accumulator[row][col] > maxVal)
                maxVal = accumulator[row][col];

    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            if (maxVal > 0.0)
                recon[row][col] = (int)((accumulator[row][col] / maxVal) * 255.0 + 0.5);
            else
                recon[row][col] = 0;
        }
    }
}
#include <iostream>
#include <fstream>
#include <cmath>
#include "Display.h"      // SIZE should be 400 in this header

using namespace std;

const int N      = 400;   // phantom & sinogram size: 400 x 400
const int ANGLES = 180;   // number of projection angles
const double PI  = 3.14159265358979323846;

// Prototypes
void loadPhantom(int image[][N], const string& fileName);
double bilinearInterpolation(int image[][N], double x, double y);
void createSinogram(int phantom[][N], int sinogram[][N]);
void copyToShort(int src[][N], short dest[][SIZE]);

int main() {
    int   Phantom[N][N];     // 400 x 400 reconstructed phantom (int)
    int   Sinogram[N][N];    // 400 x 400 sinogram result (int)
    short SinoDisp[SIZE][SIZE];

    cout << "Loading reconstructed_phantom_400x400.txt ..." << endl;
    loadPhantom(Phantom, "reconstructed_phantom_400x400.txt");

    cout << "Creating sinogram from phantom (forward Radon transform) ..." << endl;
    createSinogram(Phantom, Sinogram);

    // Copy sinogram into short buffer for Display
    copyToShort(Sinogram, SinoDisp);

    // Display: this will generate P_to_S_sinogram_400x400.html
    Display sinoImg(SinoDisp, "P_to_S_sinogram_400x400");

    cout << "Done." << endl;
    cout << "Open P_to_S_sinogram_400x400.html in your browser." << endl;
    return 0;
}

// ---------------------------------------------------
// Load phantom image from file (400 x 400 integers)
// ---------------------------------------------------
void loadPhantom(int image[][N], const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cout << "Error opening phantom file: " << fileName << endl;
        exit(1);
    }

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            inFile >> image[row][col];
        }
    }

    inFile.close();
}

// ---------------------------------------------------
// Bilinear interpolation for sub-pixel sampling
// (same structure as your original 180x180 version)
// ---------------------------------------------------
double bilinearInterpolation(int image[][N], double x, double y) {
    // Boundary checks
    if (x < 0 || x >= N - 1 || y < 0 || y >= N - 1)
        return 0.0;

    int x0 = static_cast<int>(x);
    int y0 = static_cast<int>(y);
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    double dx = x - x0;
    double dy = y - y0;

    double value =
        (1.0 - dx) * (1.0 - dy) * image[y0][x0] +
        dx         * (1.0 - dy) * image[y0][x1] +
        (1.0 - dx) * dy         * image[y1][x0] +
        dx         * dy         * image[y1][x1];

    return value;
}

// ---------------------------------------------------
// Create sinogram using the same Radon geometry
// as your original 180x180 createSinogram
// ---------------------------------------------------
void createSinogram(int phantom[][N], int sinogram[][N]) {
    double center = N / 2.0; // Center of the image

    // Initialize sinogram to zero
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            sinogram[i][j] = 0;

    // For each projection angle
    for (int angleIdx = 0; angleIdx < ANGLES; ++angleIdx) {
        double theta    = angleIdx * PI / ANGLES; // radians
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        // For each detector position (projection ray)
        for (int t = 0; t < N; ++t) {
            double tPos = t - center; // position relative to center
            double sum  = 0.0;
            int    count = 0;

            // Trace along the ray perpendicular to the detector
            // x =  tPos * cosTheta - s * sinTheta + center
            // y =  tPos * sinTheta + s * cosTheta + center
            for (double s = -center; s <= center; s += 0.5) {
                double x = tPos * cosTheta - s * sinTheta + center;
                double y = tPos * sinTheta + s * cosTheta + center;

                if (x >= 0 && x < N - 1 && y >= 0 && y < N - 1) {
                    sum += bilinearInterpolation(phantom, x, y);
                    ++count;
                }
            }

            if (count > 0) {
                sinogram[angleIdx][t] = static_cast<int>(sum / count);
            }
        }

        // Progress indicator
        if ((angleIdx + 1) % 40 == 0) {
            cout << "  Sinogram progress: "
                 << (angleIdx + 1) << "/" << ANGLES << " angles" << endl;
        }
    }

    // Normalize sinogram for better visualization (0–255)
    int maxVal = 0;
    for (int i = 0; i < ANGLES; ++i) {
        for (int j = 0; j < N; ++j) {
            if (sinogram[i][j] > maxVal)
                maxVal = sinogram[i][j];
        }
    }

    if (maxVal > 0) {
        for (int i = 0; i < ANGLES; ++i) {
            for (int j = 0; j < N; ++j) {
                sinogram[i][j] =
                    (sinogram[i][j] * 255) / maxVal;
            }
        }
    }
}

// ---------------------------------------------------
// Copy int[400][400] into short[400][400] for Display
// ---------------------------------------------------
void copyToShort(int src[][N], short dest[][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            dest[row][col] = static_cast<short>(src[row][col]);
        }
    }
}

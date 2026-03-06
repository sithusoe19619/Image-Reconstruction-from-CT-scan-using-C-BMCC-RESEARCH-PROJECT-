#include <iostream>
#include <fstream>
#include <cmath>
#include "Display.h"   // your new Display with SIZE = 400

using namespace std;

const int N      = 400;     // sinogram size 400 x 400
const int ANGLES = 180;     // number of projection angles
const double PI  = 3.14159265358979323846;

// Prototypes
void loadSinogram(int sino[][N], const string& fileName);
void back_projection(int sinogram[][N], int recon[][N]);
void copyToShort(int src[][N], short dest[][SIZE]);

int main() {
    int   Sinogram[N][N];    // 400 x 400 sinogram (int)
    int   Recon[N][N];       // 400 x 400 reconstruction (int)
    short SinoDisp[SIZE][SIZE];
    short ReconDisp[SIZE][SIZE];

    cout << "Loading sinogram_400x400.txt ..." << endl;
    loadSinogram(Sinogram, "sinogram_400x400.txt");

    // Optional: display the sinogram itself
    copyToShort(Sinogram, SinoDisp);
    Display sinoImg(SinoDisp, "sinogram_400x400");

    cout << "Performing back-projection on 400x400 sinogram ..." << endl;
    back_projection(Sinogram, Recon);

    // Copy reconstruction into short buffer for Display
    copyToShort(Recon, ReconDisp);
    Display reconImg(ReconDisp, "S_to_P_Reconstruction_400x400");

    cout << "Done." << endl;
    cout << "Open sinogram_400x400.html and Reconstruction_400x400.html in your browser." << endl;
    return 0;
}

// --------------------------------------------
// Load sinogram from text file into a 400x400 int array
// --------------------------------------------
void loadSinogram(int sino[][N], const string& fileName) {
    ifstream inFile(fileName);
    if (!inFile) {
        cout << "Error opening file: " << fileName << endl;
        exit(1);
    }

    for (int row = 0; row < N; ++row) {
        for (int col = 0; col < N; ++col) {
            inFile >> sino[row][col];
        }
    }

    inFile.close();
}

// --------------------------------------------
// Raw back-projection from sinogram to reconstructed image
// Same logic as your 180x180 version, adapted to N=400, ANGLES=400
// --------------------------------------------
void back_projection(int sinogram[][N], int recon[][N]) {
    double center = N / 2.0;

    // Accumulate in double for precision
    static double accumulator[N][N];

    // Initialize accumulator
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            accumulator[row][col] = 0.0;

    // Pixel-driven back-projection
    for (int angleIdx = 0; angleIdx < ANGLES; angleIdx++) {
        double theta    = angleIdx * PI / ANGLES;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        for (int row = 0; row < N; row++) {
            double y0 = row - center; // relative to center
            for (int col = 0; col < N; col++) {
                double x0 = col - center; // relative to center

                // Detector coordinate t
                double tPos = x0 * cosTheta + y0 * sinTheta;
                double t    = tPos + center;   // detector index

                if (t >= 0 && t <= N - 1) {
                    // 1D linear interpolation along detector axis
                    int    lowIdx  = (int)floor(t);
                    int    highIdx = lowIdx + 1;
                    double weight  = t - lowIdx;

                    double val = 0.0;
                    if (lowIdx >= 0 && highIdx < N) {
                        double lowValue  = sinogram[angleIdx][lowIdx];
                        double highValue = sinogram[angleIdx][highIdx];
                        val = (1.0 - weight) * lowValue + weight * highValue;
                    } else if (lowIdx >= 0 && lowIdx < N) {
                        val = sinogram[angleIdx][lowIdx];
                    }

                    accumulator[row][col] += val;
                }
            }
        }

        if ((angleIdx + 1) % 40 == 0) {
            cout << "  Back-projection progress: "
                 << (angleIdx + 1) << "/" << ANGLES << " angles" << endl;
        }
    }

    // Average over number of angles
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            accumulator[row][col] /= (double)ANGLES;

    // Normalize 0–255 for reconstruction
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

// --------------------------------------------
// Copy int[400][400] into short[400][400] for Display
// --------------------------------------------
void copyToShort(int src[][N], short dest[][SIZE]) {
    for (int row = 0; row < SIZE; ++row) {
        for (int col = 0; col < SIZE; ++col) {
            dest[row][col] = static_cast<short>(src[row][col]);
        }
    }
}

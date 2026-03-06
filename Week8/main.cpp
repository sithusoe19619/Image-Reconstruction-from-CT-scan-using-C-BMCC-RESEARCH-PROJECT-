#include <iostream>
#include <fstream>
#include <cmath>
#include "Display.h"

using namespace std;

const double PI = 3.14159265358979323846;

// ===================== PROTOTYPES =====================

// Memory
int**    allocate2DArray(int rows, int cols);
void     free2DArray(int** arr, int rows);
double** allocate2DArrayD(int rows, int cols);
void     free2DArrayD(double** arr, int rows);

// File I/O
void loadImage(int**& img, int& size, string fileName);
void loadSinogram(int**& sino, int& numAngles, int& detSize, string fileName);

// Radon Transform
double bilinearInterp(int** image, double x, double y, int N);
void   createSinogram(int** phantom, int** sinogram, int size, int numAngles);

// Fourier Transform
void DFT(double* x, double* realOut, double* imagOut, int N);
void iDFT(double* realIn, double* imagIn, double* xOut, int N);

// Ramp Filter & Filtering
void createRampFilter(double* filter, int N);
void filterSinogram(int** sino, double** filtered, int numAngles, int detSize, double* ramp);

// Back Projection
void backProject(double** sino, double** recon, int N, int numAngles);

// Scaling
int** scaleToUint8(double** data, int rows, int cols);

// ===================== MAIN =====================

int main() {

    // ============================================================
    // PART 1: DFT and iDFT Verification
    // ============================================================
    cout << "============================================" << endl;
    cout << "  PART 1: DFT / iDFT Verification"          << endl;
    cout << "============================================" << endl;

    double testSeq[] = { 1.0, 2.0, 3.0, 4.0 };
    int    testN    = 4;

    double* testReal = new double[testN];
    double* testImag = new double[testN];

    DFT(testSeq, testReal, testImag, testN);

    cout << "\n  Input sequence: ";
    for (int i = 0; i < testN; i++) cout << testSeq[i] << " ";
    cout << endl;

    cout << "\n  DFT Coefficients:" << endl;
    for (int k = 0; k < testN; k++) {
        cout << "    X[" << k << "] = " << testReal[k];
        if (testImag[k] >= 0) cout << " + " << testImag[k] << "i";
        else                  cout << " - " << -testImag[k] << "i";
        cout << endl;
    }

    // Inverse DFT — should recover original sequence
    double* testRecovered = new double[testN];
    iDFT(testReal, testImag, testRecovered, testN);

    cout << "\n  iDFT Recovered: ";
    for (int i = 0; i < testN; i++) cout << testRecovered[i] << " ";
    cout << endl;

    delete[] testReal;
    delete[] testImag;
    delete[] testRecovered;

    // ============================================================
    // PART 2: Phantom 1 (Shapes) — 180 x 180
    // ============================================================
    cout << "\n============================================" << endl;
    cout << "  PART 2: Phantom 1 (Shapes) 180x180"        << endl;
    cout << "============================================" << endl;

    int  size1   = 0;
    int** Pixels1 = nullptr;
    loadImage(Pixels1, size1, "shapesPhantom.txt");

    if (Pixels1 && size1 > 0) {
        DynamicDisplay dispPhantom1(Pixels1, size1, "Phantom_01");
        cout << "  -> Phantom_01.html generated" << endl;

        // --- Sinogram ---
        int  numAngles1 = 180;
        int** Sino1     = allocate2DArray(numAngles1, size1);
        createSinogram(Pixels1, Sino1, size1, numAngles1);
        DynamicDisplay dispSino1(Sino1, size1, "Sinogram_01");
        cout << "  -> Sinogram_01.html generated" << endl;

        // --- Ramp filter (size 180) ---
        double* ramp180 = new double[size1];
        createRampFilter(ramp180, size1);
        cout << "  Ramp filter (size " << size1 << ") first values: ";
        for (int k = 0; k < 6; k++) cout << ramp180[k] << " ";
        cout << "..." << endl;

        // --- Filter sinogram in frequency domain ---
        double** filtSino1 = allocate2DArrayD(numAngles1, size1);
        filterSinogram(Sino1, filtSino1, numAngles1, size1, ramp180);

        // --- Display filtered sinogram (scaled 0-255) ---
        int** filtSinoDisp1 = scaleToUint8(filtSino1, numAngles1, size1);
        DynamicDisplay dispFiltSino1(filtSinoDisp1, size1, "FilteredSinogram_01");
        cout << "  -> FilteredSinogram_01.html generated" << endl;

        // --- Reconstruct using UNSCALED filtered sinogram ---
        double** recon1 = allocate2DArrayD(size1, size1);
        backProject(filtSino1, recon1, size1, numAngles1);

        // --- Display reconstruction (scaled 0-255) ---
        int** reconDisp1 = scaleToUint8(recon1, size1, size1);
        DynamicDisplay dispRecon1(reconDisp1, size1, "Reconstruction_01");
        cout << "  -> Reconstruction_01.html generated" << endl;

        // Cleanup
        free2DArray(Sino1, numAngles1);
        free2DArrayD(filtSino1, numAngles1);
        free2DArray(filtSinoDisp1, numAngles1);
        free2DArrayD(recon1, size1);
        free2DArray(reconDisp1, size1);
        free2DArray(Pixels1, size1);
        delete[] ramp180;
    }

    // ============================================================
    // PART 2b: Phantom 2 (Head) — 180 x 180
    // ============================================================
    cout << "\n============================================" << endl;
    cout << "  PART 2: Phantom 2 (Head) 180x180"          << endl;
    cout << "============================================" << endl;

    int  size2   = 0;
    int** Pixels2 = nullptr;
    loadImage(Pixels2, size2, "headPhantom.txt");

    if (Pixels2 && size2 > 0) {
        DynamicDisplay dispPhantom2(Pixels2, size2, "Phantom_02");
        cout << "  -> Phantom_02.html generated" << endl;

        int  numAngles2 = 180;
        int** Sino2     = allocate2DArray(numAngles2, size2);
        createSinogram(Pixels2, Sino2, size2, numAngles2);
        DynamicDisplay dispSino2(Sino2, size2, "Sinogram_02");
        cout << "  -> Sinogram_02.html generated" << endl;

        double* ramp180 = new double[size2];
        createRampFilter(ramp180, size2);

        double** filtSino2 = allocate2DArrayD(numAngles2, size2);
        filterSinogram(Sino2, filtSino2, numAngles2, size2, ramp180);

        int** filtSinoDisp2 = scaleToUint8(filtSino2, numAngles2, size2);
        DynamicDisplay dispFiltSino2(filtSinoDisp2, size2, "FilteredSinogram_02");
        cout << "  -> FilteredSinogram_02.html generated" << endl;

        double** recon2 = allocate2DArrayD(size2, size2);
        backProject(filtSino2, recon2, size2, numAngles2);

        int** reconDisp2 = scaleToUint8(recon2, size2, size2);
        DynamicDisplay dispRecon2(reconDisp2, size2, "Reconstruction_02");
        cout << "  -> Reconstruction_02.html generated" << endl;

        free2DArray(Sino2, numAngles2);
        free2DArrayD(filtSino2, numAngles2);
        free2DArray(filtSinoDisp2, numAngles2);
        free2DArrayD(recon2, size2);
        free2DArray(reconDisp2, size2);
        free2DArray(Pixels2, size2);
        delete[] ramp180;
    }

    // ============================================================
    // PART 3: 400x400 Sinogram (Real Data)
    // ============================================================
    cout << "\n============================================" << endl;
    cout << "  PART 3: 400x400 Sinogram (Real Data)"      << endl;
    cout << "============================================" << endl;

    int  numAngles400 = 0, detSize400 = 0;
    int** Sino400     = nullptr;
    loadSinogram(Sino400, numAngles400, detSize400, "sinogram_400x400.txt");

    if (Sino400 && detSize400 > 0) {
        // Display original sinogram (180 rows x 400 cols)
        DynamicDisplay dispSino400(Sino400, numAngles400, detSize400, "Sinogram_400");
        cout << "  -> Sinogram_400.html generated" << endl;

        // --- Ramp filter (size 400) ---
        double* ramp400 = new double[detSize400];
        createRampFilter(ramp400, detSize400);
        cout << "  Ramp filter (size " << detSize400 << ") first values: ";
        for (int k = 0; k < 6; k++) cout << ramp400[k] << " ";
        cout << "..." << endl;

        // --- Filter sinogram ---
        double** filtSino400 = allocate2DArrayD(numAngles400, detSize400);
        filterSinogram(Sino400, filtSino400, numAngles400, detSize400, ramp400);

        // --- Display filtered sinogram (scaled 0-255) ---
        int** filtSinoDisp400 = scaleToUint8(filtSino400, numAngles400, detSize400);
        DynamicDisplay dispFiltSino400(filtSinoDisp400, numAngles400, detSize400, "FilteredSinogram_400");
        cout << "  -> FilteredSinogram_400.html generated" << endl;

        // --- Reconstruct 400x400 using UNSCALED filtered sinogram ---
        double** recon400 = allocate2DArrayD(detSize400, detSize400);
        backProject(filtSino400, recon400, detSize400, numAngles400);

        // --- Display reconstruction (scaled 0-255) ---
        int** reconDisp400 = scaleToUint8(recon400, detSize400, detSize400);
        DynamicDisplay dispRecon400(reconDisp400, detSize400, "Reconstruction_400");
        cout << "  -> Reconstruction_400.html generated" << endl;

        free2DArray(Sino400, numAngles400);
        free2DArrayD(filtSino400, numAngles400);
        free2DArray(filtSinoDisp400, numAngles400);
        free2DArrayD(recon400, detSize400);
        free2DArray(reconDisp400, detSize400);
        delete[] ramp400;
    }

    cout << "\nAll files generated successfully!" << endl;
    cout << "Open the HTML files in your browser to view results." << endl;

    return 0;
}

// ===================== MEMORY MANAGEMENT =====================

int** allocate2DArray(int rows, int cols) {
    int** arr = new int*[rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new int[cols];
        for (int j = 0; j < cols; j++)
            arr[i][j] = 0;
    }
    return arr;
}

void free2DArray(int** arr, int rows) {
    if (arr) {
        for (int i = 0; i < rows; i++) delete[] arr[i];
        delete[] arr;
    }
}

double** allocate2DArrayD(int rows, int cols) {
    double** arr = new double*[rows];
    for (int i = 0; i < rows; i++) {
        arr[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            arr[i][j] = 0.0;
    }
    return arr;
}

void free2DArrayD(double** arr, int rows) {
    if (arr) {
        for (int i = 0; i < rows; i++) delete[] arr[i];
        delete[] arr;
    }
}

// ===================== FILE I/O =====================

void loadImage(int**& img, int& size, string fileName) {
    ifstream file(fileName);
    if (!file) {
        cout << "  File not found: " << fileName << endl;
        img  = nullptr;
        size = 0;
        return;
    }

    // Count total values to deduce image size
    int value, count = 0;
    while (file >> value) count++;

    size = (int)sqrt((double)count);
    if (size * size != count) {
        cout << "  Error: not a square image (" << count << " values)" << endl;
        img  = nullptr;
        size = 0;
        return;
    }

    file.clear();
    file.seekg(0);

    img = allocate2DArray(size, size);
    for (int row = 0; row < size; row++)
        for (int col = 0; col < size; col++)
            file >> img[row][col];

    file.close();
    cout << "  Loaded image: " << size << "x" << size << endl;
}

void loadSinogram(int**& sino, int& numAngles, int& detSize, string fileName) {
    ifstream file(fileName);
    if (!file) {
        cout << "  File not found: " << fileName << endl;
        sino       = nullptr;
        numAngles  = 0;
        detSize    = 0;
        return;
    }

    numAngles = 180;
    detSize   = 400;

    sino = allocate2DArray(numAngles, detSize);
    for (int a = 0; a < numAngles; a++)
        for (int d = 0; d < detSize; d++)
            file >> sino[a][d];

    file.close();
    cout << "  Loaded sinogram: " << numAngles << " angles x " << detSize << " detectors" << endl;
}

// ===================== RADON TRANSFORM =====================

double bilinearInterp(int** image, double x, double y, int N) {
    if (x < 0 || x >= N - 1 || y < 0 || y >= N - 1)
        return 0.0;

    int    x0 = (int)x,  y0 = (int)y;
    int    x1 = x0 + 1,  y1 = y0 + 1;
    double dx = x - x0,  dy = y - y0;

    return (1 - dx) * (1 - dy) * image[y0][x0] +
            dx      * (1 - dy) * image[y0][x1] +
           (1 - dx) *  dy      * image[y1][x0] +
            dx      *  dy      * image[y1][x1];
}

void createSinogram(int** phantom, int** sinogram, int size, int numAngles) {
    double center = size / 2.0;

    for (int angleIdx = 0; angleIdx < numAngles; angleIdx++) {
        double theta    = angleIdx * PI / numAngles;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        for (int t = 0; t < size; t++) {
            double tPos  = t - center;
            double sum   = 0.0;
            int    count = 0;

            for (double s = -center; s <= center; s += 0.5) {
                double x = tPos * cosTheta - s * sinTheta + center;
                double y = tPos * sinTheta + s * cosTheta + center;

                if (x >= 0 && x < size - 1 && y >= 0 && y < size - 1) {
                    sum += bilinearInterp(phantom, x, y, size);
                    count++;
                }
            }
            sinogram[angleIdx][t] = (count > 0) ? (int)(sum / count) : 0;
        }

        if ((angleIdx + 1) % 30 == 0)
            cout << "  Sinogram: " << (angleIdx + 1) << "/" << numAngles << " angles" << endl;
    }

    // Normalize to 0-255
    int maxVal = 0;
    for (int i = 0; i < numAngles; i++)
        for (int j = 0; j < size; j++)
            if (sinogram[i][j] > maxVal) maxVal = sinogram[i][j];

    if (maxVal > 0)
        for (int i = 0; i < numAngles; i++)
            for (int j = 0; j < size; j++)
                sinogram[i][j] = (sinogram[i][j] * 255) / maxVal;
}

// ===================== FOURIER TRANSFORM =====================

// Discrete Fourier Transform
// X[k] = sum_{n=0}^{N-1} x[n] * e^{-2*pi*i*k*n/N}
void DFT(double* x, double* realOut, double* imagOut, int N) {
    for (int k = 0; k < N; k++) {
        double sumR = 0.0;
        double sumI = 0.0;
        for (int n = 0; n < N; n++) {
            double angle = 2.0 * PI * k * n / (double)N;
            sumR += x[n] * cos(angle);
            sumI -= x[n] * sin(angle);
        }
        realOut[k] = sumR;
        imagOut[k] = sumI;
    }
}

// Inverse Discrete Fourier Transform
// x[n] = (1/N) * sum_{k=0}^{N-1} X[k] * e^{+2*pi*i*k*n/N}
void iDFT(double* realIn, double* imagIn, double* xOut, int N) {
    for (int n = 0; n < N; n++) {
        double sumR = 0.0;
        for (int k = 0; k < N; k++) {
            double angle = 2.0 * PI * k * n / (double)N;
            sumR += realIn[k] * cos(angle) - imagIn[k] * sin(angle);
        }
        xOut[n] = sumR / (double)N;
    }
}

// ===================== RAMP FILTER =====================

// H[k] = |k|  in the DFT frequency index sense:
//   H[0]           = 0          (DC)
//   H[1..N/2]      = 1..N/2     (positive frequencies)
//   H[N/2+1..N-1]  = N/2-1..1   (negative frequencies, symmetric)
void createRampFilter(double* filter, int N) {
    filter[0] = 0;
    for (int k = 1; k <= N / 2; k++)
        filter[k] = (double)k;
    for (int k = N / 2 + 1; k < N; k++)
        filter[k] = (double)(N - k);
}

// ===================== SINOGRAM FILTERING =====================

// For each row of the sinogram:
//   1) DFT  -> frequency domain
//   2) multiply real and imaginary parts by rampFilter
//   3) iDFT -> back to spatial domain
void filterSinogram(int** sino, double** filtered, int numAngles, int detSize, double* ramp) {
    double* row         = new double[detSize];
    double* realPart    = new double[detSize];
    double* imagPart    = new double[detSize];
    double* filteredRow = new double[detSize];

    for (int angle = 0; angle < numAngles; angle++) {

        // Convert int row to double
        for (int t = 0; t < detSize; t++)
            row[t] = (double)sino[angle][t];

        // Step 1: DFT
        DFT(row, realPart, imagPart, detSize);

        // Step 2: multiply by ramp filter
        for (int k = 0; k < detSize; k++) {
            realPart[k] *= ramp[k];
            imagPart[k] *= ramp[k];
        }

        // Step 3: iDFT
        iDFT(realPart, imagPart, filteredRow, detSize);

        // Store result (keep as double for later reconstruction)
        for (int t = 0; t < detSize; t++)
            filtered[angle][t] = filteredRow[t];

        if ((angle + 1) % 30 == 0)
            cout << "  Filtering: " << (angle + 1) << "/" << numAngles << " rows" << endl;
    }

    delete[] row;
    delete[] realPart;
    delete[] imagPart;
    delete[] filteredRow;
}

// ===================== SCALING TO 0-255 =====================

// Min-max normalization: maps [min, max] -> [0, 255]
// Many filtered values are negative, so we shift by min first.
int** scaleToUint8(double** data, int rows, int cols) {
    double minVal = data[0][0];
    double maxVal = data[0][0];

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (data[i][j] < minVal) minVal = data[i][j];
            if (data[i][j] > maxVal) maxVal = data[i][j];
        }

    int** scaled = allocate2DArray(rows, cols);
    double range = maxVal - minVal;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            scaled[i][j] = (range > 0.0) ? (int)round((data[i][j] - minVal) / range * 255.0) : 0;

    return scaled;
}

// ===================== FILTERED BACK PROJECTION =====================

// Pixel-driven back-projection using the (double) filtered sinogram.
// Result is NOT scaled here — scaling is done separately before display.
void backProject(double** sino, double** recon, int N, int numAngles) {
    double center = N / 2.0;

    // Initialize
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            recon[i][j] = 0.0;

    for (int angleIdx = 0; angleIdx < numAngles; angleIdx++) {
        double theta    = angleIdx * PI / numAngles;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);

        for (int row = 0; row < N; row++) {
            for (int col = 0; col < N; col++) {
                double x0 = col - center;
                double y0 = row - center;

                // Project pixel (col, row) onto detector axis
                double t = x0 * cosTheta + y0 * sinTheta + center;

                if (t >= 0.0 && t <= (double)(N - 1)) {
                    int    t0 = (int)floor(t);
                    int    t1 = t0 + 1;
                    double w  = t - t0;

                    double val = 0.0;
                    if (t0 >= 0 && t1 < N)
                        val = (1.0 - w) * sino[angleIdx][t0] + w * sino[angleIdx][t1];
                    else if (t0 >= 0 && t0 < N)
                        val = sino[angleIdx][t0];

                    recon[row][col] += val;
                }
            }
        }

        if ((angleIdx + 1) % 30 == 0)
            cout << "  Reconstruction: " << (angleIdx + 1) << "/" << numAngles << " angles" << endl;
    }

    // Average over angles
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            recon[row][col] /= (double)numAngles;
}

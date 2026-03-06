#include <iostream>
#include <fstream>
#include <cmath>
#include <climits>
#include <string>
#include "Display.h"
using namespace std;

using namespace std;

const int N = 180; // image is N x N
const double PI = 3.14159265358979323846;

void loadImage(int img[][N], const string& fileName){
    ifstream f(fileName);
    if(!f){
        cerr << "File does not exist: " << fileName << "\n";
        return;
    }
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            f >> img[r][c];
        }
    }
    f.close();
}

// Rotate src by +theta (radians) around center using inverse mapping + nearest neighbor.
// Result written to dst.
void rotateImageNearest(int src[][N], int dst[][N], double theta){
    double cx = (N-1)/2.0;
    double cy = (N-1)/2.0;
    double ct = cos(theta);
    double st = sin(theta);

    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            // destination pixel (c, r) in rotated image corresponds to source (x', y')
            double x = c - cx;
            double y = r - cy;
            // inverse rotation by +theta to sample from source? We are generating the image rotated by -theta.
            // Mapping destination -> source using rotation matrix for -theta:
            // [x'] = [ cos(-θ) -sin(-θ)] [x]
            // [y']   [ sin(-θ)  cos(-θ)] [y]
            // cos(-θ)=cosθ, sin(-θ)=-sinθ
            double xs =  ct * x + st * y;     // since -sin(-θ)=+sinθ in formula with signs resolved
            double ys = -st * x + ct * y;

            xs += cx;
            ys += cy;

            int xs_i = (int)round(xs);
            int ys_i = (int)round(ys);

            if(xs_i>=0 && xs_i<N && ys_i>=0 && ys_i<N){
                dst[r][c] = src[ys_i][xs_i];
            }else{
                dst[r][c] = 0;
            }
        }
    }
}

// Computing sinogram : for each angle a=0..179°, rotate by -a so detector aligns with columns, then sum columns.
void computeSinogram(int img[][N], int sino[][N]){
    static int rot[N][N];
    for(int a=0;a<N;a++){
        double theta = (PI/180.0) * a;
        rotateImageNearest(img, rot, -theta);
        for(int c=0;c<N;c++){
            long long sum = 0;
            for(int r=0;r<N;r++) sum += rot[r][c];
            sino[a][c] = (int)sum; // temporary unnormalized
        }
    }
    // Normalizing to 0..255 for display
    int mn = INT_MAX, mx = INT_MIN;
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            mn = min(mn, sino[r][c]);
            mx = max(mx, sino[r][c]);
        }
    }
    if(mx==mn) mx = mn+1;
    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++){
            double v = (sino[r][c] - mn) * 255.0 / (mx - mn);
            int iv = (int)round(v);
            if(iv<0) iv=0; if(iv>255) iv=255;
            sino[r][c] = iv;
        }
    }
}

int main(){
    // Loading three phantoms
    int phantomA[N][N], phantomB[N][N], phantomC[N][N];
    loadImage(phantomA, "testPhantom.txt");   // Phantom 1
    loadImage(phantomB, "shapesPhantom.txt"); // Phantom 2
    loadImage(phantomC, "headPhantom.txt");   // Phantom 3

    // Computing sinograms
    int sinoA[N][N], sinoB[N][N], sinoC[N][N];
    computeSinogram(phantomA, sinoA);
    computeSinogram(phantomB, sinoB);
    computeSinogram(phantomC, sinoC);

    // Displaying original phantoms and their sinograms
    Display dA(phantomA, "Phantom_1");
    Display sA(sinoA, "Sinogram_1");

    Display dB(phantomB, "Phantom_2");
    Display sB(sinoB, "Sinogram_2");

    Display dC(phantomC, "Phantom_3");
    Display sC(sinoC, "Sinogram_3");

    return 0;
}

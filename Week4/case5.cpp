#include <iostream>
#include "Display.h"
using namespace std;

const int N = 500;
const int n = 10;


int main() {

  //Vertical Measurement:
  int verticalMeasurements[n] = {0, 0, 10, 10, 2, 2, 2, 2, 0, 0};
    cout << "\n\nVertical measurements"<<endl;
    for (int i = 0; i < n; i++)
        cout << verticalMeasurements[i] << " ";


  //Horizontal Measurement:
  int horizontalMeasurements[n] = {2, 2, 2, 2, 2, 2, 2, 2, 6, 6};
    cout << "\n\nHorizontal measurements"<<endl;
    for (int i = 0; i < n; i++) 
        cout << horizontalMeasurements[i] << " ";


  //Diagonal Measurement:
  int diagonalMeasurements[2*n - 1] = {0, 0, 1, 2, 3, 4, 4, 4, 3, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0};
    cout << "\n\nDiagonal measurements" << endl;
    for (int i = 0; i < 2*n - 1; ++i) 
        cout << diagonalMeasurements[i] << " ";


  //Anti-Diagonal Measurement:
  int antiDiagonalMeasurements[2*n - 1] = {0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0};
    cout << "\n\nAnti-diagonal measurements"<<endl;
    for (int i = 0; i < 2*n - 1; ++i) 
        cout << antiDiagonalMeasurements[i] << " ";



  //Reconstruction
  short reconstruction[n][n];
    for (int row = 0; row < n; row++)
      for (int col = 0; col < n; col++)
        reconstruction[row][col] = 0;


  //Vertical Reconstruction
    cout << "\n\nReconstruction from vertical rays"<<endl;
    for (int row = 0; row < n; row++)
      for (int col = 0; col < n; col++)
        reconstruction[row][col] += verticalMeasurements[col];  

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++)
          cout << reconstruction[row][col]<< " ";
      cout << endl;
    } 


  //Horizontal Reconstruction
    cout << "\n\nReconstruction from horizontal rays"<<endl;
    for (int row = 0; row < n; row++)
      for (int col = 0; col < n; col++)
        reconstruction[row][col] += horizontalMeasurements[row];

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++)
          cout << reconstruction[row][col] << " ";
      cout << endl;
    } 


  //Diagonal Reconstruction (reverse index to align with provided orientation)
    cout << "\n\nReconstruction from diagonal rays"<<endl;
    for (int row = 0; row < n; row++)
      for (int col = 0; col < n; col++)
        reconstruction[row][col] += diagonalMeasurements[(2*n - 2) - (row - col + (n - 1))];

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++)
        cout << reconstruction[row][col] << " ";
      cout << endl;
    }


  //Anti-Diagonal Reconstruction (reverse index so edges align)
    cout << "\n\nReconstruction from anti-diagonal rays"<<endl;
    for (int row = 0; row < n; row++)
      for (int col = 0; col < n; col++)
        reconstruction[row][col] += antiDiagonalMeasurements[(2*n - 2) - (row + col)];

    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++)
          cout << reconstruction[row][col] << " ";
      cout << endl;
    }


  // ===== FILTERING/THRESHOLDING =====
  double threshold = 11.5;
  short binary[n][n];

    cout << "\n\nFinal reconstruction after removing blurring - threshold = " << threshold << "\n";
    for (int r = 0; r < n; ++r) {
      for (int c = 0; c < n; ++c) {
        if (reconstruction[r][c] >= threshold)
          binary[r][c] = 1;
        else
          binary[r][c] = 0;
        cout << binary[r][c] << " ";
      }
      cout << endl;
    }

  // Display threshold result visually
  short binaryGray[N][N];
    for (int R = 0; R < N; ++R) {
      int rr = R * n / N;
      for (int C = 0; C < N; ++C) {
        int cc = C * n / N;
        binaryGray[R][C] = (binary[rr][cc] == 1 ? 0 : 224);
      }
    }
    Display binaryImage(binaryGray, "Case5Thresholded");

  return 0;
}

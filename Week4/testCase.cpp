#include <iostream>
#include "Display.h"
using namespace std;

const int N = 500;
const int n = 10;


int main() {
  
  short phantom[n][n] = {{1,1,0,0,0,0,0,0,1,1},{1,1,1,0,0,0,0,1,1,1},{1,1,1,1,0,0,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,0,1,1,1,1,0,1,1},{1,1,0,0,1,1,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1}}; 
    cout << "\nPhantom" << endl;
  //Phantom  
  int row, col;  
    for (row = 0; row < n; row++) {
      for (col = 0; col < n; col++)
          cout << phantom[row][col] << " ";
      cout << endl;
    } 
  //grayImage
  short grayImage[N][N];
    for (row = 0; row < N; row++)
      for (col = 0; col < N; col++)
          grayImage[row][col] = 0;



  int r,c;
    for (row = 0; row < N; row++)
      for (col = 0; col < N; col++) {
        r = row/50;
        c = col/50;
        if (phantom[r][c] == 0)
          grayImage[row][col] = 224;
      } 
    Display image01(grayImage, "testCaseOutput");



  //Vertical Measurement:
  int verticalMeasurements[n] = {0};
    for (int c = 0; c < n; c++)
      for (int r = 0; r < n; r++)
        verticalMeasurements[c] += phantom[r][c];
    cout << "\n\nVertical measurements"<<endl;
    for (int i = 0; i < n; i++)
        cout << verticalMeasurements[i] << " ";

  

  //Horizontal Measurement:
  int horizontalMeasurements[n] = {0};
    for (int r = 0; r < n; r++)
      for (int c = 0; c < n; c++)
        horizontalMeasurements[r] += phantom[r][c];
    cout << "\n\nHorizontal measurements"<<endl;
    for (int i = 0; i < n; i++) 
        cout << horizontalMeasurements[i] << " ";



  //Diagonal Measurement:
  int diagonalMeasurements[2*n - 1] = {0};
    for (int r = 0; r < n; ++r)
      for (int c = 0; c < n; ++c)
        diagonalMeasurements[r - c + (n - 1)] += phantom[r][c];
    cout << "\n\nDiagonal measurements" << endl;
    for (int i = 2*n - 2; i >= 0; --i)
        cout << diagonalMeasurements[i] << " ";



  //Anti-Diagonal Measurement:
  int antiDiagonalMeasurements[2*n - 1] = {0};
    for (int r = 0; r < n; ++r)
      for (int c = 0; c < n; ++c)
        antiDiagonalMeasurements[r + c] += phantom[r][c];
    cout << "\n\nAnti-diagonal measurements"<<endl;
    for (int i = 0; i < 2*n - 1; ++i) 
        cout << antiDiagonalMeasurements[i] << " ";



  //Reconstruction
  short reconstruction[n][n];
    for (row = 0; row < n; row++)
      for (col = 0; col < n; col++)
        reconstruction[row][col] = 0;


  
  //Vertical Reconstruction
    cout << "\n\nReconstruction from vertical rays"<<endl;
    for (row = 0; row < n; row++)
      for (col = 0; col < n; col++)
        reconstruction[row][col] += verticalMeasurements[col];  

    for (row = 0; row < n; row++) {
      for (col = 0; col < n; col++)
          cout << reconstruction[row][col]<< " ";
      cout << endl;
    } 



  //Horizontal Reconstruction
    cout << "\n\nReconstruction from horizontal rays"<<endl;
    for (row = 0; row < n; row++)
      for (col = 0; col < n; col++)
        reconstruction[row][col] += horizontalMeasurements[row];

    for (row = 0; row < n; row++) {
      for (col = 0; col < n; col++)
          cout << reconstruction[row][col] << " ";
      cout << endl;
    } 



  //Diagonal Reconstruction
    cout << "\n\nReconstruction from diagonal rays"<<endl;
    for (row = 0; row < n; row++)
      for (col = 0; col < n; col++)
        reconstruction[row][col] += diagonalMeasurements[row - col + (n - 1)];

    for (row = 0; row < n; row++) {
      for (col = 0; col < n; col++)
        cout << reconstruction[row][col] << " ";
      cout << endl;
    }



  //Anti-Diagonal Reconstruction
    cout << "\n\nReconstruction from anti-diagonal rays"<<endl;
    for (row = 0; row < n; row++)
      for (col = 0; col < n; col++)
        reconstruction[row][col] += antiDiagonalMeasurements[row + col];

    for (row = 0; row < n; row++) {
      for (col = 0; col < n; col++)
          cout << reconstruction[row][col] << " ";
      cout << endl;
    }


  

  // ===== FILTERING/THRESHOLDING =====
  double threshold = 18.5;
  short binary[n][n];

    cout << "\n\nFinal reconstruction after removing blurring - threshold = 18.5\n";
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

//Displaying the threshold
  short binaryGray[N][N];
    for (int R = 0; R < N; ++R) {
      int rr = R * n / N;
      for (int C = 0; C < N; ++C) {
        int cc = C * n / N;
        binaryGray[R][C] = (binary[rr][cc] == 1 ? 0 : 224);
      }
    }
    Display binaryImage(binaryGray, "testCaseThresholded");
  return 0;
}



#include <iostream>
#include "Display.h"
using namespace std;

const int N = 500;
const int n = 10;


int main() {
  short phantom[n][n] = {{1,1,0,0,0,0,0,0,1,1},{1,1,1,0,0,0,0,1,1,1},{1,1,1,1,0,0,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{1,1,0,1,1,1,1,0,1,1},{1,1,0,0,1,1,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1},{1,1,0,0,0,0,0,0,1,1}}; 

  //double threshold = 18.5;

  cout << "\nPhantom" << endl;
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

  Display image01(grayImage, "output");

  int verticalMeasurements[10] = {10,10,3,3,3,3,3,3,10,10};
  int horizontalMeasurements[10] = {4,6,8,10,8,6,4,4,4,4};
  int diagonalMeasurements[19] = {1,2,2,2,2,2,2,2,6,8,7,4,3,4,3,3,2,2,1};
  int antiDiagonalMeasurements[19] = {1,2,2,3,3,4,3,4,7,8,6,2,2,2,2,2,2,2,1};

  cout << "\n\nVertical measurements"<<endl;
  for (col = 0; col < 10; col++)
    cout << verticalMeasurements[col] << " ";

    cout << "\n\nHorizontal measurements"<<endl;
  for (col = 0; col < 10; col++)
    cout << horizontalMeasurements[col] << " ";

  cout << "\n\nDiagonal measurements"<<endl;
  for (col = 0; col < 19; col++)
    cout << diagonalMeasurements[col] << " ";

  cout << "\n\nAnti-diagonal measurements"<<endl;
  for (col = 0; col < 19; col++)
    cout << antiDiagonalMeasurements[col] << " "; 

  //Reconstruction
  short reconstruction[n][n];
  for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
      reconstruction[row][col] = 0;

  cout << "\n\nReconstruction from vertical rays"<<endl;
  for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
      reconstruction[row][col] += verticalMeasurements[col];  

  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
        cout << reconstruction[row][col]<< " ";
    cout << endl;
  } 

  cout << "\n\nReconstruction from horizontal rays"<<endl;
  for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
      reconstruction[row][col] += horizontalMeasurements[row];

  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
        cout << reconstruction[row][col] << " ";
    cout << endl;
  } 

  cout << "\n\nReconstruction from diagonal rays"<<endl;
  //To be done

  cout << "\n\nReconstruction from anti-diagonal rays"<<endl;
  //To be done

  /*

Reconstruction from diagonal rays
22 21 11 10 11 10 10 9 16 15 
22 24 16 13 12 13 12 12 18 18 
20 24 19 18 15 14 15 14 21 20 
22 22 19 21 20 17 16 17 23 23 
20 20 13 17 19 18 15 14 22 21 
18 18 11 11 15 17 16 13 19 20 
16 16 9 9 9 13 15 14 18 17 
16 16 9 9 9 9 13 15 21 18 
16 16 9 9 9 9 9 13 22 21 
15 16 9 9 9 9 9 9 20 22 


Reconstruction from anti-diagonal rays
23 23 13 13 14 14 13 13 23 23 
24 26 19 16 16 16 16 19 26 24 
22 27 22 22 18 18 22 22 27 22 
25 25 23 24 24 24 24 23 25 25 
23 24 16 21 26 26 21 16 24 23 
22 21 15 18 23 23 18 15 21 22 
19 20 16 17 15 15 17 16 20 19 
20 23 17 15 11 11 15 17 23 20 
23 24 15 11 11 11 11 15 24 23 
23 22 11 11 11 11 11 11 22 23 

Final reconstruction after removing blurring - threshold = 18.5
1 1 0 0 0 0 0 0 1 1 
1 1 1 0 0 0 0 1 1 1 
1 1 1 1 0 0 1 1 1 1 
1 1 1 1 1 1 1 1 1 1 
1 1 0 1 1 1 1 0 1 1 
1 1 0 0 1 1 0 0 1 1 
1 1 0 0 0 0 0 0 1 1 
1 1 0 0 0 0 0 0 1 1 
1 1 0 0 0 0 0 0 1 1 
1 1 0 0 0 0 0 0 1 1 
#####################################
CASE 1:
Another Phantom:
phantom[n][n] = {{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1},{0,0,0,0,0,0,1,1,1,0},{0,0,0,0,0,1,1,1,0,0},{0,0,0,0,1,1,1,0,0,0},{0,0,0,1,1,1,0,0,0,0},{0,0,1,1,1,0,0,0,0,0},{0,1,1,1,0,0,0,0,0,0},{1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1}}; // threshold = 18.5;
#####################################
CASE 2:
Other Measurments (use threshold 16.5):
Vertical measurements
0 0 10 10 6 6 6 6 0 0 
Horizontal measurements
6 6 2 2 6 6 2 2 6 6 
Diagonal measurements
0 0 1 2 3 4 4 4 4 4 4 4 3 2 2 2 1 0 0 
Anti-diagonal measurements
0 0 1 2 3 4 4 4 4 4 4 4 3 2 2 2 1 0 0
#####################################
CASE 3:
Other Measurments (use threshold 19.5):
Vertical measurements
0 10 10 2 2 2 2 10 10 0 
Horizontal measurements
4 4 4 4 8 8 4 4 4 4 
Diagonal measurements
0 1 2 2 2 2 2 4 6 6 6 4 2 2 2 2 2 1 0 
Anti-diagonal measurements
0 1 2 2 2 2 2 4 6 6 6 4 2 2 2 2 2 1 0 
#####################################
CASE 4:
Other Measurments (use threshold 13.5):
Vertical measurements
0 2 2 2 10 10 2 2 2 0 
Horizontal measurements
8 8 2 2 2 2 2 2 2 2 
Diagonal measurements
0 0 0 0 1 2 2 2 2 3 4 4 3 2 2 2 2 1 0 
Anti-diagonal measurements
0 1 2 2 2 2 3 4 4 3 2 2 2 2 1 0 0 0 0
#####################################
CASE 5:
Other Measurments (use threshold 11.5):
Vertical measurements
0 0 10 10 2 2 2 2 0 0 
Horizontal measurements
2 2 2 2 2 2 2 2 6 6 
Diagonal measurements
0 0 1 2 3 4 4 4 3 2 2 2 1 0 0 0 0 0 0 
Anti-diagonal measurements
0 0 1 2 2 2 2 2 2 2 2 2 2 2 2 2 1 0 0 

  */


  return 0;

}
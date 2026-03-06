#include <iostream>
using namespace std;

const int n = 10;

int main() {
  short verticalRays[n][n];
  short horizontalRays[n][n];
  short diagonalRays[n][n];
  short antiDiagonalRays[n][n];
  int row, col;
  short ray;

  //Computing vertical rays
  for (row = 0; row < n; row++) {
    ray = 11;
    for (col = 0; col < n; col++) {
      verticalRays[row][col] = ray;
      ray++;
    }		
  }

  //Computing horizontal rays
  ray = 11;
  for (row = 0; row < n; row++) {		
    for (col = 0; col < n; col++) {
      horizontalRays[row][col] = ray;
    }		
    ray++;
  }

  //Computing diagonal rays
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++) {
      diagonalRays[row][col] = 11 + (n - 1) + (col - row);
    }
  }
  
  //Computing anti-diagonal rays
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++) {
      antiDiagonalRays[row][col] = 11 + (row + col);
    }
  }

  //**********************************
  //*****Displaying vertical rays*****
  //**********************************

  //Displaying vertical rays
  cout << endl << "Vertical rays:" << endl;
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
      cout << verticalRays[row][col] << " ";
    cout << endl;
  }

  //Displaying horizontal rays
  cout << endl << "Horizontal rays:" << endl;
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
      cout << horizontalRays[row][col] << " ";
    cout << endl;
  }

  //Displaying diagonal rays
  cout << endl << "Diagonal rays:" << endl;
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
      cout << diagonalRays[row][col] << " ";
    cout << endl;
  }
  //Displaying anti-diagonal rays
  cout << endl << "Anti-diagonal rays:" << endl;
  for (row = 0; row < n; row++) {
    for (col = 0; col < n; col++)
      cout << antiDiagonalRays[row][col] << " ";
    cout << endl;
  }
  return 0;
}

#include <iostream>
#include "Display.h"
using namespace std;

const int N = 500; //Image Size N x N

int main() {		
  short Pixels[N][N];

  for (int row = 0; row < N; row++) {
    for (int col = 0; col < N; col++) {      
      Pixels[row][col] = row ;      
    }		
  }

  Display image(Pixels,"output5");

  return 0;
}
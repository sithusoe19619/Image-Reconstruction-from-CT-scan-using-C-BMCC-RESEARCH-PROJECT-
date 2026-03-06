#include <iostream>
#include <fstream>
#include "Display.h"
using namespace std;

const int N = 180; //Image Size N x N

void loadImage(int[][N], string);

int main() {		
  int Pixels[N][N];

  loadImage(Pixels, "testPhantom.txt") ;
  Display image(Pixels, "testPhantom") ;

  loadImage(Pixels, "shapesPhantom.txt");
  Display image01(Pixels,"Phantom_01");

  loadImage(Pixels, "headPhantom.txt");
  Display image02(Pixels,"Phantom_02");

  return 0;
}

void loadImage(int Gray[][N], string fileName) {
    ifstream file;
    file.open(fileName);
    if (!file) cout << "File does not exist!";
    for (int row = 0; row < N; row++)
        for (int col = 0; col < N; col++)
            file >> Gray[row][col];
    file.close();
}
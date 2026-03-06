#include <iostream>
#include <fstream>
#include "Display.h"
using namespace std;

const int N = 400;   // size of the data in the text file (400 x 400)

int main() {
    short Pixels[SIZE][SIZE] = {0};  // SIZE comes from Display.h (500)

    ifstream inFile("reconstructed_phantom_400x400.txt");
    if (!inFile) {
        cout << "Error opening file: phantom_400x400.txt" << endl;
        return 1;
    }

    // Read 400x400 values into the top-left corner of the 500x500 array
    for (int row = 0; row < N; row++) {
        for (int col = 0; col < N; col++) {
            int temp;
            inFile >> temp;
            Pixels[row][col] = static_cast<short>(temp);
        }
    }

    inFile.close();

    // Now display using your Display class
    Display output(Pixels, "loaded_reconstructed_phantom_400x400");

    return 0;
}

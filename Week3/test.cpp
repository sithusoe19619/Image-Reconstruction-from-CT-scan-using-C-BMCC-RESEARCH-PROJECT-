#include <iostream>
#include <cmath>
#include "Display.h"
using namespace std;

const int N = 180; //Image Size N x N

int main() 
{		
    short Pixels[N][N] ;

        for (int row = 0 ; row < N ; row++)
        {
            for (int col = 0; col < N; col++)
            {
                Pixels[row][col] = 0 ;
            }
        }

    int sq_x         = 110 ;           // x-coordinate
    int sq_y         = 110 ;         // y-coordinate
    int sq_size      = 41 ;         // width=length=150pixels

        for (int y = sq_y ; y < sq_y + sq_size ; y++) // 300,301,302,...,449
        {
            for (int x = sq_x ; x < sq_x + sq_size ; x++) // 30,31,32,...,179
            {
            Pixels [y][x] = 499 ; // [row][col] = [y][x] = [300][30],[300][31],...,[449][179]
            }                       // 0-->255=black-->white
        }

    int disk_x       = 45 ;          // x-coordinate of the center
    int disk_y       = 45 ;         // y-coordinate of the center
    int R            = 20 ;          // radius
    int R2           = 0 ;
    
        R2      = R * R ;            // radius squared

        for (int y = disk_y - R ; y <= disk_y + R ; y++)
        {
            for (int x= disk_x - R ; x <= disk_x + R ; x++)
            {
                int distance_x = x - disk_x ;    // horizontal distance from center
                int distance_y = y - disk_y ;    // vertical distance from center
            
                if ((distance_x * distance_x) + (distance_y * distance_y) <= R2)
                {
                    Pixels[y][x] = 255 ; // 0-->255=black-->white
                }   
            }
        }

    int tri_x      = 128 ;              // center x (near top-right; stays inside 180×180)
    int tri_y_top  = 25 ;               // y of the apex (top point)
    int tri_height = 20 ;               // match square size
    int tri_base   = 41 ;               // match square size
    int tri_y_btm  = 0 ;

        tri_y_btm  = tri_y_top + tri_height ;

        for (int y = tri_y_top; y <= tri_y_btm; y++)   // from top to bottom
        {
            int dy = y - tri_y_top;                    // how far down we are 

            // Triangle grows wider as we go down
            int half_width = (tri_base * dy) / tri_height / 2; 
            
            // left and right edges for this row
            int x_left  = tri_x - half_width;
            int x_right = tri_x + half_width;

            // fill pixels between left and right edges
            for (int x = x_left; x <= x_right; x++)
            {
                if (y >= 0 && y < N && x >= 0 && x < N)
                    Pixels[y][x] = 255;  // white
            }
        }

        Display image(Pixels, "output") ;
        
    double angle = 60.0;           // degrees: try 30, 60, 90, etc.
    short Rot[N][N];               // new canvas for the rotated image

        // 1) clear the destination
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                Rot[r][c] = 0;

        // 2) rotation setup (rotate around image center)
        int cx = N / 2;                // center x (column)
        int cy = N / 2;                // center y (row)
        double rad = angle * 3.14159265358979323846 / 180.0;
        double cs  = cos(rad);
        double sn  = sin(rad);

        // 3) for each pixel in the output, find where it came from in the source
        for (int y2 = 0; y2 < N; y2++)
        {
            for (int x2 = 0; x2 < N; x2++)
            {
                // move the point so the center is at (0,0)
                double xr = x2 - cx;
                double yr = y2 - cy;

                // inverse rotation to sample from the original image
                double xs =  cs * xr - sn * yr + cx;
                double ys =  sn * xr + cs * yr + cy;

                // nearest-neighbor: round to the closest pixel
                int x = (int)round(xs) ;
                int y = (int)round(ys) ;

                // copy the pixel if it’s inside the image
                if (x >= 0 && x < N && y >= 0 && y < N)
                    Rot[y2][x2] = Pixels[y][x];
            }
        }

        // 4) save the rotated image
        Display image(Rot, "output_rot");   // writes output_rot.html

return 0;
}
#include <iostream>
#include "Display.h"

using namespace std ;

const int N = 180 ; 

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

int sq_x         = 75 ;           // x-coordinate
int sq_y         = 275 ;         // y-coordinate
int sq_size      = 100 ;         // width=length=150pixels

int disk_x       = 354 ;         // x-coordinate of the center
int disk_y       = 145 ;         // y-coordinate of the center
int R            = 50 ;          // radius

int R2      = R * R ;            // radius squared

    for (int y = sq_y ; y < sq_y + sq_size ; y++) // 300,301,302,...,449
    {
        for (int x = sq_x ; x < sq_x + sq_size ; x++) // 30,31,32,...,179
        {
            Pixels [y][x] = 499 ; // [row][col] = [y][x] = [300][30],[300][31],...,[449][179]
        }                       // 0-->255=black-->white

    }

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
    Display image(Pixels, "output7") ;

return 0 ;
}
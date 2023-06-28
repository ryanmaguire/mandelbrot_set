/******************************************************************************
 *                                  LICENSE                                   *
 ******************************************************************************
 *  This file is part of mandelbrot_set.                                      *
 *                                                                            *
 *  mandelbrot_set is free software: you can redistribute it and/or modify it *
 *  under the terms of the GNU General Public License as published by         *
 *  the Free Software Foundation, either version 3 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  mandelbrot_set is distributed in the hope that it will be useful,         *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with mandelbrot_set.  If not, see <https://www.gnu.org/licenses/>.  *
 ******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void color(int red, int green, int blue, FILE *fp)
{
    fputc((char)red,   fp);
    fputc((char)green, fp);
    fputc((char)blue,  fp);
}

int main(int argc, char *argv[])
{
    /*  Declare a variable for the output file and give it write permission.  */
    FILE *fp;
    fp = fopen("mandelbrot_set_001.ppm", "w");
    int size = 1024;
    int brightness, x, y, i;
    double pr, pi;
    double newRe, newIm, oldRe, oldIm;
    double zoom = 0.65;
    double moveX = -1.0;
    double moveY = 0.0;
    double radius = 4.0;
    int maxIterations = 256;

    fprintf(fp, "P6\n%d %d\n255\n", size, size);

    double radius_squared = radius*radius;

    //  Loop through each pixel.
    for(y = 0; y < size; y++){
        for(x = 0; x < size; x++){

            // Calculate the location of the current point being calculated.
            pr = (x - size / 2) / (0.5 * zoom * size) + moveX;
            pi = (y - size / 2) / (0.5 * zoom * size) + moveY;

            // Reset starting Real and Imaginary parts to zero.
            newRe = newIm = 0;

            //  Start the iteration process.
            for(i = 0; i < maxIterations; i++)
            {

                /*  Remember value of previous iteration.                     */
                oldRe = newRe;
                oldIm = newIm;

                /*  Calculate real and imaginary parts.                       */
                newRe = oldRe * oldRe - oldIm * oldIm + pr;
                newIm = 2 * oldRe * oldIm + pi;

                //  Check for divergence.
                if((newRe * newRe + newIm * newIm) > radius_squared)
                    break;
            }

            if(i == maxIterations)
                color(0, 0, 0, fp);
            else if (i < 64)
            {
                brightness = 4*i;
                color(brightness, brightness, 255-brightness, fp);
            }
            else
                color(255, 255, 0, fp);
        }
    }
    return 0;
}

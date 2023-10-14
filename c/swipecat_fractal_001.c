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

/*  FILE, fopen, fputc, puts, and other input / output functions found here.  */
#include <stdio.h>

/*  exp and trig functions provided here.                                     */
#include <math.h>

/*  Some implementations of libm provide pi / 2, some don't. Declare this as  *
 *  a macro for improved portability.                                         */
#define PI_BY_TWO (+1.5707963267948966)

/*  Function for drawing a modified Mandelbrot set.                           */
int main(void)
{
    /*  The number of pixels in the x and y axes, respectively.               */
    const unsigned int width = 1024U;
    const unsigned int height = 1024U;

    /*  The maximum number of iterations allowed.                             */
    const unsigned int max_iters = 100U;

    /*  Threshold for the coloring scheme.                                    */
    const double zmax = 150.0;

    /*  Setup parameters for the image. These are the bounds of the PPM.      */
    const double x_min = -6.6;
    const double x_max = -0.4;
    const double y_min = -3.5;
    const double y_max = +3.5;

    /*  Scale factors for converting from pixels to points.                   */
    const double x_factor = (x_max - x_min) / (double)(width - 1U);
    const double y_factor = (y_max - y_min) / (double)(height - 1U);

    /*  Variables for looping over the x and y components of the picture.     */
    unsigned int x, y;

    /*  Index for keeping track of the number of iterations performed.        */
    unsigned int iters;

    /*  Variables for the color in RGB format.                                */
    unsigned char red, green, blue;

    /*  Dummy variable for creating the color scheme for the drawing.         */
    double val;

    /*  Declare a variable for the output file and give it write permission.  */
    FILE * const fp = fopen("swipecat_fractal_001.ppm", "w");

    /*  fopen returns NULL on failure. Check for this.                        */
    if (!fp)
    {
        puts("fopen return NULL. Aborting.");
        return -1;
    }

    /*  Otherwise print the preamble to the PPM file.                         */
    fprintf(fp, "P6\n%u %u\n255\n", width, height);

    /*  Loop through each pixel for the y-axis.                               */
    for (y = 0U; y < height; ++y)
    {
        /*  Calculate the imaginary part corresponding to these pixels.       */
        const double c_y = y_max - (double)y * y_factor;

        /*  Loop through each pixel in the x-axis.                            */
        for (x = 0U; x < width; ++x)
        {
            /*  Calculate the real part corresponding to this pixel.          */
            const double c_x = x_min + (double)x * x_factor;

            /*  Initialize the complex number to the origin.                  */
            double xn = 0.0;
            double yn = 0.0;

            /*  Factor used for creating a gradient in color.                 */
            double background = 0.0;

            /*  Start the iteration process. Stop when the iteration diverges *
             *  outside of the circle, or when too many iterations are done.  */
            for (iters = 0U; iters < max_iters; ++iters)
            {
                /*  Modified Mandelbrot iterate. Defined by                   *
                 *  z_{n+1} = (pi/2)(exp(z_{n}) - z_{n}) + z_{0}.             */
                const double exp_x = exp(xn);
                xn = PI_BY_TWO*(exp_x*cos(yn) - xn) + c_x;
                yn = PI_BY_TWO*(exp_x*sin(yn) - yn) + c_y;

                /*  Once the iterate gets too big, abort the computation.     */
                if (fabs(xn) >= zmax)
                {
                    /*  Gradient factor for coloring the image.               */
                    background = log(log(fabs(xn) + 1.0) * 0.3333333333333);
                    background = log(fabs((double)iters - background));
                    background = background * 0.3076923076923077;
                    break;
                }
            }

            /*  Factor used for coloring.                                     */
            val = 1.0 - fabs(1.0 - background);

            /*  Non-positive corresponds to the modified Mandelbrot set.      */
            if (val <= 0.0)
            {
                red = 0x00U;
                green = 0x00U;
                blue = 0x00U;
            }

            else if (background <= 1.0)
            {
                red = (unsigned char)(255.0 * pow(val, 4.0));
                green = (unsigned char)(255.0 * pow(val, 2.5));
                blue = (unsigned char)(unsigned char)(255.0 * val);
            }

            else
            {
                red = (unsigned char)(255.0 * val);
                green = (unsigned char)(255.0 * pow(val, 1.5));
                blue = (unsigned char)(255.0 * pow(val, 3.0));
            }

            /*  Add this current color to the image.                          */
            fputc(red, fp);
            fputc(green, fp);
            fputc(blue, fp);
        }
    }

    /*  Close the file and return.                                            */
    fclose(fp);
    return 0;
}
/*  End of main.                                                              */

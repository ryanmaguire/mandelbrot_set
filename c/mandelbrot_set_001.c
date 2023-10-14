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
 ******************************************************************************
 *  Purpose:                                                                  *
 *      Draw the Mandelbrot set.                                              *
 ******************************************************************************
 *  Author: Ryan Maguire                                                      *
 *  Date:   June 2, 2021                                                      *
 ******************************************************************************/

/*  FILE, fopen, fputc, puts, and other input / output functions found here.  */
#include <stdio.h>

/******************************************************************************
 *  Function:                                                                 *
 *      color                                                                 *
 *  Purpose:                                                                  *
 *      Writes an RGB color to a PPM file.                                    *
 *  Arguments:                                                                *
 *      red (unsigned char):                                                  *
 *          The value of the red channel. 8-bits.                             *
 *      green (unsigned char):                                                *
 *          The value of the green channel. 8-bits.                           *
 *      blue (unsigned char):                                                 *
 *          The value of the blue channel. 8-bits.                            *
 *      fp (FILE *):                                                          *
 *          A pointer to the PPM file.                                        *
 *  Output:                                                                   *
 *      None (void).                                                          *
 ******************************************************************************/
static void
color(unsigned char red, unsigned char green, unsigned char blue, FILE *fp)
{
    fputc(red, fp);
    fputc(green, fp);
    fputc(blue, fp);
}

/******************************************************************************
 *  Function:                                                                 *
 *      complex_abs_squared                                                   *
 *  Purpose:                                                                  *
 *      Computes the square of the complex modulus, f(z) = |z|^2.             *
 *  Arguments:                                                                *
 *      re (double):                                                          *
 *          The real part of the complex number.                              *
 *      im (double):                                                          *
 *          The imaginary part of the complex number.                         *
 *  Output:                                                                   *
 *      abs_sq (double):                                                      *
 *          The square of the absolute value of z = re + i*im.                *
 ******************************************************************************/
static double
complex_abs_squared(double re, double im)
{
    return re*re + im*im;
}

/******************************************************************************
 *  Function:                                                                 *
 *      mandelbrot_iter                                                       *
 *  Purpose:                                                                  *
 *      Computes the Mandelbrot iteration, z_{n+1} = z_{n}^2 + z_{0}.         *
 *  Arguments:                                                                *
 *      xn (double *):                                                        *
 *          The real part of z_{n}.                                           *
 *      yn (double *):                                                        *
 *          The imaginary part of z_{n}.                                      *
 *      x0 (const double *):                                                  *
 *          The real part of z_{0}.                                           *
 *      y0 (const double *):                                                  *
 *          The imaginary part of z_{0}.                                      *
 *  Output:                                                                   *
 *      None (void).                                                          *
 ******************************************************************************/
static void
mandelbrot_iter(double *xn, double *yn, const double *x0, const double *y0)
{
    /*  Avoiding overwriting the real part of the complex number.             */
    const double tmp = *xn;

    /*  Calculate the new value. z_{n+1} = z_{n}^2 + z_{0}.                   */
    *xn = *xn * *xn - *yn * *yn + *x0;
    *yn = 2.0 * tmp * *yn + *y0;
}

/*  Function for drawing the Mandelbrot set.                                  */
int main(void)
{
    /*  The number of pixels in both the x and y axes. The PPM is a square.   */
    const unsigned int size = 1024U;

    /*  Half the above constant, used for the scale factor.                   */
    const unsigned int half_size = size >> 1U;

    /*  Variables for looping over the x and y coordinates in the plane.      */
    unsigned int x, y;

    /*  Index for keeping track of the number of iterations performed.        */
    unsigned char iters;

    /*  "Zoom" factor for scaling the image so the Mandelbrot set fits better.*/
    const double zoom = 0.65;

    /*  Scale factor for converting from pixels (xpx, ypx) to points (x, y).  */
    const double scale_factor = 2.0 / (zoom * size);

    /*  The starting parameters for the x and y values. z0 = x0 + i*y0.       */
    const double x_start = -0.8;
    const double y_start = +0.0;

    /*  Translations used for converting between pixels and points.           */
    const double x_shift = x_start - scale_factor * (double)half_size;
    const double y_shift = y_start - scale_factor * (double)half_size;

    /*  The radius of the circle. Points outside of this diverge.             */
    const double radius = 4.0;
    const double radius_squared = radius*radius;

    /*  Maximum number of iterations allowed in the computation.              */
    const unsigned char max_iters = 0xFFU;

    /*  Color factors to brighten the region around the Mandelbrot set.       */
    const unsigned char threshold = 0x40U;
    const unsigned char color_scale = 0x04U;

    /*  Declare a variable for the output file and give it write permission.  */
    FILE * const fp = fopen("mandelbrot_set_001.ppm", "w");

    /*  fopen returns NULL on failure. Check for this.                        */
    if (!fp)
    {
        puts("fopen returned NULL. Aborting.");
        return -1;
    }

    /*  Otherwise print the preamble to the PPM file.                         */
    fprintf(fp, "P6\n%u %u\n255\n", size, size);

    /*  Loop through each pixel for the y-axis.                               */
    for(y = 0U; y < size; y++)
    {
        /*  And each pixel in the x-axis.                                     */
        for(x = 0U; x < size; x++)
        {
            /*  Calculate the location of the current point in the plane.     */
            const double x0 = x * scale_factor + x_shift;
            const double y0 = y * scale_factor + y_shift;

            /*  Initialize the complex number to x0 + i*y0.                   */
            double z_re = x0;
            double z_im = y0;

            /*  Start the iteration process. Stop when the iteration diverges *
             *  outside of the circle, or when too many iterations are done.  */
            for (iters = 0x00U; iters < max_iters; ++iters)
            {
                /*  Calculate the next iteration.                             */
                mandelbrot_iter(&z_re, &z_im, &x0, &y0);

                /*  Once the iteration falls outside the circle, abort.       */
                if (complex_abs_squared(z_re, z_im) > radius_squared)
                    break;
            }

            /*  Points that don't diverge are the Mandelbrot set. Color black.*/
            if (iters == max_iters)
                color(0x00U, 0x00U, 0x00U, fp);

            /*  Points that diverged very quickly. Blue-to-Yellow gradient.   */
            else if (iters < threshold)
            {
                const unsigned char brightness = iters * color_scale;
                color(brightness, brightness, 0xFFU - brightness, fp);
            }

            /*  Points that took a long time to diverge, color yellow.        */
            else
                color(0xFF, 0xFF, 0x00, fp);
        }
    }

    /*  Close the file and return.                                            */
    fclose(fp);
    return 0;
}
/*  End of main.                                                              */

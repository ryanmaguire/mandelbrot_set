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
 *  Author:     Ryan Maguire                                                  *
 *  Date:       June 2, 2021                                                  *
 ******************************************************************************/

/*  Avoid warnings with MSVC for using standard library functions.            */
#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)
#define _CRT_SECURE_NO_DEPRECATE
#endif

/*  FILE data type, fopen, and fputc are found here.                          */
#include <stdio.h>

static void
color(unsigned char red, unsigned char green, unsigned char blue, FILE *fp)
{
    fputc(red, fp);
    fputc(green, fp);
    fputc(blue, fp);
}

int main(void)
{
    /*  Declare a variable for the output file and give it write permission.  */
    const unsigned int width = 1024U;
    const unsigned int height = 1024U;
    double new_re, new_im, old_re, old_im, c_x, c_y, abs_z_sq;
    const double x_min = -3.0;
    const double x_max = 1.0;
    const double y_min = -2.0;
    const double y_max = 2.0;
    const double x_factor = (x_max - x_min) / (double)(width - 1U);
    const double y_factor = (y_max - y_min) / (double)(height - 1U);
    unsigned int x, y;
    const unsigned char MaxIters = 0xFFU;
    unsigned char iters, brightness;

    const double radius = 4.0;
    const double radius_squared = radius*radius;

    FILE *fp = fopen("mandelbrot_set_002.ppm", "w");

    if (!fp)
    {
        puts("fopen failed and returned NULL. Aborting.");
        return -1;
    }

    fprintf(fp, "P6\n%d %d\n255\n", width, height);

    for(y = 0U; y < height; ++y)
    {
        c_y = y_max - (double)y * y_factor;
        for(x = 0U; x < width; ++x)
        {
            c_x = x_min + (double)x * x_factor;
            new_re = 0.0;
            new_im = 0.0;
            abs_z_sq = 0.0;
            iters = 0x00U;

            while ((abs_z_sq < radius_squared) && (iters < MaxIters))
            {
                old_re = new_re;
                old_im = new_im;

                new_re = old_re*old_re - old_im*old_im + c_x;
                new_im = 2.0*old_re*old_im + c_y;
                abs_z_sq = new_re*new_re + new_im*new_re;
                ++iters;
            }

            if (iters == MaxIters)
                color(0x00U, 0x00U, 0x00U, fp);
            else if (iters < 64)
            {
                brightness = 4U*iters;
                color(brightness, brightness, 255-brightness, fp);
            }
            else
                color(0xFFU, 0xFFU, 0x00U, fp);
        }
    }
    return 0;
}

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
#include <math.h>
#include <stdlib.h>
#include "gif.h"

#define PI_BY_TWO 1.5707963267948966

struct complex_number {
    double real, imag;
};

struct complex_number complex_pow(struct complex_number z, double exponent)
{
    double abs_z = sqrt(z.real*z.real + z.imag*z.imag);
    double arg = atan2(z.imag, z.real);
    double x = exponent * log(abs_z);
    double y = exponent * arg;

    double exp_val = exp(x);
    double real_part = exp_val * cos(y);
    double imag_part = exp_val * sin(y);
    struct complex_number out;
    out.real = real_part;
    out.imag = imag_part;
    return out;
}

static struct complex_number
mandel_func(struct complex_number z, struct complex_number c, double r)
{
    struct complex_number out = complex_pow(z, r);
    out.real += c.real;
    out.imag += c.imag;
    return out;
}

int main(void)
{
    const unsigned int imax = 255U;
    const double zmax = 4.0;
    const double center_x = 0.0;
    const double center_y = 0.0;
    const double ds = 2.0;
    const unsigned int width = 512U;
    const unsigned int height = 512U;
    const unsigned int nframes = 500U;
    uint8_t image[width*height*4];

    unsigned int x, y, iters, frame;
    unsigned char red, green, blue;
    double c_x, c_y, backgnd, val, exp_x;
    struct complex_number z, c;
    double r = 1.0;
    double dr = 10.0 / (double)nframes;

    const char* filename = "mandelbrot_set_gif_002.gif";
    GifWriter writer;
    GifBegin(&writer, filename, width, height, 2, 8, true);

    for (frame = 0; frame < nframes; ++frame)
    {
        double x_min = center_x - ds;
        double x_max = center_x + ds;
        double y_min = center_y - ds;
        double y_max = center_y + ds;

        double x_factor = (x_max - x_min) / ((double)width - 1.0);
        double y_factor = (y_max - y_min) / ((double)height - 1.0);
        for (y = 0U; y < height; ++y)
        {
            c_y = y_max - (double)y * y_factor;
            for (x = 0U; x < width; ++x)
            {
                c_x = x_min + (double)x * x_factor;
                c.real = c_x;
                c.imag = c_y;
                z.real = 0.0;
                z.imag = 0.0;
                backgnd = 0.0;
                for (iters = 0U; iters < imax; ++iters)
                {
                    z = mandel_func(z, c, r);
                    if (fabs(z.real) >= zmax)
                    {
                        backgnd = log(log(fabs(z.real) + 1.0) * 0.33333333333);
                        backgnd = log(fabs((double)iters - backgnd));
                        backgnd = backgnd * 0.3076923076923077;
                        break;
                    }
                }

                val = 1.0 - fabs(1.0 - backgnd);
                if (val < 0.0)
                    val = 0.0;

                if (backgnd <= 1.0)
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
                uint8_t* pixel = &image[(y*width + x)*4];
                pixel[0] = red;
                pixel[1] = green;
                pixel[2] = blue;
                pixel[3] = 255;
            }
        }

        r += dr;
        printf( "Writing frame %d...\n", frame);
        GifWriteFrame(&writer, image, width, height, 2, 8, true);
    }
    GifEnd(&writer);
    return 0;
}

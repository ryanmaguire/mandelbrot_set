""""
################################################################################
#                                  LICENSE                                     #
################################################################################
#   This file is part of mandelbrot_set.                                       #
#                                                                              #
#   mandelbrot_set is free software: you can redistribute it and/or modify it  #
#   under the terms of the GNU General Public License as published by          #
#   the Free Software Foundation, either version 3 of the License, or          #
#   (at your option) any later version.                                        #
#                                                                              #
#   mandelbrot_set is distributed in the hope that it will be useful,          #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#   GNU General Public License for more details.                               #
#                                                                              #
#   You should have received a copy of the GNU General Public License          #
#   along with mandelbrot_set.  If not, see <https://www.gnu.org/licenses/>.   #
################################################################################
#   Purpose:                                                                   #
#       Draws a variant of the Mandelbrot set.                                 #
################################################################################
#   Author: Ryan Maguire                                                       #
#   Date:   June 1, 2021.                                                      #
################################################################################
"""
import math
import cmath

WIDTH = 2048
HEIGHT = 2048
IMAX = 100
ZMAX = 150
X_FACTOR = 2.0 / (WIDTH - 1.0)
Y_FACTOR = 2.0 / (HEIGHT - 1.0)

FILE_POINTER = open("swipecat_fractal.ppm", "w")
FILE_POINTER.write("P3\n%d %d\n255\n" % (WIDTH, HEIGHT))

for y_val in range(HEIGHT):
    z_y = 1.0 - Y_FACTOR*y_val

    for X_VAL in range(WIDTH):
        z_x = X_FACTOR*X_VAL - 1.0
        C_VAL = complex(z_x, z_y)

        if abs(C_VAL) >= 1.0:
            FILE_POINTER.write("%u %u %u\n" % (255, 255, 255))
            continue

        C_VAL = C_VAL / (1.0 - (C_VAL.real*C_VAL.real + C_VAL.imag*C_VAL.imag))
        Z_VAL = 0.0
        BACKGROUND = 0

        for its in range(IMAX):
            Z_VAL = 0.5*math.pi * (cmath.exp(Z_VAL) - Z_VAL) + C_VAL

            if abs(Z_VAL.real) >= ZMAX:
                BACKGROUND = math.log(math.log(abs(Z_VAL.real) + 1.0) / 3.0)
                BACKGROUND = math.log(abs(its - BACKGROUND)) / 3.25
                break

        VAL = max(0.0, 1.0 - abs(1.0 - BACKGROUND))

        if BACKGROUND <= 1.0:
            RED = int(255.0 * VAL**4)
            GREEN = int(255.0 * VAL**2.5)
            BLUE = int(255.0 * VAL)
        else:
            RED = int(255.0 * VAL)
            GREEN = int(255.0 * VAL**1.5)
            BLUE = int(255.0 * VAL**3)

        FILE_POINTER.write("%u %u %u\n" % (RED, GREEN, BLUE))
FILE_POINTER.close()

## Notes on OpenGL Tile Overscan

Description of issue:  Vertical or Horizontal 1 pixel lines that show up as
artifacts while scrolling the tile map.  The lines appear at tile boundearies
at certain scrolling offsets.  These lines are due to sampling the next tile
over in the tile atlas.  The following are my notes from troubleshooting the
problem and trying to understand it as best as possible.

Window is 800x600 in code but really 1600x1200 with the Retina display.

Camera is set to display 8 tiles by 6 tiles

Each screen pixel is represented by 0.005 tiles
(because 8.0 [tiles] / 1600.0 [screen pixels] = 0.005 [tiles / screen pixel])

Each tile in the texture is 16x16 pixels

Since 1600 screen pixels represent 8 tiles there are 200 screen pixels per tile
(because 1600 [screen pixels] / 8 [tiles] = 200)

The number of screen pixels per texture pixel is 12.5
(Because 200 [screen pixels / tile] / 16 [texture pixels / tile])

The number of tiles per texture pixel is 0.0625
(Because 12.5 [screen pixels / texture pixel] * 0.005 [tiles / screen pixel] = 0.0625 [tiles / texture pixel]

Moving the camera a non-integer number of screen pixels seems to cause the problem.  If I step it by half a screen pixel it only sometimes has an issue

Camera center of (0.0275, 0.0275) has vertical line issues.
Camera center of (0.0375, 0.0375) has partial horizontal line issues.

Starting at 0.0275 and adding 0.0625 (number of tiles per texture tile) to the X coordinate of the camera center the first 3 positions that show vertical lines are:

```
0.0275
0.4025
2.4025
4.0275
4.4025
6.4025
8.4025

Didn’t see any past these even at 16.4025
```

Starting at 0.0624 and advancing by 0.0625 I see it every other step.  Not sure why it isn’t every step..

Starting at 0.02620 and stepping by 0.00001 the vertical lines are visible starting at 0.06238 through 0.06249

For Y axis it occurs starting at 0.06252 through 0.06262.  This may be because of the inversion of the y axis in the vertex shader.

When starting at 2^(-4) - 15 * 2^(-13) and stepping by 2^(-13) the lines show up at 0.0623779 when it gets close to a rollover in the binary representation

```
cameraCenter = 0.0620117, 0
 00111101 01111110 00000000 00000000
cameraCenter = 0.0621338, 0
 00111101 01111110 10000000 00000000
cameraCenter = 0.0622559, 0
 00111101 01111111 00000000 00000000
cameraCenter = 0.0623779, 0 <----- happens here
 00111101 01111111 10000000 00000000
cameraCenter = 0.0625, 0
 00111101 10000000 00000000 00000000
cameraCenter = 0.0626221, 0
 00111101 10000000 01000000 00000000
cameraCenter = 0.0627441, 0
 00111101 10000000 10000000 00000000
```

When starting at 0.06230 and stepping by 0.00001 It also starts happening when the first 8 bits of the mantissa are all 1s.  The lines go away just before the bits roll over but many are 1s at that point.

```
cameraCenter = 0.06231, 0
 00111101 01111111 00111000 11000101
cameraCenter = 0.06232, 0
 00111101 01111111 01000011 01000001
cameraCenter = 0.06233, 0
 00111101 01111111 01001101 10111101
cameraCenter = 0.06234, 0
 00111101 01111111 01011000 00111001
cameraCenter = 0.06235, 0
 00111101 01111111 01100010 10110101
cameraCenter = 0.06236, 0
 00111101 01111111 01101101 00110001
cameraCenter = 0.06237, 0
 00111101 01111111 01110111 10101101
cameraCenter = 0.06238, 0 <----- lines start here
 00111101 01111111 10000010 00101001
cameraCenter = 0.06239, 0
 00111101 01111111 10001100 10100101
cameraCenter = 0.0624, 0
 00111101 01111111 10010111 00100001
cameraCenter = 0.06241, 0
 00111101 01111111 10100001 10011101
cameraCenter = 0.06242, 0
 00111101 01111111 10101100 00011001
cameraCenter = 0.06243, 0
 00111101 01111111 10110110 10010101
cameraCenter = 0.06244, 0
 00111101 01111111 11000001 00010001
cameraCenter = 0.06245, 0
 00111101 01111111 11001011 10001101
cameraCenter = 0.06246, 0
 00111101 01111111 11010110 00001001
cameraCenter = 0.06247, 0
 00111101 01111111 11100000 10000101
cameraCenter = 0.06248, 0
 00111101 01111111 11101011 00000001
cameraCenter = 0.06249, 0 <----- lines last visible here
 00111101 01111111 11110101 01111101
cameraCenter = 0.0625, 0
 00111101 01111111 11111111 11111001
cameraCenter = 0.06251, 0
 00111101 10000000 00000101 00111011
cameraCenter = 0.06252, 0
 00111101 10000000 00001010 01111001
cameraCenter = 0.06253, 0
 00111101 10000000 00001111 10110111
cameraCenter = 0.06254, 0
 00111101 10000000 00010100 11110101
```

Stepping back from 0.0625 (2^-4) by (2^-20) the lines start at 0.0624895

```
cameraCenter = 0.0624905, 0
 00111101 01111111 11110110 00000000
cameraCenter = 0.0624895, 0. <---- lines start here
 00111101 01111111 11110101 00000000
cameraCenter = 0.0624886, 0
 00111101 01111111 11110100 00000000
```

If we use a 64x64 tile size instead of 16x16

The number of screen pixels per texture pixel is 3.125
(Because 200 [screen pixels / tile] / 64 [texture pixels / tile])

The number of tiles per texture pixel is 0.0625
(Because 3.125 [screen pixels / texture pixel] * 0.005 [tiles / screen pixel] = 0.015625 [tiles / texture pixel]

With 64x64 tile size starting at 2^(-6) and stepping back by 2^(-21) the lines start at 0.0156221 ( 2^(-6) - 6 * 2^(-21) ) but only if the field of view is set to 2 tiles wide by 1.5 tiles tall

```
cameraCenter = 0.0156226, 0
 00111100 01111111 11110110 00000000
cameraCenter = 0.0156221, 0 <----- lines start here
 00111100 01111111 11110100 00000000
cameraCenter = 0.0156217, 0
 00111100 01111111 11110010 00000000
```

Note that the code used to generated the binary output for the floats was as follows.  Note that this code puts spaces on byte boundaries but really the fields in a IEEE-754 32-bit float are:
- 1 bit sign
- 8 bits for exponent
- 23 bits for mantissa

See:
- https://en.wikipedia.org/wiki/Single-precision_floating-point_format
- https://www.astro.gla.ac.uk/users/norman/star/sc13/sc13.htx/N-a2b3c2.html

```
void print_raw_float_binary(float f)
{
 uint32_t *float_as_int = (uint32_t *)&f;
 int i;

 for (i=0; i<=31; i++)
   {
    if (i%8 == 0)
      printf(" "); // Space

    if ((*float_as_int >> (31-i)) & 1)
      printf("1");
    else
      printf("0");
   }
 printf("\n");
}
```
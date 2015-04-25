#ifndef __IMAGE_H_
#define __IMAGE_H_

#include <cstdio>
#include <cstdlib>
#include "../core/color.h"

class Screen {
public:
        Screen(int _w, int _h);
       ~Screen();

    // Writes out the buffer to disk, as a TGA file.
    bool writeImage(const char *file);

    int getWidth()
    { return width; }
    int getHeight()
    { return height; }

    void setPixel(int x, int y, const Color &color)
    {
        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            printf("pixel (%d, %d) out of range...\n", x, y);
            return;
        }
        data[x+y*width] = color;
    }

private:
    int width;
    int height;
    Color *data;
};

#endif

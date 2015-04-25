#include "screen.h"
#include <fstream>
#include <iostream>

Screen::Screen(int _w, int _h)
{
    width = _w;
    height = _h;
    data = (Color *)malloc(sizeof(Color) * width * height);
}

Screen::~Screen()
{
    free(data);
    data = 0;
}

bool Screen::writeImage(const char *file)
{
    std::ofstream o(file, std::ios::out | std::ios::binary);

    // Write the header.
    o.put(0);
    o.put(0);

    // Uncompressed RGB format.
    o.put(2);
    o.put(0);   o.put(0);
    o.put(0);   o.put(0);
    o.put(0);
    o.put(0);   o.put(0);
    o.put(0);   o.put(0);
    o.put((width & 0x00FF));
    o.put((width & 0xFF00) / 256);
    o.put((height & 0x00FF));
    o.put((height & 0xFF00) / 256);
    o.put(32);
    o.put(0);

    for (int i = 0; i < width*height; i++) {
        o.put((unsigned char)(data[i].b * 255));
        o.put((unsigned char)(data[i].g * 255));
        o.put((unsigned char)(data[i].r * 255));
        o.put((unsigned char)(data[i].a * 255));
    }

    o.close();
    return true;
}

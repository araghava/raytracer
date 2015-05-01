#ifndef __UTIL_H_
#define __UTIL_H_

float UTILrandomFloatBetween(float min, float max)
{
    float r = ((float) rand()) / (float) RAND_MAX;
    return (r * (max - min)) + min;
}

#endif

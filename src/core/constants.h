#ifndef __CONSTANTS_H
#define __CONSTANTS_H

extern const float TOLERANCE;

// To avoid ray intersections that occur at the exact same spot repeatedly,
// we add a little bias at the ray origin.
extern const float RAY_INTERSECTION_BIAS;

extern const float RAY_NEAR_DISTANCE;
extern const float RAY_FAR_DISTANCE;

extern const float RAY_CAST_ATTENUATION;

extern const int RAY_RECURSION_LIMIT;

#endif

#ifndef __CORE_H
#define __CORE_H

extern const float TOLERANCE;

// To avoid ray intersections that occur at the exact same spot repeatedly,
// we add a little bias at the ray origin.
extern const float RAY_INTERSECTION_BIAS;

extern const float RAY_NEAR_DISTANCE;
extern const float RAY_FAR_DISTANCE;
#endif

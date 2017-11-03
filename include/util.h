#ifndef UTIL_H
#define UTIL_H

#include "types.h"
#include "image.h"

class Util {
  public:
    static bool getIntersection(doubleRect*, doubleRect*, doubleRect*);
    static bool isIntersecting(doubleRect*, doubleRect*);
    static bool getIntersection(Image* a, Image* b, doubleRect* intersection);
    static bool isIntersecting(Image* a, Image* b);
};

#endif

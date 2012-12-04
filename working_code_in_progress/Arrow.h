#ifndef ARROW_H
#define ARROW_H

#include "Vector3.h"

class Arrow
{
private:
    Vector3 base;
    Vector3 end;

public:
    Arrow(Vector3 basePoint, Vector3 endPoint);
    void draw();
};

#endif
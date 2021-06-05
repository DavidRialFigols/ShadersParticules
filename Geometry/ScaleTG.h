#ifndef SCALE_H
#define SCALE_H

#include "TG.h"

class ScaleTG : public TG
{
public:
    vec3 scale;
    ScaleTG(vec3 vecScale);
    ScaleTG(float sc);
    ~ScaleTG();
};

#endif // SCALE_H

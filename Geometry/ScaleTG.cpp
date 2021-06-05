#include "Geometry/ScaleTG.h"

ScaleTG::ScaleTG(vec3 vecScale) : scale(vecScale)
{
    matTG = Scale(scale);
}

ScaleTG::ScaleTG(float sc) : scale(sc, sc, sc) {
    matTG = Scale(scale);
}

ScaleTG::~ScaleTG() {

}

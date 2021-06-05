#ifndef PLANE_H
#define PLANE_H

#include "Object.h"

class Plane: public Object {
public:
    Plane(vec3 normal, float d, float xmin, float xmax, float zmin, float zmax, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect);
    virtual ~Plane(){}

private:
    // Retorna la coordenada y corresponent al punt (x,y,z) del pla
    float getCoordY(vec3 normal, float d, float x, float z);
};

#endif // PLANE_H

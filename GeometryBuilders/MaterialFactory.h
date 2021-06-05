#ifndef MATERIALFACTORY_H
#define MATERIALFACTORY_H

#include "Geometry/Material.h"

class MaterialFactory
{
    MaterialFactory(){};
public:
    enum  MATERIAL_TYPES{
        COLOR_MAP,
        MATERIAL
    };

    static MaterialFactory& getInstance() {
        static MaterialFactory instance;
        return instance;
    }

    shared_ptr<Material> createMaterial(vec3 a, vec3 d, vec3 s, float shininess, MATERIAL_TYPES t);
};

#endif // MATERIALFACTORY_H

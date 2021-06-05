#include "MaterialFactory.h"

shared_ptr<Material> MaterialFactory::createMaterial(vec3 a, vec3 d, vec3 s, float shininess, MATERIAL_TYPES t) {
    shared_ptr<Material> m;
    switch (t) {
    case COLOR_MAP:
        m = make_shared<Material>(a, d, s, shininess);
        break;
    case MATERIAL:
        m = make_shared<Material>(a, d, s, shininess);
    default:
        break;
    }
    return m;
}

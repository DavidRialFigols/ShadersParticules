#pragma once

#include <library/vec.h>
#include "Light.h"
using namespace Common;

// Classe que representa els atributs d'una DirectionalLight

class DirectionalLight : public Light {
    private:
        vec4 direction;

    public:
        DirectionalLight(vec3 Ia, vec3 Id, vec3 Is, vec4 direction);

        vec4 getLightDirection();
        void setLightDirection(vec4 v);
};


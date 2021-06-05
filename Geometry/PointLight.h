#pragma once

#include <library/vec.h>
#include "Light.h"
using namespace Common;

// Classe que representa els atributs d'una PointLight

class PointLight : public Light{
    private:
        vec4 position;

    public:
        PointLight(vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients, vec4 position);

        vec4 getLightPosition();
        void setLightPosition(vec4 v);
};


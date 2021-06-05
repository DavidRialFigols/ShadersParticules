#pragma once

#include <library/vec.h>
#include "Light.h"
using namespace Common;

// Classe que representa els atributs d'una SpotLight

class SpotLight : public Light{
    private:
        vec4 position, direction;

        float angle;

    public:
        SpotLight(vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients, vec4 position, vec4 direction, float angle);

        vec4 getLightPosition();
        void setLightPosition(vec4 v);

        vec4 getLightDirection();
        void setLightDirection(vec4 v);

        float getAngle();
        void setAngle(float f);
};


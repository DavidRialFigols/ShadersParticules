#pragma once

#include <library/vec.h>
using namespace Common;

// Tipus de Lights
enum LightType {Puntual, Direccional, Spot};

// Classe que representa els atributs d'una Light

class Light {
    private:
        vec3 Ia, Id, Is;
        vec3 coeficients;

        LightType type;

    public:
        Light(LightType l, vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients);
        virtual ~Light() {};

        vec3 getIa() const;
        void setIa(const vec3 &value);

        vec3 getId();
        void setId(vec3 i);

        vec3 getIs() const;
        void setIs(const vec3 &value);

        vec3 getCoeficients() const;
        void setCoeficients(const vec3 &value);

        LightType getTipusLight() const;
        void setTipusLight(const LightType &value);
};


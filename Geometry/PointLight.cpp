#include "Geometry/PointLight.h"

/**
 * @brief PointLight::PointLight
 * @param t
 */
PointLight::PointLight(vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients, vec4 position): Light(Puntual, Ia, Id, Is, coeficients) {
    this->position = position;
}

/**
 * @brief PointLight::getLightPosition
 */
vec4 PointLight::getLightPosition() {
    return(this->position);
}

/**
 * @brief PointLight::setLightPosition
 * @param v
 */
void PointLight::setLightPosition(vec4 v) {
    this->position = v;
}

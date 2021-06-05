#include "Geometry/DirectionalLight.h"

/**
 * @brief DirectionalLight::DirectionalLight
 * @param t
 */
DirectionalLight::DirectionalLight(vec3 Ia, vec3 Id, vec3 Is, vec4 direction): Light(Direccional, Ia, Id, Is, vec3(0, 0, 0)) {
    this->direction = direction;
}


/**
 * @brief DirectionalLight::getLightDirection
 */
vec4 DirectionalLight::getLightDirection() {
    return(this->direction);
}

/**
 * @brief DirectionalLight::setLightDirection
 * @param v
 */
void DirectionalLight::setLightDirection(vec4 v) {
    this->direction = v;
}


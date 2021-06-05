#include "Geometry/SpotLight.h"

/**
 * @brief SpotLight::SpotLight
 * @param t
 */
SpotLight::SpotLight(vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients, vec4 position, vec4 direction, float angle): Light(Spot, Ia, Id, Is, coeficients) {
    this->position = position;
    this->direction = direction;
    this->angle = angle;
}

/**
 * @brief SpotLight::getLightPosition
 */
vec4 SpotLight::getLightPosition() {
    return(this->position);
}

/**
 * @brief SpotLight::setLightPosition
 * @param v
 */
void SpotLight::setLightPosition(vec4 v) {
    this->position = v;
}

/**
 * @brief SpotLight::getLightDirection
 */
vec4 SpotLight::getLightDirection() {
    return(this->direction);
}

/**
 * @brief SpotLight::setLightDirection
 * @param v
 */
void SpotLight::setLightDirection(vec4 v) {
    this->direction = v;
}


/**
 * @brief SpotLight::getAngle
 *
 */
float SpotLight::getAngle()
{
    return this->angle;
}

/**
 * @brief SpotLight::setAngle
 * @param f
 */
void SpotLight::setAngle(float f)
{
    this->angle = f;
}

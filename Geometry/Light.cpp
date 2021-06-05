#include "Geometry/Light.h"

/**
 * @brief Light::Light
 * @param t
 */
Light::Light(LightType t, vec3 Ia, vec3 Id, vec3 Is, vec3 coeficients) {
    this->type = t;
    this->Ia = Ia;
    this->Id = Id;
    this->Is = Is;
    this->coeficients = coeficients;
}

/**
 * @brief Light::getDiffuseIntensity
 * @return
 */
vec3 Light::getId() {
   return(this->Id);
}

/**
 * @brief Light::setId
 * @param i
 */
void Light::setId(vec3 i) {
    this->Id = i;
}


/**
 * @brief Light::getIa
 * @return
 */
vec3 Light::getIa() const
{
       return(this->Ia);
}

/**
 * @brief Light::setIa
 * @param value
 */
void Light::setIa(const vec3 &value)
{
    this->Ia = value;

}

/**
 * @brief Light::getIs
 * @return
 */
vec3 Light::getIs() const
{
       return(this->Is);
}

/**
 * @brief Light::setIs
 * @param value
 */
void Light::setIs(const vec3 &value)
{
    this->Is = value;
}

/**
 * @brief Light::getCoeficients
 * @return
 */
vec3 Light::getCoeficients() const
{
       return(this->coeficients);
}

/**
 * @brief Light::setCoeficients
 * @param value
 */
void Light::setCoeficients(const vec3 &value)
{
    this->coeficients = value;
}


/**
 * @brief Light::getTipusLight
 *
 */
LightType Light::getTipusLight() const
{
    return this->type;
}

/**
 * @brief Light::setTipusLight
 * @param value
 */
void Light::setTipusLight(const LightType &value)
{
    this->type = value;
}



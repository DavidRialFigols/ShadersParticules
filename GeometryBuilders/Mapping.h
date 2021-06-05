#pragma once

#include <string.h>

#include "DataService/ConfigMappingReader.h"
#include "Geometry/ColorMap.h"
#include "Geometry/ColorMapStatic.h"
#include "Geometry/Material.h"
#include "GeometryBuilders/MaterialFactory.h"

#include <QString>

using namespace std;



class Mapping
{
public:

    Mapping();
    Mapping(const shared_ptr<ConfigMappingReader> configMapping);

    vec3  mapeigPunt(vec3 puntMonReal);
    float mapeigValor(float valorMonReal);
    //Seguents 2 metodes fets per poder traslladar i escalar els gizmos (dades reals)
    vec3 mapeigPuntGizmo(vec3 puntMonReal);
    float mapeigEscalaGizmo(int i, float valorMonReal);

    shared_ptr<Material> mapeigMaterial(int i, ColorMapStatic::COLOR_MAP_TYPES tCM, MaterialFactory::MATERIAL_TYPES mat_type, double valorReal);

    int getNumProps() { return setup->numProp; }


    ObjectFactory::OBJECT_TYPES getObjectTypeProp(int i) {
        return setup->props[i].first;
    }

    ColorMapStatic::COLOR_MAP_TYPES getColorMapProp(int i) {
        return setup->props[i].second.second.first;
    }

    bool getTextMapType(int i) {
        return setup->props[i].second.second.second;
    }
    shared_ptr<QOpenGLTexture> getTextureProp(int i) {
        return setup->propTexture[i];
    }

    QString getQStringProp(int i) {
        return setup->props[i].second.first;
    }

    shared_ptr<ColorMap> getColorMap(ColorMapStatic::COLOR_MAP_TYPES tCM);


    vec3 getNormalPlaBase(){
        return setup->normalPlaBase;
    }

    double getdPlaBase(){
        return setup->dPlaBase;
    }

    shared_ptr<Material> getMaterialPlaBase() {
        return setup->materialPlaBase;
    }

    float getXmin(){
        return setup->Vxmin;
    }

    float getXmax(){
        return setup->Vxmax;
    }

    float getZmin(){
        return setup->Vzmin;
    }

    float getZmax(){
        return setup->Vzmax;
    }

    shared_ptr<QOpenGLTexture> getTexturePlaBase() {
        return setup->texture;
    }

    bool getTextMapType() {
        return setup->isTextDirectPlaBase;
    }

    vec3 getGlobalLight() {
        return setup->globalLight;
    }

    vector<shared_ptr<Light>> getLights() {
        return setup->lights;
    }

    vec3 getToonDirection() {
        return setup->toonDirection;
    }

private:
    shared_ptr<ConfigMappingReader> setup;
};


#pragma once


#include <QFile>
#include <QTextStream>
#include <iostream>
#include <sstream>
#include <QObject>
#include <QString>
#include <QStringList>
#include <vector>
#include "Geometry/Scene.h"
#include "GeometryBuilders/ObjectFactory.h"
#include "Geometry/ColorMapStatic.h"
#include "GeometryBuilders/MaterialFactory.h"

class ConfigMappingReader {
public:
    explicit ConfigMappingReader();

    ConfigMappingReader(QString filename);

    void readFile (QString fileName);

protected:
    virtual void fileLineRead (QString lineReaded);

    void mappingTypeFound(QStringList fields);

    void limitsRealFound(QStringList list);

    void limitsVirtualFound(QStringList list);

    void baseFound(QStringList list);

    void propFound(QStringList list);

    void globalLightFound(QStringList list);

    void lightFound(QStringList list);

    void toonDirectionFound(QStringList list);

public:
    Scene::DATA_TYPES   dataType;
    float               Rxmin, Rxmax, Rymin, Rymax, Rzmin, Rzmax;
    float               Vxmin, Vxmax, Vymin, Vymax, Vzmin, Vzmax;
    int                 numProp;
    vec3                normalPlaBase;
    double              dPlaBase;
    shared_ptr<Material> materialPlaBase;
    shared_ptr<QOpenGLTexture> texture;
    bool isTextDirectPlaBase;
    vec3 globalLight = vec3(0.0f, 0.0f, 0.0f);
    vec3 toonDirection = vec3(0.0f, 0.0f, 0.0f);
    // Vector de llums
    std::vector<shared_ptr<Light>> lights;

    vector<pair<double, double>> propLimits;
    vector<pair<ObjectFactory::OBJECT_TYPES, pair<QString, pair<ColorMapStatic::COLOR_MAP_TYPES, bool>>>> props;
    vector<shared_ptr<QOpenGLTexture>> propTexture;
};


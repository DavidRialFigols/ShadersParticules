#pragma once

#include "Geometry/Object.h"
#include "Geometry/Plane.h"
#include "Geometry/TranslateTG.h"
#include "Geometry/Material.h"

class ObjectFactory
{
    ObjectFactory(){}

public:
    enum  OBJECT_TYPES{
        OBJECT,
        PLANE
    };

    static ObjectFactory& getInstance() {
        static ObjectFactory instance;
        return instance;
    }

    shared_ptr<Object> createObject(vec3 normal, double d, float xmin, float xmax, float zmin, float zmax, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t);
    shared_ptr<Object> createObject(QString s, OBJECT_TYPES t);
    //Crea un objecte unitari i l'escala segons scale i el trasllada al centre center.
    shared_ptr<Object> createObject(float scale, vec3 center, QString s, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t);
    shared_ptr<Object> createObject(float scale, vec3 center, QString s, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t);
};


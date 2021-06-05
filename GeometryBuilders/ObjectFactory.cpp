#include "ObjectFactory.h"

shared_ptr<Object> ObjectFactory::createObject(vec3 normal, double d, float xmin, float xmax, float zmin, float zmax, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t)
{
    shared_ptr<Object> o;
    switch (t) {
    case PLANE:
        o = make_shared<Plane>(normal, d, xmin, xmax, zmin, zmax, mat, texture, isTextMapDirect);
        break;
    default:
        break;
    }

    return o;
}

shared_ptr<Object> ObjectFactory::createObject(QString s, OBJECT_TYPES t)
{
    shared_ptr<Object> o;
    switch (t) {
        case OBJECT:
            o = make_shared<Object>(3000000, s, nullptr);
        break;
    default:
        break;
    }

    return o;
}

shared_ptr<Object> ObjectFactory::createObject(float scale, vec3 center, QString s, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t) {
    shared_ptr<Object> o;
    switch (t) {
        case OBJECT: {
            o = make_shared<Object>(3000000, s, texture, true, isTextMapDirect);
            shared_ptr<ScaleTG> scale_m = make_shared<ScaleTG>(scale);
            o->aplicaTG(scale_m);
            shared_ptr<TranslateTG> translation = make_shared<TranslateTG>(center);
            o->aplicaTG(translation);
        }
        break;
    default:
        break;
    }

    return o;
}

shared_ptr<Object> ObjectFactory::createObject(float scale, vec3 center, QString s, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, OBJECT_TYPES t) {
    shared_ptr<Object> o;
    switch (t) {
        case OBJECT: {
            o = make_shared<Object>(3000000, s, mat, texture, true, isTextMapDirect);
            shared_ptr<ScaleTG> scale_m = make_shared<ScaleTG>(scale);
            o->aplicaTG(scale_m);
            shared_ptr<TranslateTG> translation = make_shared<TranslateTG>(center);
            o->aplicaTG(translation);
        }
        break;
    default:
        break;
    }

    return o;
}

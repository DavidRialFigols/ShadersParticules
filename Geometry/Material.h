#pragma once



#include <library/Common.h>

#include <QGLShaderProgram>
#include <memory>

using namespace Common;

using namespace std;
// TO DO: A canviar a la fase 1 de la practica 2
// Classe que representa els materials d'un objecte
class Material {


public:
    Material();
    Material(vec3 d);
    Material(vec3 a, vec3 d, vec3 s, float shininess);
    ~Material();

    void toGPU(shared_ptr<QGLShaderProgram> program);

    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;

protected:
};


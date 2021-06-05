#include "Geometry/Material.h"


//Valors arbitraris. Podem decidir canviar-los
Material::Material() {
    //Valors arbitraris. Podem decidir canviar-los.
    Ka = vec3(1.0f, 1.0f, 1.0f);
    Kd = vec3(1.0f, 1.0f, 1.0f);
    Ks = vec3(1.0f, 1.0f, 1.0f);
    shininess = 1.0f;
}

Material::~Material()
{}

Material::Material(vec3 d) {
    Kd = d;
    //Valors arbitraris. Podem decidir canviar-los.
    Ka = vec3(1.0f, 1.0f, 1.0f);
    Ks = vec3(1.0f, 1.0f, 1.0f);
    shininess = 1.0f;
}

Material::Material(vec3 a, vec3 d, vec3 s, float shininess) {
    //Fase 2
    Ka = a;
    Kd = d;
    Ks = s;
    this->shininess = shininess;
}

/**
 * Passa el material de CPU a GPU
 * @brief Material::toGPU
 * @param program
 */
void Material::toGPU(shared_ptr<QGLShaderProgram> program){
    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object

    // 1. Per a passar els diferents atributs del shader declarem una estructura amb els identificadors de la GPU
    struct {
        GLuint Ka;
        GLuint Kd;
        GLuint Ks;
        GLfloat shininess;
    } gl_Material;

    // 2. obtencio dels identificadors de la GPU
    gl_Material.Ka = program->uniformLocation("material.Ka");
    gl_Material.Ks = program->uniformLocation("material.Ks");
    gl_Material.Kd = program->uniformLocation("material.Kd");
    gl_Material.shininess = program->uniformLocation("material.shininess");

    // 3. Bind de les zones de memòria que corresponen a la GPU a valors de les variables de la CPU
    glUniform4fv(gl_Material.Ka, 1, vec4(this->Ka,1.0f));
    glUniform4fv(gl_Material.Ks, 1, vec4(this->Ks,1.0f));
    glUniform4fv(gl_Material.Kd, 1, vec4(this->Kd,1.0f));
    glUniform1f(gl_Material.shininess, this->shininess);
}


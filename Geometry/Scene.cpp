#include "Geometry/Scene.h"


/**
 * @brief Scene::Scene
 */
Scene::Scene() {
    // Inicialització de la capsa minima
    capsaMinima.pmin = vec3(-1.0, -1.0,-1.0);
    capsaMinima.a = 2;
    capsaMinima.h = 2;
    capsaMinima.p = 2;
    // Posem una llum ambient global per defecte
    lightAmbientGlobal = vec3(0.3, 0.3, 0.3);
    //Posem una direccio pel toon shading per defecte
    lightToonDirection = vec3(-1.0f, -1.0f, 0.0f);
    // Creacio d'una primera Light per apoder modificar el seus valors amb la interficie
    // Si es modifiquen els valors de la llum, modificar tambe els valors per defecte de la interficie
    // perque coincideixin amb els inicials
    auto l  = make_shared<PointLight>(vec3(0.2,0.2,0.2), vec3(0.8, 0.8, 0.8), vec3(1.0, 1.0, 1.0), vec3(0, 0, 0), vec4(10, 10, 20, 1));
    this->addLight(l);


}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene() {
    objects.clear();
    lights.clear();
}

/**
 * @brief Scene::addObject
 * @param obj
 */
void Scene::addObject(shared_ptr<Object> obj) {
    objects.push_back(obj);
    calculCapsaMinCont3DEscena();
}

/**
 * @brief Scene::toGPU
 */
void Scene::toGPU(shared_ptr<QGLShaderProgram> p) {
    for(unsigned int i=0; i < objects.size(); i++){
        objects.at(i)->toGPU(p);
    }
    lightsToGPU(p);
    toonLightDirectionToGPU(p);
    setAmbientToGPU(p);
}

/**
 * @brief Scene::draw
 */
void Scene::draw() {
    for(unsigned int i=0; i < objects.size(); i++){
        objects.at(i)->draw();
    }
}

/**
 * @brief Scene::drawTexture
 */
void Scene::drawTexture() {
    for(unsigned int i=0; i < objects.size(); i++){
        objects.at(i)->drawTexture();
    }
}

/**
 * @brief Scene::getLightActual
 * @return
 */
shared_ptr<Light> Scene::getLightActual() {
    // TO DO OPCIONAL: A modificar si es vol canviar el comportament de la GUI
    // Ara per ara dona com a Light actual la darrera que s'ha inserit
    return (lights[lights.size()-1]);
}

/**
 * @brief Scene::setLightActual
 * @param l
 */
void Scene::setLightActual(shared_ptr<Light> l){
    lights[lights.size()-1]=l;
}



/**
 * @brief Scene::lightsToGPU
 * @param program
 */
void Scene::lightsToGPU(shared_ptr<QGLShaderProgram> program){
    struct gl_IdLight{
        GLuint Ia;
        GLuint Id;
        GLuint Is;
        GLuint coeficients;
        GLuint lightType;
        GLuint position;
        GLuint direction;
        GLuint angle;
    };

    gl_IdLight gl_lights[MAX_NUM_LIGHTS];

    int n = MAX_NUM_LIGHTS;
    if(MAX_NUM_LIGHTS > lights.size()){
        n = lights.size();
    }

    for (int i=0; i<n; i++) {

        gl_lights[i].Ia = program->uniformLocation(QString("lights[%1].Ia").arg(i));
        gl_lights[i].Id = program->uniformLocation(QString("lights[%1].Id").arg(i));
        gl_lights[i].Is = program->uniformLocation(QString("lights[%1].Is").arg(i));

        gl_lights[i].coeficients = program->uniformLocation(QString("lights[%1].coeficients").arg(i));

        gl_lights[i].lightType = program->uniformLocation(QString("lights[%1].lightType").arg(i));

        gl_lights[i].position = program->uniformLocation(QString("lights[%1].position").arg(i));
        gl_lights[i].direction = program->uniformLocation(QString("lights[%1].direction").arg(i));
        gl_lights[i].angle = program->uniformLocation(QString("lights[%1].angle").arg(i));


        glUniform4fv(gl_lights[i].Ia, 1, vec4(lights[i]->getIa(),1.0f));
        glUniform4fv(gl_lights[i].Id, 1, vec4(lights[i]->getId(),1.0f));
        glUniform4fv(gl_lights[i].Is, 1, vec4(lights[i]->getIs(),1.0f));

        glUniform3fv(gl_lights[i].coeficients, 1, lights[i]->getCoeficients());

        int type = 0;
        vec4 position(0.0f, 0.0f, 0.0f, 1.0f);
        vec4 direction(0.f);
        float angle = 0.f;
        if (lights[i]->getTipusLight() == Puntual){
            auto pl = dynamic_pointer_cast<PointLight>(lights[i]);
            type = 0;
            position = pl->getLightPosition();
        }
        else if(lights[i]->getTipusLight() == Direccional){
            type = 1;
            direction = (dynamic_pointer_cast<DirectionalLight>(lights[i]))->getLightDirection();
        }
        else if(lights[i]->getTipusLight() == Spot){
            type = 2;
            position = (dynamic_pointer_cast<SpotLight>(lights[i]))->getLightPosition();
            direction = (dynamic_pointer_cast<SpotLight>(lights[i]))->getLightDirection();
            angle = (dynamic_pointer_cast<SpotLight>(lights[i]))->getAngle();
        }

        glUniform1i(gl_lights[i].lightType, type);
        glUniform4fv(gl_lights[i].position, 1, position);
        glUniform4fv(gl_lights[i].direction, 1, direction);
        glUniform1f(gl_lights[i].angle, angle);
    }

    // Acabem d'omplir el vector de llums de la GPU amb llums "buides" que no afectaran
    // en el calcul del color final. Ho fem perque si anteriorment s'ha carregat una escena
    // amb mes llums que l'actual, les ultimes llums no s'actualitzaran a la GPU i afectaran
    // en el calcul de la nova escena
    for (int i=n; i<MAX_NUM_LIGHTS; i++) {
        gl_lights[i].Ia = program->uniformLocation(QString("lights[%1].Ia").arg(i));
        gl_lights[i].Id = program->uniformLocation(QString("lights[%1].Id").arg(i));
        gl_lights[i].Is = program->uniformLocation(QString("lights[%1].Is").arg(i));

        gl_lights[i].coeficients = program->uniformLocation(QString("lights[%1].coeficients").arg(i));

        gl_lights[i].lightType = program->uniformLocation(QString("lights[%1].lightType").arg(i));

        gl_lights[i].position = program->uniformLocation(QString("lights[%1].position").arg(i));
        gl_lights[i].direction = program->uniformLocation(QString("lights[%1].direction").arg(i));
        gl_lights[i].angle = program->uniformLocation(QString("lights[%1].angle").arg(i));

        glUniform4fv(gl_lights[i].Ia, 1, vec4(0.0f,0.0f,0.0f,1.0f));
        glUniform4fv(gl_lights[i].Id, 1, vec4(0.0f,0.0f,0.0f,1.0f));
        glUniform4fv(gl_lights[i].Is, 1, vec4(0.0f,0.0f,0.0f,1.0f));
        glUniform3fv(gl_lights[i].coeficients, 1, vec3(0.0f,0.0f,0.0f));
        glUniform1i(gl_lights[i].lightType, 0);
        glUniform4fv(gl_lights[i].position, 1, vec4(0.0f,0.0f,0.0f,1.0f));
        glUniform4fv(gl_lights[i].direction, 1, vec4(0.0f,0.0f,0.0f,0.0f));
        glUniform1f(gl_lights[i].angle, 0);
    }

}

void Scene::addLight(shared_ptr<Light> l) {
    lights.push_back(l);
}

/**
 * @brief Scene::setAmbientToGPU
 * @param program
 */
void Scene::setAmbientToGPU(shared_ptr<QGLShaderProgram> program){
    struct gl_IdAmbientLight{
        GLuint Ia;
    };

    gl_IdAmbientLight gl_ambientLight;

    gl_ambientLight.Ia = program->uniformLocation(QString("ambientLight.Ia"));

    glUniform4fv(gl_ambientLight.Ia, 1, vec4(lightAmbientGlobal,1.0f));
}

void Scene::toonLightDirectionToGPU(shared_ptr<QGLShaderProgram> program) {
    GLuint glToonLightDirection = program->uniformLocation("toonLightDirection");
    glUniform4fv(glToonLightDirection, 1, vec4(lightToonDirection,0));

}

/**
 * @brief Scene::setCamera
 * @param Camera *
 */
void Scene::setCamera(shared_ptr<Camera> c) {
    camera = c;
}
/**
 * @brief Scene::calculCapsaMinCont3DEscena
 */

void Scene::calculCapsaMinCont3DEscena()
{
    Capsa3D c;
    vec3 pmax;

    if (objects.size()==1) {
        capsaMinima = objects[0]->calculCapsa3D();
        pmax[0] = capsaMinima.pmin[0]+capsaMinima.a;
        pmax[1] = capsaMinima.pmin[1]+capsaMinima.h;
        pmax[2] = capsaMinima.pmin[2]+capsaMinima.p;
    } else {
        capsaMinima.pmin[0]=200;
        capsaMinima.pmin[1]=200;
        capsaMinima.pmin[2]=200;
        pmax[0] = -200;
        pmax[1] = -200;
        pmax[2] = -200;
    }

    for (unsigned int i=0; i<objects.size(); i++) {
       c = objects[i]->calculCapsa3D();

       if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
       if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
       if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
       if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
       if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
       if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
    }
    capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
    capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
    capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
}

void Scene::update(int nframe) {
    for (unsigned int i = 0; i< objects.size(); i++) {
        if (dynamic_pointer_cast<Animable>(objects[i]) ) objects[i]->update(nframe);
    }
}

void Scene::setAmbientLight(vec3 ambientLight) {
    this->lightAmbientGlobal = ambientLight;
}

void Scene::setLightToonDirection(vec3 direction) {
    this->lightToonDirection = direction;
}

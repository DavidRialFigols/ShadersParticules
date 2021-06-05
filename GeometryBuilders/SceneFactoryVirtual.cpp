#include "SceneFactoryVirtual.h"

#include "Geometry/PointLight.h"


SceneFactoryVirtual::SceneFactoryVirtual(shared_ptr<ConfigMappingReader> mr):SceneFactory()
{
    map = make_shared<Mapping>(mr);
}

shared_ptr<Scene> SceneFactoryVirtual::createScene(QString filename) {

    auto scene= make_shared<Scene>();

    //Posem les llums a l'escena
    scene->setAmbientLight(map->getGlobalLight());

    vector<shared_ptr<Light>> lights = map->getLights();
    for (unsigned int i = 0; i < lights.size(); i++) {
        scene->addLight(lights[i]);
    }

    vec3 toonDirection = map->getToonDirection();
    if (!(toonDirection.x == 0 && toonDirection.y == 0 && toonDirection.z == 0)) {
        scene->setLightToonDirection(toonDirection);
    }

    shared_ptr<VirtualWorldReader> vwr = make_shared<VirtualWorldReader>(scene);
    vwr->readFile(filename, map);
    return scene;
}

shared_ptr<Scene> SceneFactoryVirtual::createScene() {
    auto scene= make_shared<Scene>();
    this->OneSphere(scene);
    return scene;
}

void SceneFactoryVirtual::OneSphere(shared_ptr<Scene> s) {
    //auto sphere = make_shared<Sphere>(vec3(0, 0, -1), 0.5, 1.0);
    //sphere->setMaterial(make_shared<Lambertian>(vec3(0.5, 0.2, 0.7)));

    // CODI EXEMPLE PER FER ANIMACIONS

    //    shared_ptr<Animation> anim = make_shared<Animation>();
    //    anim->transf =  make_shared<Translate>(vec3(0.2));
    //    sphere->addAnimation(anim);

    //s->objects.push_back(sphere);

}

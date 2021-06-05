#include "SceneFactoryData.h"

SceneFactoryData::SceneFactoryData(shared_ptr<ConfigMappingReader> mr):SceneFactory()
{
    map = make_shared<Mapping>(mr);
}


shared_ptr<Scene> SceneFactoryData::createScene(QString filename) {

    shared_ptr<Object> base = ObjectFactory::getInstance().createObject(map->getNormalPlaBase(),
                                                                        map->getdPlaBase(),
                                                                        map->getXmin(), map->getXmax(),
                                                                        map->getZmin(), map->getZmax(),
                                                                        map->getMaterialPlaBase(),
                                                                        map->getTexturePlaBase(),
                                                                        map->getTextMapType(),
                                                                        ObjectFactory::OBJECT_TYPES::PLANE);
    shared_ptr<Scene> scene = make_shared<Scene>();
    scene->addObject(base);

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

    auto rdr = make_shared<RealDataReader>(scene);

    rdr->readFile(filename, map);
    return scene;
}

shared_ptr<Scene> SceneFactoryData::createScene() {
    auto scene= make_shared<Scene>();
    //  TODO: Només si voleu provar manualment algunes dades posades
    //  hardcoded aqui

    return scene;
}

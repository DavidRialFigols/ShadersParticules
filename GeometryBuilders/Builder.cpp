#include "GeometryBuilders/Builder.h"

Builder::Builder(GLWidget *glWid)
{
    glWidget = glWid;
    scene = glWidget->getScene();

}

void Builder::newObjFromFile()
{
    QString fileName = QFileDialog::getOpenFileName();
    if (!fileName.isNull()) {
            auto obj = ObjectFactory::getInstance().createObject(fileName, ObjectFactory::OBJECT_TYPES::OBJECT);
            scene->addObject(obj);
            scene->camera->actualitzaCamera(scene->capsaMinima);
            emit newObj(obj);
    }
}

void Builder::newVirtualScene() {
    // Fase 1: creació de la nova escena Virtual a partir d'un fitxer
    // de  a partir de les dades
    // del configMapping  i un fitxer .txt de tipus VIRTUALWORLD.
    // Nomes hi hauran fitxers de tipus BoundaryObject.
    // Usa el ConfigMappingReader i la teva SceneFactoryVirtual
    // per a construir l'escena tal i com feies a la practica 1

    QString configMapping = QFileDialog::getOpenFileName(nullptr, tr("Select the config mapping file"));
    if (configMapping.isNull()) {
        //Si no s'ha entrat cap fitxer sortim del metode
        return;
    }
    QString dataFileName = QFileDialog::getOpenFileName(nullptr, tr("Select the data file"));
    if (dataFileName.isNull()) {
        //Si no s'ha entrat cap fitxer sortim del metode
        return;
    }

    shared_ptr<ConfigMappingReader> mapping = make_shared<ConfigMappingReader>(configMapping);
    auto factory = make_shared<SceneFactoryVirtual>(mapping);

    scene = factory->createScene(dataFileName);
     emit newScene(scene);
}


void Builder::newDataScene()
{
    // Fase 1: Crear la nova escena a partir de les dades
    // del configMapping i el fitxer .txt que conté les dades
    // Utilitza el ConfigMappingReader i la teva SceneFactoryData per a llegir els fitxers
    // i crear l'escena corresponent.

    // Opcionalment pots crear un dialeg per posar els valors del mapping

    QString configMapping = QFileDialog::getOpenFileName(nullptr, tr("Select the config mapping file"));
    if (configMapping.isNull()) {
        //Si no s'ha entrat cap fitxer sortim del metode
        return;
    }

    QString dataFileName = QFileDialog::getOpenFileName(nullptr, tr("Select the data file"));
    if (dataFileName.isNull()) {
        //Si no s'ha entrat cap fitxer sortim del metode
        return;
    }

    shared_ptr<ConfigMappingReader> mapping = make_shared<ConfigMappingReader>(configMapping);
    auto factory = make_shared<SceneFactoryData>(mapping);

    scene = factory->createScene(dataFileName);
    emit newScene(scene);
}

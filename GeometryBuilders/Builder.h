#ifndef BUILDER_H
#define BUILDER_H

#include <library/Common.h>
using namespace Common;

#include <QtWidgets>
#include <Renders/GLWidget.h>
#include <Geometry/Material.h>
#include <DataService/ConfigMappingReader.h>
#include <GeometryBuilders/SceneFactoryVirtual.h>
#include <GeometryBuilders/SceneFactoryData.h>
#include <GeometryBuilders/ObjectFactory.h>

class Builder : public QObject {
        Q_OBJECT

    shared_ptr<Scene> scene;
    GLWidget *glWidget;

public:
    Builder(GLWidget *glWid);

public slots:
    void newObjFromFile();
    void newVirtualScene();
    void newDataScene();

signals:
    void newObj(shared_ptr<Object> o);
    void newScene(shared_ptr<Scene> sc);
};

#endif // BUILDER_H

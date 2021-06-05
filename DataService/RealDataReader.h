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
#include "GeometryBuilders/Mapping.h"
#include "GeometryBuilders/MaterialFactory.h"

class RealDataReader {
public:
    explicit RealDataReader();

    RealDataReader(shared_ptr<Scene> s);

    void readFile (QString fileName, shared_ptr<Mapping> map);

private:

    virtual void fileLineRead (QString lineReaded);

    shared_ptr<Scene> scene;
    shared_ptr<Mapping> mapping;

    void dataFound(QStringList list);

};

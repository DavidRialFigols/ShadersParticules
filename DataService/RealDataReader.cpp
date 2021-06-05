#include "RealDataReader.h"


RealDataReader::RealDataReader(shared_ptr<Scene> s)
{
    scene = s;
}

void RealDataReader::readFile(QString fileName, shared_ptr<Mapping> map) {
    QFile file(fileName);

    mapping = map;

    if(!file.open(QIODevice::ReadOnly)) {
        std::cerr << "Error opening the file" << std::endl;
        return;
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        fileLineRead(line);
    }

    file.close();
}

void RealDataReader::fileLineRead (QString lineReaded) {

    QStringList fields = lineReaded.split(",");
    if (QString::compare("data", fields[0], Qt::CaseInsensitive) == 0)
        dataFound(fields);
    else
        std::cerr << "Element unknown" << std::endl;
}

void RealDataReader::dataFound(QStringList fields) {
    //data x z valor1 valor2...
    int n = mapping->getNumProps();

    if (fields.size() != (3 + n)) {
        std::cerr << "Wrong data format" << std::endl;
        return;
    }

    vec3 puntBase = vec3(fields[1].toDouble(), 0.0f, fields[2].toDouble());//mapping->getPuntBase(vec2(fields[1].toDouble(), fields[2].toDouble()));
    for (int i=0; i<n; i++) {

        shared_ptr<Object> o;
        // Construccio de l'objecte al Mon Virtual
        float escala = mapping->mapeigEscalaGizmo(i, fields[3 + i].toFloat());
        vec3 centre = mapping->mapeigPuntGizmo(puntBase);

        QString s = mapping->getQStringProp(i);

        auto mat = mapping->mapeigMaterial(i, mapping->getColorMapProp(i), MaterialFactory::MATERIAL_TYPES::COLOR_MAP, fields[3 + i].toDouble());
        auto texture = mapping->getTextureProp(i);
        o = ObjectFactory::getInstance().createObject(escala, centre, s, mat, texture, mapping->getTextMapType(i), mapping->getObjectTypeProp(i));

        // Afegir objecte a l'escena
        scene->addObject(o);
    }
}

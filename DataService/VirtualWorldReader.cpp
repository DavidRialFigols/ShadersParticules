#include "VirtualWorldReader.h"


VirtualWorldReader::VirtualWorldReader(shared_ptr<Scene> s)
{
    scene = s;
}

void VirtualWorldReader::readFile(QString fileName, shared_ptr<Mapping> map) {

    mapping = map;
    std::cout << fileName.toStdString()<<std::endl;

    QFile file(fileName);

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

void VirtualWorldReader::fileLineRead (QString lineReaded) {
    QStringList fields = lineReaded.split(",");
    if (QString::compare("brobject", fields[0], Qt::CaseInsensitive) == 0)
        brObjectFound(fields);
    else
        std::cerr << "Element unknown" << std::endl;
}

void VirtualWorldReader::brObjectFound(QStringList fields) {

    if (fields.size() != 8 && fields.size() != 18) {
        std::cerr << "Wrong brObject format" << std::endl;
        return;
    }

    QString fileName = fields[5];
    vec3 center = mapping->mapeigPunt(vec3(fields[1].toFloat(), fields[2].toFloat(), fields[3].toFloat()));
    float scale = mapping->mapeigValor(fields[4].toFloat());
    shared_ptr<QOpenGLTexture> texture;
    if(fields[6] == "NULL") {
        texture = nullptr;
    } else {
        texture = make_shared<QOpenGLTexture>(QImage(fields[6]).mirrored());
    }
    bool isTextMapDirect = true;
    if (QString::compare("indirect", fields[7], Qt::CaseInsensitive) == 0) {
        isTextMapDirect = false;
    }
    shared_ptr<Object> object;
    // Si no s'entren les propietats del material, vol dir que el material estara definit
    // a l'obj. Si no el te definit a l'obj, per defecte se li assignara un color.
    if (fields.size() == 8) {
        object = ObjectFactory::getInstance().createObject(scale, center, fileName, texture, isTextMapDirect, ObjectFactory::OBJECT_TYPES::OBJECT);
    } else {
        vec3 Ka(fields[8].toFloat(), fields[9].toFloat(), fields[10].toFloat());
        vec3 Kd(fields[11].toFloat(), fields[12].toFloat(), fields[13].toFloat());
        vec3 Ks(fields[14].toFloat(), fields[15].toFloat(), fields[16].toFloat());
        float shininess = fields[17].toFloat();
        shared_ptr<Material> mat = make_shared<Material>(Ka, Kd, Ks, shininess);
        object = ObjectFactory::getInstance().createObject(scale, center, fileName, mat, texture, isTextMapDirect, ObjectFactory::OBJECT_TYPES::OBJECT);
    }
    scene->addObject(object);
}

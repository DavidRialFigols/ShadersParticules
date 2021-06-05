#include "ConfigMappingReader.h"


ConfigMappingReader::ConfigMappingReader(QString filename)
{
  numProp = 0;
  readFile(filename);

}

void ConfigMappingReader::readFile(QString fileName) {
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

void ConfigMappingReader::fileLineRead (QString lineReaded) {

    QStringList fields = lineReaded.split(",");
    if (QString::compare("limitsMonVirtual", fields[0], Qt::CaseInsensitive) == 0)
        limitsVirtualFound(fields);
    else  if (QString::compare("limitsMonReal", fields[0], Qt::CaseInsensitive) == 0)
        limitsRealFound(fields);
    else if (QString::compare("dataType", fields[0], Qt::CaseInsensitive) == 0)
        mappingTypeFound(fields);
    else if (QString::compare("base", fields[0], Qt::CaseInsensitive) == 0)
        baseFound(fields);
    else if (QString::compare("prop", fields[0], Qt::CaseInsensitive) == 0)
        propFound(fields);
    else if (QString::compare("globalLight", fields[0], Qt::CaseInsensitive) == 0)
        globalLightFound(fields);
    else if (QString::compare("light", fields[0], Qt::CaseInsensitive) == 0)
        lightFound(fields);
    else if (QString::compare("toonDirection", fields[0], Qt::CaseInsensitive) == 0)
        toonDirectionFound(fields);
    else
        std::cerr << "Element unknown" << std::endl;
}


void ConfigMappingReader::mappingTypeFound(QStringList fields) {
    //rendertype VIRTUALWORLD, REAL, TEMPORAL
    if (fields.size() != 2) {
        std::cerr << "Wrong propietat format" << std::endl;
        return;
    }
    if (QString::compare("VIRTUALWORLD", fields[1], Qt::CaseInsensitive) == 0)
        dataType = Scene::DATA_TYPES::VIRTUALWORLD;
    else if (QString::compare("REALDATA", fields[1], Qt::CaseInsensitive) == 0)
        dataType = Scene::DATA_TYPES::REALDATA;
    else if (QString::compare("TEMPORALVW", fields[1], Qt::CaseInsensitive) == 0)
        dataType = Scene::DATA_TYPES::TEMPORALVW;
    else if (QString::compare("TEMPORALDATA", fields[1], Qt::CaseInsensitive) == 0)
        dataType = Scene::DATA_TYPES::TEMPORALDATA;
    else std::cerr << "Error de sintaxi en el tipus de dades" << std::endl;
}

void ConfigMappingReader::limitsRealFound(QStringList fields) {
    // limitsMonVirtual xmin xmax zmin zmax
    if (this->dataType==Scene::DATA_TYPES::REALDATA) {
        if (fields.size() != 5 ) {
            std::cerr << "Wrong limits format" << std::endl;
            return;
        }
        Rxmin = fields[1].toDouble();
        Rxmax = fields[2].toDouble();
        Rzmin = fields[3].toDouble();
        Rzmax = fields[4].toDouble();

    } else if (this->dataType==Scene::DATA_TYPES::VIRTUALWORLD) {
        // limitsMonVirtual xmin xmax ymin ymax zmin zmax
        if (fields.size() != 7 ) {
            std::cerr << "Wrong limits format" << std::endl;
            return;
        }
        Rxmin = fields[1].toDouble();
        Rxmax = fields[2].toDouble();
        Rymin = fields[3].toDouble();
        Rymax = fields[4].toDouble();
        Rzmin = fields[5].toDouble();
        Rzmax = fields[6].toDouble();
    }
}

void ConfigMappingReader::limitsVirtualFound(QStringList fields) {
    // limitsMonVirtual xmin xmax zmin zmax
    if (fields.size() != 7 ) {
        std::cerr << "Wrong limits format" << std::endl;
        return;
    }
    Vxmin = fields[1].toDouble();
    Vxmax = fields[2].toDouble();
    Vymin = fields[3].toDouble();
    Vymax = fields[4].toDouble();
    Vzmin = fields[5].toDouble();
    Vzmax = fields[6].toDouble();
}

void ConfigMappingReader::baseFound(QStringList fields) {
    //base plane nx ny nz d
    if (fields.size() != 18) {
        std::cerr << "Wrong base format" << std::endl;
        return;
    }

    if (QString::compare("plane", fields[1], Qt::CaseInsensitive) == 0) {
        normalPlaBase = vec3(fields[2].toDouble(), fields[3].toDouble(), fields[4].toDouble());
        dPlaBase = fields[5].toDouble();
        materialPlaBase = MaterialFactory::getInstance().createMaterial(vec3(fields[6].toDouble(), fields[7].toDouble(), fields[8].toDouble()),
                                                        vec3(fields[9].toDouble(), fields[10].toDouble(), fields[11].toDouble()),
                                                        vec3(fields[12].toDouble(), fields[13].toDouble(), fields[14].toDouble()),
                                                        fields[15].toDouble(), MaterialFactory::MATERIAL_TYPES::MATERIAL);

        if(fields[16] == "NULL") {
            texture = nullptr;
        } else {
            texture = make_shared<QOpenGLTexture>(QImage(fields[16]).mirrored());
        }

        if(QString::compare("direct", fields[17], Qt::CaseInsensitive) == 0) {
            isTextDirectPlaBase = true;
        } else if (QString::compare("indirect", fields[17], Qt::CaseInsensitive) == 0) {
            isTextDirectPlaBase = false;
        } else {
            isTextDirectPlaBase = true;
        }

    }
}

void ConfigMappingReader::propFound(QStringList fields) {
    //prop,numProp,vmin,vmax,tipusGizmo,path_brobject,color_map,path_texture,direct/indirect

    std::cerr<<"Prop found"<<std::endl;
    if (fields.size() != 9) {
        std::cerr << "Wrong propietat format config mapping" << std::endl;
        return;
    }
    numProp++;
    QString propQString;
    propLimits.push_back(std::make_pair(fields[2].toDouble(), fields[3].toDouble()));

    auto tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_INFERNO;
    if (QString::compare("COLOR_MAP_TYPE_INFERNO", fields[6], Qt::CaseInsensitive) == 0) {
        tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_INFERNO;
    } else if (QString::compare("COLOR_MAP_TYPE_PLASMA", fields[6], Qt::CaseInsensitive) == 0) {
       tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_PLASMA;
    } else if (QString::compare("COLOR_MAP_TYPE_MAGMA", fields[6], Qt::CaseInsensitive) == 0) {
        tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_MAGMA;
    } else if (QString::compare("COLOR_MAP_TYPE_VIRIDIS", fields[6], Qt::CaseInsensitive) == 0) {
        tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_VIRIDIS;
    } else if (QString::compare("COLOR_MAP_TYPE_PARULA", fields[6], Qt::CaseInsensitive) == 0) {
        tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_PARULA;
    } else if (QString::compare("COLOR_MAP_TYPE_JET", fields[6], Qt::CaseInsensitive) == 0) {
        tipusColorMap = ColorMapStatic::COLOR_MAP_TYPE_JET;
    } else {
        std::cerr << "Color map unknown" << std::endl;
        return;
    }

    auto tipusObj = ObjectFactory::OBJECT_TYPES::OBJECT;
    if (QString::compare("brobject", fields[4], Qt::CaseInsensitive) == 0) {
        //tipusObj = ObjectFactory::OBJECT_TYPES::OBJECT;
        propQString = fields[5];
    } else {
        std::cerr << "Object type unknown" << std::endl;
        return;
    }

    shared_ptr<QOpenGLTexture> texture;
    if(fields[7] == "NULL") {
        texture = nullptr;
    } else {
        texture = make_shared<QOpenGLTexture>(QImage(fields[7]).mirrored());
    }
    propTexture.push_back(texture);

    bool textureMapType = true;
    if (QString::compare("indirect", fields[8], Qt::CaseInsensitive) == 0) {
        textureMapType = false;
    }
    props.push_back(std::make_pair(tipusObj, std::make_pair(propQString, std::make_pair(tipusColorMap, textureMapType))));
}

void ConfigMappingReader::globalLightFound(QStringList fields) {
    //Format: globalLight, colorR, colorG, colorB
    if (fields.size() != 4) {
        std::cerr << "Wrong global light format" << std::endl;
        return;
    }
    globalLight = vec3(fields[1].toDouble(), fields[2].toDouble(), fields[3].toDouble());
}

void ConfigMappingReader::lightFound(QStringList fields) {
    if (fields.size() == 17) {
        if (QString::compare("pointLight", fields[1], Qt::CaseInsensitive) == 0) {
            shared_ptr<Light> l;
            vec3 Ia = vec3(fields[2].toFloat(), fields[3].toFloat(), fields[4].toFloat());
            vec3 Id = vec3(fields[5].toFloat(), fields[6].toFloat(), fields[7].toFloat());
            vec3 Is = vec3(fields[8].toFloat(), fields[9].toFloat(), fields[10].toFloat());
            float a = fields[11].toFloat(), b = fields[12].toFloat(), c = fields[13].toFloat();
            vec4 pos(fields[14].toFloat(), fields[15].toFloat(), fields[16].toFloat(), 1.0f);
            l = make_shared<PointLight>(Ia, Id, Is, vec3(a,b,c), pos);
            lights.push_back(l);
        }
        else {
            std::cerr << "Wrong light format" << std::endl;
            return;
        }
    }
    else if (fields.size() == 14) {
        if (QString::compare("directionalLight", fields[1], Qt::CaseInsensitive) == 0) {
            shared_ptr<Light> l;
            vec3 Ia = vec3(fields[2].toFloat(), fields[3].toFloat(), fields[4].toFloat());
            vec3 Id = vec3(fields[5].toFloat(), fields[6].toFloat(), fields[7].toFloat());
            vec3 Is = vec3(fields[8].toFloat(), fields[9].toFloat(), fields[10].toFloat());
            vec4 dir(fields[11].toFloat(), fields[12].toFloat(), fields[13].toFloat(), 0.0f);
            l = make_shared<DirectionalLight>(Ia, Id, Is, dir);
            lights.push_back(l);
        }
        else {
            std::cerr << "Wrong light format" << std::endl;
            return;
        }
    } else if(fields.size() == 21){
        if (QString::compare("spotLight", fields[1], Qt::CaseInsensitive) == 0) {
            shared_ptr<Light> l;
            vec3 Ia = vec3(fields[2].toFloat(), fields[3].toFloat(), fields[4].toFloat());
            vec3 Id = vec3(fields[5].toFloat(), fields[6].toFloat(), fields[7].toFloat());
            vec3 Is = vec3(fields[8].toFloat(), fields[9].toFloat(), fields[10].toFloat());
            float a = fields[11].toFloat(), b = fields[12].toFloat(), c = fields[13].toFloat();
            vec4 pos(fields[14].toFloat(), fields[15].toFloat(), fields[16].toFloat(), 1.0f);
            vec4 dir(fields[17].toFloat(), fields[18].toFloat(), fields[19].toFloat(), 0.0f);
            float angle = fields[20].toFloat();
            l = make_shared<SpotLight>(Ia, Id, Is, vec3(a,b,c), pos, dir, angle);
            lights.push_back(l);
        }
        else {
            std::cerr << "Wrong light format" << std::endl;
            return;
        }
    }
    else{
        std::cerr << "Wrong light format" << std::endl;
        return;
    }
}
void ConfigMappingReader::toonDirectionFound(QStringList fields) {
    if (fields.size() != 4) {
        std::cerr << "Wrong toon direction format" << std::endl;
        return;
    }
    vec3 direction = vec3(fields[1].toFloat(), fields[2].toFloat(), fields[3].toFloat());
    this->toonDirection = direction;
}



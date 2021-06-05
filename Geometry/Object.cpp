#include "Geometry/Object.h"


/**
 * @brief Object::Object
 * @param npoints
 * @param parent
 */
Object::Object(int npoints, QObject *parent) : QObject(parent){
    numPoints = npoints;
    points = new point4[numPoints];
    normals= new point4[numPoints];
    coordText = new vec2[numPoints];

    vec3 Ka(0.2, 0.2, 0.2);
    vec3 Kd(0.8, 0.5, 0.5);
    vec3 Ks(1.0, 1.0, 1.0);
    float shininess = 20.0f;
    shared_ptr<Material> mat = make_shared<Material>(Ka, Kd, Ks, shininess);
    material = mat;
    this->isTextMapDirect = true;
    this->texture = nullptr;

 }

/**
 * @brief Object::Object
 * @param npoints
 * @param mat
 * @param parent
 */
Object::Object(int npoints, shared_ptr<Material> mat, QObject *parent) : QObject(parent){
    numPoints = npoints;
    points = new point4[numPoints];
    normals= new point4[numPoints];
    coordText = new vec2[numPoints];

    material = mat;
    this->isTextMapDirect = true;
    this->texture = nullptr;
}

/**
 * @brief Object::Object
 * @param npoints
 * @param mat
 * @param texture
 * @param parent
 */
Object::Object(int npoints, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, QObject *parent) : QObject(parent){
    numPoints = npoints;
    points = new point4[numPoints];
    normals= new point4[numPoints];
    coordText = new vec2[numPoints];

    material = mat;
    this->texture = texture;
    this->isTextMapDirect = isTextMapDirect;
}


/**
 * @brief Object::Object
 * @param npoints
 * @param n
 * @param mat
 * @param untiari
 */
Object::Object(int npoints, QString n, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool unitari, bool isTextMapDirect) : numPoints(npoints){
    points = new point4[numPoints];
    normals= new point4[numPoints];
    coordText = new vec2[numPoints];
    this->texture = texture;
    this->isTextMapDirect = isTextMapDirect;
    parseObjFile(n);
    // Assignem el material. Si l'obj tenia material, el sobreescrivim perque s'ha passat el material
    // per parametre.
    material = mat;
    make();

    if (unitari) {
        makeUntari();
    }
}

/**
 * @brief Object::Object
 * @param npoints
 * @param n
 * @param unitari
 */
Object::Object(int npoints, QString n, shared_ptr<QOpenGLTexture> texture, bool unitari, bool isTextMapDirect) : numPoints(npoints) {
    points = new point4[numPoints];
    normals= new point4[numPoints];
    coordText = new vec2[numPoints];
    this->texture = texture;
    this->isTextMapDirect = isTextMapDirect;

    // Creem i assignem un material per defecte per si en el .obj no hi ha indicat el material
    vec3 Ka(0.2, 0.2, 0.2);
    vec3 Kd(0.8, 0.5, 0.5);
    vec3 Ks(1.0, 1.0, 1.0);
    float shininess = 20.0f;
    shared_ptr<Material> mat = make_shared<Material>(Ka, Kd, Ks, shininess);
    material = mat;

    parseObjFile(n);
    make();

    if (unitari)  {
        makeUntari();
    }
}


/**
 * @brief Object::~Object
 */
Object::~Object(){
    delete points;
    delete normals;
    delete coordText;
}

/**
 * @brief Object::toGPU
 * @param pr
 */
void Object::toGPU(shared_ptr<QGLShaderProgram> pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2

    qDebug() << "Obj to GPU.....";

    program = pr;
    // Creació d'un vertex array object

    glGenVertexArrays( 1, &vao );

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    glGenBuffers( 1, &buffer );

    // Aqui s'ha de fer el pas de dades a la GPU per si hi ha més d'un objecte
    // Activació a GL del Vertex Buffer Object
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU

    glBufferData( GL_ARRAY_BUFFER, 2*sizeof(point4)*Index + sizeof(vec2)*Index, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, normals );
    glBufferSubData( GL_ARRAY_BUFFER, 2*sizeof(point4)*Index, sizeof(vec2)*Index, coordText);

    // set up vertex arrays
    glBindVertexArray( vao );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0,  (void*)(sizeof(point4)*Index));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0,  (void*)((sizeof(point4)*Index)+(sizeof(point4)*Index)));
    glEnableVertexAttribArray(2);

}


/**
 * Pintat en la GPU.
 * @brief Object::draw
 */
void Object::draw(){
    glBindVertexArray( vao );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Crida per passar el material a la GPU
    this->material->toGPU(program);

    // Passem la textura a la GPU
    toGPUTexture(program);
    drawTexture();

    // Passem el centre de l'objecte a la GPU
    vec4 center = getCenter();
    GLuint glCenter = program->uniformLocation("center");
    glUniform4fv(glCenter, 1, center);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glDisable(GL_TEXTURE_2D);

}

/**
 * @brief Object::make
 */
void Object::make(){

    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU

    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            normals[Index] = normalsVertexs[cares[i].idxVertices[j]];
            if(textVertexs.size() > 0) {
                coordText[Index] = textVertexs[cares[i].idxTextures[j]];
            }
            Index++;
        }
    }

    // Inicialització de la textura
    initTexture();
}




/**
 * @brief Object::toGPUTexture
 * @param pr
 */
void Object::toGPUTexture(shared_ptr<QGLShaderProgram> pr) {
    program = pr;

// TO DO: Cal implementar en la fase 1 de la practica 2
// S'ha d'activar la textura i es passa a la GPU

    texture->bind(0);
    program->setUniformValue("textureMap", 0);

    GLuint isTextDirect = program->uniformLocation("isTextMapDirect");
    GLuint isTextureActivated = program->uniformLocation("isTextureActivated");
    glUniform1i(isTextDirect, this->isTextMapDirect);
    glUniform1i(isTextureActivated, this->isTextureActivated);
}


/**
 * Pintat en la GPU.
 * @brief Object::drawTexture
 */
void Object::drawTexture(){

    // TO DO: Cal implementar en la fase 1 de la practica 2
    // S'ha d'activar la textura i es passa a la GPU
    glEnable(GL_TEXTURE_2D);

}

void Object::setTexture(shared_ptr<QOpenGLTexture> t){
   texture = t;
}

/**
 * @brief Object::initTexture
 */
void Object::initTexture()
 {
    // TO DO: A implementar a la fase 1 de la practica 2
    // Cal inicialitzar la textura de l'objecte: veure l'exemple del CubGPUTextura
    qDebug() << "Initializing textures...";

    // Carregar la textura
    glActiveTexture(GL_TEXTURE0);
    // En el cas que fem mapeig directe pero no tinguem vertexos, no activarem la textura
    this->isTextureActivated = false;
    if (texture == nullptr) {
        // Posem una textura per defecte
        texture = make_shared<QOpenGLTexture>(QImage("://resources/textures/bricks.png").mirrored());
    }
    if (textVertexs.size() > 0 || !isTextMapDirect)
        this->isTextureActivated = true;
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
 }

/**
 * @brief Object::setMaterial
 */
void Object::setMaterial(shared_ptr<Material> mat) {
    this->material = mat;
}


void Object::parseObjFile(const QString &fileName)
{
    QFile file(fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            QVector<QVector3D> v, vn;
            QVector<QVector2D> vt;

            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        qDebug() << line.remove(0, 1).trimmed();
                    }

                    // if it’s a vertex position (v)
                    else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
                    {
                        vertexs.push_back(point4(lineParts.at(1).toFloat(),
                                           lineParts.at(2).toFloat(),
                                           lineParts.at(3).toFloat(), 1.0f));
                    }

                    // if it’s a normal (vn)
                    else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                    {
                        normalsVertexs.push_back (point4(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat(),
                                            lineParts.at(3).toFloat(), 0.0f));
                    }

                    // if it’s a texture (vt)
                    else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                    {
                        textVertexs.push_back(vec2(lineParts.at(1).toFloat(),
                                            lineParts.at(2).toFloat()));
                    }

                    // if it’s face data (f)
                    // there’s an assumption here that faces are all triangles
                    else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
                    {
                        Cara *cara = new Cara();

                        // get points from v array
                        cara->idxVertices.push_back(lineParts.at(1).split("/").at(0).toInt() - 1);
                        cara->idxVertices.push_back(lineParts.at(2).split("/").at(0).toInt() - 1);
                        cara->idxVertices.push_back(lineParts.at(3).split("/").at(0).toInt() - 1);

                        if(textVertexs.size() > 0) // check if really there are any UV coords
                        {
                            cara->idxTextures.push_back( lineParts.at(1).split("/").at(1).toInt() - 1);
                            cara->idxTextures.push_back( lineParts.at(2).split("/").at(1).toInt() - 1);
                            cara->idxTextures.push_back( lineParts.at(3).split("/").at(1).toInt() - 1);
                        }

                        // get normals from vn array
                        cara->idxNormals.push_back( lineParts.at(1).split("/").at(2).toInt() - 1);
                        cara->idxNormals.push_back( lineParts.at(2).split("/").at(2).toInt() - 1);
                        cara->idxNormals.push_back( lineParts.at(3).split("/").at(2).toInt() - 1);


                        // cara->calculaNormal();
                        cares.push_back(*cara);
                    }

                    // if it's a material file
                    else if(lineParts.at(0).compare("mtllib", Qt::CaseInsensitive) == 0)
                    {
                        QString materialFile = lineParts.at(1);
                        parseMaterialFile(materialFile);
                    }

                }
            }

            file.close();
        }
    }
}

void Object::parseMaterialFile(const QString &fileName) {
    QString folder = "://resources/models/";
    QFile file(folder+fileName);
    if(file.exists()) {
        if(file.open(QFile::ReadOnly | QFile::Text)) {
            vec3 Ka, Kd, Ks;
            float shininess;

            while(!file.atEnd()) {
                QString line = file.readLine().trimmed();
                QStringList lineParts = line.split(QRegularExpression("\\s+"));
                if(lineParts.count() > 0) {
                    // if it’s a comment
                    if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
                    {
                        qDebug() << line.remove(0, 1).trimmed();
                    }

                    // if it’s Ka
                    else if(lineParts.at(0).compare("Ka", Qt::CaseInsensitive) == 0)
                    {
                        Ka = vec3(lineParts.at(1).toFloat(), lineParts.at(2).toFloat(), lineParts.at(3).toFloat());
                    }

                    // if it’s Kd
                    else if(lineParts.at(0).compare("Kd", Qt::CaseInsensitive) == 0)
                    {
                        Kd = vec3(lineParts.at(1).toFloat(), lineParts.at(2).toFloat(), lineParts.at(3).toFloat());
                    }

                    // if it’s Ks
                    else if(lineParts.at(0).compare("Ks", Qt::CaseInsensitive) == 0)
                    {
                        Ks = vec3(lineParts.at(1).toFloat(), lineParts.at(2).toFloat(), lineParts.at(3).toFloat());
                    }

                    // if it’s Ns
                    else if(lineParts.at(0).compare("Ns", Qt::CaseInsensitive) == 0)
                    {
                        shininess = lineParts.at(1).toFloat();
                    }

                }
            }
            this->setMaterial(make_shared<Material>(Ka, Kd, Ks, shininess));

            file.close();
        }
    }
}

Capsa3D Object::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
    for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}

void Object::aplicaTG(shared_ptr<TG> tg) {
    if (dynamic_pointer_cast<TranslateTG>(tg)) {
        for(int i = 0; i < this->vertexs.size(); ++i) {
            this->vertexs[i] = tg->getTG() * this->vertexs[i];
        }
   }
   else if (dynamic_pointer_cast<ScaleTG>(tg)) {
        for(int i = 0; i < this->vertexs.size(); ++i) {
            this->vertexs[i] = tg->getTG() * this->vertexs[i];
        }
    }
    make();
}

void Object::makeUntari() {
    Capsa3D c = calculCapsa3D();
    float escalat_unitari = 1.0f/ max({c.a, c.h, c.p});

    shared_ptr<ScaleTG> scale_unit = make_shared<ScaleTG>(escalat_unitari);
    aplicaTG(scale_unit);

    c = calculCapsa3D();
    vec3 centre;
    centre.x = c.pmin.x + c.a/2.0f;
    centre.y = c.pmin.y + c.h/2.0f;
    centre.z = c.pmin.z + c.p/2.0f;
    shared_ptr<TranslateTG> trans_origen = make_shared<TranslateTG>(-centre);
    aplicaTG(trans_origen);
}

vec4 Object::getCenter() {
    Capsa3D c = calculCapsa3D();
    vec4 centre = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    centre.x = c.pmin.x + c.a/2.0f;
    centre.y = c.pmin.y + c.h/2.0f;
    centre.z = c.pmin.z + c.p/2.0f;
    return centre;
}

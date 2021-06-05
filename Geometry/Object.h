#ifndef Object_H
#define Object_H

#include <QObject>
#include <vector>
#include <memory>

#include <library/Common.h>
using namespace Common;

#include <QGLShaderProgram>
#include <QOpenGLTexture>

#include <QFile>
#include <QRegularExpression>
#include <QDebug>
#include <QString>
#include <QStringList>

#include <Geometry/TG.h>
#include <Geometry/Animation.h>
#include <Geometry/Cara.h>
#include <Geometry/Material.h>
#include <Geometry/TranslateTG.h>
#include <Geometry/ScaleTG.h>

typedef vec4  point4;

// Classe que conte la representacio d'un Objecte: geometria, topologia, material i textures
class Object : public QObject, public Animable {

    Q_OBJECT
protected:
    QString nom;
    vector<Cara> cares;
    vector<point4> vertexs;
    vector<point4> normalsVertexs;
    vector<vec2>   textVertexs;

    // Estructures per passar a la GPU
    GLuint buffer;
    GLuint vao;

    int     numPoints;
    point4 *points;
    point4 *normals;
    vec2 *coordText;

    // Els colors s'usen en la primera execució però després son prescindibles
    // point4 *colors;
    // En canvi, ara utilitzem la classe Material
    shared_ptr<Material> material;

    int Index; // index de control del numero de vertexs a passar a la GPU

    shared_ptr<QGLShaderProgram> program;

    shared_ptr<QOpenGLTexture> texture;
    bool isTextMapDirect;
    bool isTextureActivated;

public:

    Object(const int npoints, QObject *parent = 0);
    Object(int npoints, shared_ptr<Material> mat, QObject *parent = 0);
    Object(int npoints, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect, QObject *parent = 0);
    Object(const int npoints, QString n, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool unitari = false, bool isTextMapDirect = true);
    Object(const int npoints, QString n, shared_ptr<QOpenGLTexture> texture, bool unitari = false, bool isTextMapDirect = true);
    ~Object();

    void parseObjFile(const QString &fileName);
    void parseMaterialFile(const QString &fileName);

    void setTexture(shared_ptr<QOpenGLTexture> t);
    void setMaterial(shared_ptr<Material> mat);
    vec4 getCenter();

    virtual void make();
    virtual void toGPU(shared_ptr<QGLShaderProgram> p);
    virtual void toGPUTexture(shared_ptr<QGLShaderProgram> pr);

    virtual void draw();
    virtual void drawTexture();

    Capsa3D calculCapsa3D();

    virtual void aplicaTG(shared_ptr<TG> tg);



private:
    void initTexture();
    void makeUntari();
};



#endif // Object_H

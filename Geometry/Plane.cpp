#include "Plane.h"

Plane::Plane(vec3 normal, float d, float xmin, float xmax, float zmin, float zmax, shared_ptr<Material> mat, shared_ptr<QOpenGLTexture> texture, bool isTextMapDirect): Object(12, mat, texture, isTextMapDirect)
{
    point4 bottom_left = point4(xmin, getCoordY(normal, d, xmin, zmax), zmax, 1.0f);
    point4 bottom_right = point4(xmax, getCoordY(normal, d, xmax, zmax), zmax, 1.0f);
    point4 top_left = point4(xmin, getCoordY(normal, d, xmin, zmin), zmin, 1.0f);
    point4 top_right = point4(xmax, getCoordY(normal, d, xmax, zmin), zmin, 1.0f);

    vertexs.push_back(bottom_left);
    vertexs.push_back(top_left);
    vertexs.push_back(top_right);
    vertexs.push_back(bottom_right);
    // Repetim els vertex per fer la cara al reves i poder assignar-li
    // la normal corresponent
    vertexs.push_back(bottom_left);
    vertexs.push_back(top_left);
    vertexs.push_back(top_right);
    vertexs.push_back(bottom_right);

    normalsVertexs.push_back(point4(normal, 0.0f));
    normalsVertexs.push_back(point4(normal, 0.0f));
    normalsVertexs.push_back(point4(normal, 0.0f));
    normalsVertexs.push_back(point4(normal, 0.0f));

    normalsVertexs.push_back(point4(-normal, 0.0f));
    normalsVertexs.push_back(point4(-normal, 0.0f));
    normalsVertexs.push_back(point4(-normal, 0.0f));
    normalsVertexs.push_back(point4(-normal, 0.0f));

    textVertexs.push_back(vec2((bottom_left.x - xmin)/(2*xmax), (bottom_left.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((top_left.x - xmin)/(2*xmax), (top_left.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((top_right.x - xmin)/(2*xmax), (top_right.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((bottom_right.x - xmin)/(2*xmax), (bottom_right.z - zmin)/(2*zmax)));

    textVertexs.push_back(vec2((bottom_left.x - xmin)/(2*xmax), (bottom_left.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((top_left.x - xmin)/(2*xmax), (top_left.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((top_right.x - xmin)/(2*xmax), (top_right.z - zmin)/(2*zmax)));
    textVertexs.push_back(vec2((bottom_right.x - xmin)/(2*xmax), (bottom_right.z - zmin)/(2*zmax)));

    Cara *cara = new Cara(0,2,1);
    Cara *cara2 = new Cara(0,3,2);
    //Posem les normals a la cara
    cara->idxNormals.push_back(0); cara->idxNormals.push_back(2); cara->idxNormals.push_back(1);
    cara2->idxNormals.push_back(0); cara2->idxNormals.push_back(3); cara2->idxNormals.push_back(2);
    //Posar textures:
    cara->idxTextures.push_back(0);
    cara->idxTextures.push_back(2);
    cara->idxTextures.push_back(1);
    cara2->idxTextures.push_back(0);
    cara2->idxTextures.push_back(3);
    cara2->idxTextures.push_back(2);
    cares.push_back(*cara);
    cares.push_back(*cara2);
    // Posem tambe les cares invertides perque el pla es vegi pels 2 costats
    Cara *cara_reverse = new Cara(4,5,6);
    Cara * cara_reverse2 = new Cara(4,6,7);
    //Posar textures:
    cara_reverse->idxTextures.push_back(4);
    cara_reverse->idxTextures.push_back(5);
    cara_reverse->idxTextures.push_back(6);
    cara_reverse2->idxTextures.push_back(4);
    cara_reverse2->idxTextures.push_back(6);
    cara_reverse2->idxTextures.push_back(7);
    cares.push_back(*cara_reverse);
    cares.push_back(*cara_reverse2);

    make();
}
float Plane::getCoordY(vec3 normal, float d, float x, float z) {
    /* Equacio del pla: normal.x*x + normal.y*y + normal.z*z + d = 0*/
    return (-d - normal.x*x - normal.z*z) / normal.y;
}

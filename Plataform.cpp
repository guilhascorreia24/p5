#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include "Object.h"
#include "Plataform.h"
#include <vector>
#include <set>
#include <algorithm>
#include <math.h>
using namespace std;

Plataform::Plataform(const char *c)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(0.2, 0.2, 0.2);
    setBlocks();
}

void Plataform::setBlocks()
{
    for (struct Faces f : faces)
    {
        superiorFaces(f);
    }
    cout << blocks.size() << std::endl;
}

void Plataform::superiorFaces(struct Faces face)
{
    float xmax = INT_MIN, zmax = INT_MIN, xmin = INT_MAX, zmin = INT_MAX;
    for (int i = 0; i < 3; i++)
    {
        if (face.v[i].y < 0)
        {
            return;
        }
        if (face.v[i].x > xmax)
            xmax = face.v[i].x;
        if (face.v[i].z > zmax)
            zmax = face.v[i].z;
        if (face.v[i].x < xmin)
            xmin = face.v[i].x;
        if (face.v[i].z < zmin)
            zmin = face.v[i].z;
    }
    struct Vertex vmax;
    vmax.x = xmax;
    vmax.y = max.y;
    vmax.z = zmax;
    struct Vertex vmin;
    vmin.x = xmin;
    vmin.y = min.y;
    vmin.z = zmin;
    struct Vertex m;
    //cout<<vmin.tostring()<<"&"<<vmax.tostring()<<std::endl;
    m.x = (xmax + xmin) / 2;
    m.y = (max.y + min.y) / 2;
    m.z = (zmax + zmin) / 2;
    Block b(m, vmin, vmax);
    if (find(blocks.begin(), blocks.end(), b) == blocks.end())
        blocks.push_back(b);
}
double Plataform::distanceOther(glm::mat4 d)
{
    return sqrt(pow(MVP[3][0] - d[3][0], 2) + pow(MVP[3][1] - d[3][1], 2) + pow(MVP[3][2] - d[3][2], 2));
}
bool Plataform::overBlock(Block b){
    for(Block b2: blocks){
        if(b.getcenter().x>=b2.min.x && b.getcenter().x<=b2.max.x && b.getcenter().z>=b2.min.z && b.getcenter().z<=b2.max.z && b.getcenter().y-b2.getcenter().y>=(b.height/2)+(b2.height/2)){
            return true;
        }
    }
    return false;
}


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

Plataform::Plataform(const char *c,const char *t)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(t);
    setBlocks();
}

void Plataform::setBlocks()
{
    for (struct Faces f : this->faces)
    {
        superiorFaces(f);
    }
    findFinal();
    //cout << blocks.size() << std::endl;
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
bool Plataform::overBlock(Block b)
{
    for (Block b2 : blocks)
    {
        if (b.getcenter().x >= b2.min.x && b.getcenter().x <= b2.max.x && b.getcenter().z >= b2.min.z && b.getcenter().z <= b2.max.z && b.getcenter().y - b2.getcenter().y >= (b.height / 2) + (b2.height / 2))
        {
            return true;
        }
    }
    return false;
}
void Plataform::findFinal()
{
    //printf("findfinal\n");
    int r = 1000000;

    for (float i = min.x + blocks.at(0).length / 2; i < max.x; i += round(blocks.at(0).length * r) / r)
    {
        for (float j = min.z + blocks.at(0).width / 2; j < max.z; j += round(blocks.at(0).width * r) / r)
        {
            struct Vertex v, v2;
            v.x = round(i * r) / r;
            v.z = round(j * r) / r;
            v.y = (max.y + min.y) / 2;
            Block b = Block(v, v, v);
            v2 = v;
            v2.z -= blocks.at(0).width;
            Block n = Block(v2, v2, v2);
            v2 = v;
            v2.z += blocks.at(0).width;
            Block s = Block(v2, v2, v2);
            v2 = v;
            v2.x += blocks.at(0).width;
            Block e = Block(v2, v2, v2);
            v2 = v;
            v2.x -= blocks.at(0).width;
            Block o = Block(v2, v2, v2);
            //cout<<b.tostring()<<std::endl;
            /*cout<<n.tostring()<<std::endl;
            cout<<s.tostring()<<std::endl;
            cout<<e.tostring()<<std::endl;
            cout<<o.tostring()<<std::endl;*/
            //cout<<(bool)(find(blocks.begin(), blocks.end(), b) != blocks.end())<<std::endl;
            if (find(blocks.begin(), blocks.end(), b) == blocks.end() && find(blocks.begin(), blocks.end(), n) != blocks.end() &&
                find(blocks.begin(), blocks.end(), s) != blocks.end() && find(blocks.begin(), blocks.end(), e) != blocks.end() &&
                find(blocks.begin(), blocks.end(), o) != blocks.end())
            {
                final_ = b;
                final_.atual[0]=round((final_.atual[0]) * r) / r;
                final_.atual[1]=round((final_.atual[1]) * r) / r;
                final_.atual[2]=round((final_.atual[2]) * r) / r;
                final_.max.x = round((final_.atual[0] + blocks.at(0).length / 2) * r) / r;
                final_.max.z = round((final_.atual[2] + blocks.at(0).width / 2) * r) / r;
                final_.max.y = round((final_.atual[1] + blocks.at(0).height / 2) * r) / r;
                final_.min.x = round((final_.atual[0] - blocks.at(0).length / 2) * r) / r;
                final_.min.z = round((final_.atual[2] - blocks.at(0).width / 2) * r) / r;
                final_.min.y = round((final_.atual[1] - blocks.at(0).height / 2) * r) / r;
            }
        }
    }
    //cout << final_.tostring() << std::endl;
}

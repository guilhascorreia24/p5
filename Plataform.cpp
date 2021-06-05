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

Plataform::Plataform(const char *c, const char *t)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(t);
    setBlocks();
    Model=glm::mat4(1);
}
void Plataform::setLavaBlocks(Block b){
    glm::vec3 t[7];
    t[0]=glm::vec3(-2.3833,0,-1.3062);
    t[1]=glm::vec3(-4.8184,0,-4.8996); //posiçoes do blocos no nivel 2
    t[2]=glm::vec3(-6.0107,0,-0.083595);
    t[3]=glm::vec3(-1.18759,0,2.29437);
    t[4]=glm::vec3(2.39821,0,-1.30617);
    t[5]=glm::vec3(6.01086,0,-2.50065);
    t[6]=glm::vec3(2.4165,0,5.91033);
    for(int i=0;i<7;i++){
        this->lavaBlocks.push_back(b);
        this->lavaBlocks.at(i).atual=this->lavaBlocks.at(i).inicial_pos=t[i];
    }
}

void Plataform::setBlocks()
{
    for (struct Faces f : this->faces)
    {
        superiorFaces(f);
    }
    findFinal();
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

bool Plataform::overLavaBlocks(Block b){

    for (Block b2 : lavaBlocks)
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
    int r = 1000000;
    for (float i = min.x + blocks.at(0).length/2; i < max.x; i +=blocks.at(0).length )
    {
        for (float j = min.z + blocks.at(0).width/2; j < max.z; j += blocks.at(0).width )
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
            if (!CheckIsBlock(b) && CheckIsBlock(n) && CheckIsBlock(s) && CheckIsBlock(e) && CheckIsBlock(o))
            {
                final_ = b;
                final_.atual[0] = round((final_.atual[0]) * r) / r;
                final_.atual[1] = round((final_.atual[1]) * r) / r;
                final_.atual[2] = round((final_.atual[2]) * r) / r;
                final_.max.x = round((final_.atual[0] + blocks.at(0).length / 2) * r) / r;
                final_.max.z = round((final_.atual[2] + blocks.at(0).width / 2) * r) / r;
                final_.max.y = round((final_.atual[1] + blocks.at(0).height / 2) * r) / r;
                final_.min.x = round((final_.atual[0] - blocks.at(0).length / 2) * r) / r;
                final_.min.z = round((final_.atual[2] - blocks.at(0).width / 2) * r) / r;
                final_.min.y = round((final_.atual[1] - blocks.at(0).height / 2) * r) / r;
            }
        }
    }
}
bool Plataform::CheckIsBlock(Block o){
    for(Block b:blocks){
        if(o.atual[0]<=b.max.x && o.atual[0]>=b.min.x && o.atual[2]<=b.max.z && o.atual[2]>=b.min.z && o.atual[1]>=b.min.y && o.atual[1]<=b.max.y){
            return true;
        }
    }
    return false;
}

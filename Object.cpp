#include <iostream>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include "Object.h"
#include "Plataform.h"
#include "Scenery.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <limits>
#include <math.h>
#include <algorithm>
using namespace std;
//std::vector<int> indexes; //indice dos vertices

struct Vertex Object::getvertex(string s, string del) //parse de uma coordenada
{
    int start = 0;
    int end = s.find(del);
    float *coord = (float *)malloc(3 * sizeof(float));
    int i = 0;
    start = end + del.size();
    end = s.find(del, start);
    while (end != -1)
    {
        coord[i++] = stof((string)s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    coord[i++] = std::stof(s.substr(start, end - start));
    struct Vertex v;
    v.x = coord[0];
    v.y = coord[1];
    v.z = coord[2];
    free(coord);
    return v;
}

void Object::getfaces(string s, string del) //parse de um(a) triangulo/face de um objecto
{
    int start = 0;
    int end = s.find(del);
    start = end + del.size();
    end = s.find(del, start);
    while (end != -1)
    {
        string k = s.substr(start, end - start);
        this->indexes.push_back(stoi(k.substr(0, k.find("/"))));
        start = end + del.size();
        end = s.find(del, start);
    }
    string k = s.substr(start, end - start);
    this->indexes.push_back(stoi(k.substr(0, k.find("/"))));
    //cout << indexes.size() << std::endl;
}
void Object::setVertexes(const char *c, struct Vertex *v, struct Faces *f) // obter todos os triangulos de uma figura
{
    ifstream fp(c);
    if (!fp.is_open())
    {
        perror("Failed: ");
        cout << f;
        exit(1);
    }
    else
    {
        int i = 0;
        string line;
        int f = 0;
        while (getline(fp, line))
        {
            if (line.substr(0, line.find(" ")).compare("v") == 0)
            {
                v[i] = this->getvertex(line, " ");
                i++;
            }
            if (line.substr(0, line.find(" ")).compare("f") == 0)
            {
                this->getfaces(line, " ");
                f++;
            }
        }
        fp.close();
        std::set<float> Y, X, Z;
        this->vertex = (struct Vertex *)malloc((int)this->indexes.size() * sizeof(struct Vertex));
        for (int i = 0; i < indexes.size(); i += 3)
        {
            this->vertex[i] = v[this->indexes.at(i) - 1];
            this->vertex[i + 1] = v[this->indexes.at(i + 1) - 1];
            this->vertex[i + 2] = v[this->indexes.at(i + 2) - 1];
            struct Faces f;
            f.v[0] = this->vertex[i];
            f.v[1] = this->vertex[i + 1];
            f.v[2] = this->vertex[i + 2];
            faces.push_back(f);
            Y.insert(vertex[i].y);
            X.insert(vertex[i].x);
            Z.insert(vertex[i].z);
        }
        max.x = *(X.rbegin());
        max.y = *(Y.rbegin());
        max.z = *(Z.rbegin());
        min.x = *X.begin();
        min.y = *Y.begin();
        min.z = *Z.begin();
        length = max.x - min.x;
        width = max.z - min.z;
        height = max.y - min.y;
        this->n_vertexes = this->indexes.size();
    }
}
Object::Object()
{
    //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    //glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    MVP = Projection * View;
}

void Object::setTexture(float r, float g, float b)
{
    this->colors = (float *)malloc(this->n_vertexes * 3 * sizeof(float));
    int c = 0;
    for (int i = 0; i < this->n_vertexes; i++)
    {
        this->colors[c++] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        this->colors[c++] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        this->colors[c++] = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
}
/*bool Object::Collisions(std::vector<Object> objs)
{
    bool collide = false;
    for (Object o : objs)
    {
        //cout<<o.min.x<<";"<<o.min.z<<" "<<o.max.x<<";"<<o.max.z<<std::endl;
        if (atual[0] >o.min.x && atual[0] <o.max.x && atual[2] >o.min.z && atual[2] <o.max.z )
        {
            if (atual[1] - o.atual[1] < (o.height / 2) + (height / 2) && atual[1] - o.atual[1] > (o.height / 2) + (height / 2)-0.5 )
                return true;
        }
    }
    return collide;
}*/

bool Object::equals(Object b)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (MVP[i][j] != b.MVP[i][j])
                return false;
        }
    }
    if (faces.size() != b.faces.size() && n_vertexes != b.n_vertexes)
        return false;
    return true;
}
double Object::distanceObjects(Object *b)
{
    return sqrt(pow(atual[0] - b->atual[0], 2) + pow(atual[1] - b->atual[1], 2) + pow(atual[2] - b->atual[2], 2));
}

struct Vertex Object::getcenter()
{
    struct Vertex v;
    v.x = MVP[3][0];
    v.y = MVP[3][1];
    v.z = MVP[3][2];
    return v;
}

string Object::tostring(){
    return to_string(atual[0])+" "+ to_string(atual[1])+" " +to_string(atual[2])+"\n";
}

bool Object::Collisions(std::vector<Object> objs)
{
    bool collide = false;
    for (Object o : objs)
    {
        //cout<<o.min.x<<";"<<o.min.z<<" "<<o.max.x<<";"<<o.max.z<<std::endl;
        //cout<<atual[0]<<";"<<atual[2]<<std::endl;
        if (atual[0] > o.min.x && atual[0] < o.max.x && atual[2] > o.min.z && atual[2] < o.max.z)
        {
            if ((abs(atual[1] - o.atual[1]) < (o.height / 2) + (height / 2)))
            {
                //printf("colide up\n");
                return true;
            }
            if (atual[1] > o.min.y && atual[1] < o.max.y)
            {
                if ((abs(atual[0] - o.atual[0]) < (o.length / 2) + (length / 2)))
                {
                    //printf("colide fesquerda/direta\n");
                    return true;
                }
                if ((abs(atual[2] - o.atual[2]) < (o.width / 2) + (width / 2)))
                {
                    //printf("colide frente/atras\n");
                    return true;
                }
            }
        }
    }
    return collide;
}
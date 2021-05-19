#include <iostream>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include "Object.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <limits>
#include <math.h>
#include <algorithm>
using namespace std;
//std::vector<int> indexes; //indice dos vertices

struct Vertex Object::getvertex(string s, string del = " ") //parse de uma coordenada
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

void Object::setBodyCollider(){

}

void Object::setVertexes(const char *f, struct Vertex *v) // obter todos os triangulos de uma figura
{
    ifstream fp(f);
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
            }
        }
        fp.close();
        this->vertex = (struct Vertex *)malloc((int)this->indexes.size() * sizeof(struct Vertex));
        for (int i = 0; i < indexes.size(); i++)
        {
            this->vertex[i] = v[this->indexes.at(i) - 1];
            Y.insert(vertex[i].y);
            X.insert(vertex[i].x);
            Z.insert(vertex[i].z);
        }
        this->n_vertexes = this->indexes.size();
        cout << X.size() << " " << Y.size() << " " << Z.size() << std::endl;
    }
}

struct Vertex Object::getCenterObject()
{
    struct Vertex c;
    c.x = this->MVP[3][0];
    c.y = this->MVP[3][1];
    c.z = this->MVP[3][2];
    return c;
}

void Object::setTexture(float r, float g, float b)
{
    this->colors = (float *)malloc(this->n_vertexes * 3 * sizeof(float));
    int c = 0;
    for (int i = 0; i < this->n_vertexes; i++)
    {
        this->colors[c++] = r;
        this->colors[c++] = g;
        this->colors[c++] = b;
    }
}

string Object::ToString()
{
    string res = to_string(this->MVP[3][0]) + " " + to_string(this->MVP[3][1]) + " " + to_string(this->MVP[3][2]);
    return res + "\n";
}

bool Object::checkCollide(Object o)
{
    return false;
}

void Object::setBodyCollider(){
    
}

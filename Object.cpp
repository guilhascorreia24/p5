#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include "Object.h"
#include <stdlib.h>
#include <vector>
std::vector<int> indexes; //indice dos vertices


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
        indexes.push_back(stoi(k.substr(0, k.find("/"))));
        start = end + del.size();
        end = s.find(del, start);
    }
    string k = s.substr(start, end - start);
    indexes.push_back(stoi(k.substr(0, k.find("/"))));
    //cout << indexes.size() << std::endl;
}

void Object::setVertexes(const char *f) // obter todos os triangulos de uma figura
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
        struct Vertex *v = (struct Vertex *)malloc(10000 * sizeof(struct Vertex));
        int i = 0;
        string line;
        while (getline(fp, line))
        {
            if (line.substr(0, line.find(" ")).compare("v") == 0)
            {
                v[i++] = this->getvertex(line, " ");
            }
            if (line.substr(0, line.find(" ")).compare("f") == 0)
            {
                this->getfaces(line, " ");
            }
        }
        fp.close();
        cout << indexes.size() << std::endl;
        this->vertices = (struct Vertex *)malloc( (int)indexes.size() * sizeof(struct Vertex));
        for (int i = 0; i < indexes.size(); i++)
        {
            this->vertices[i]=v[indexes.at(i)-1];
        }
        this->n_vertexes=indexes.size();
        
    }
}

struct Vertex Object::getCenterObject(){
    struct Vertex c;
    c.x=this->MVP[3][0];
    c.y=this->MVP[3][1];
    c.z=this->MVP[3][2];
}
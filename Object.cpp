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
            if (line.substr(0, line.find(" ")).compare("mtllib") == 0)
            {
                string del = " ";
                int start = 0;
                int end = line.find(del);
                start = end + del.size();
                end = line.find(del, start);
                s = line.substr(start, end - start);
                cout << s << std::endl;
            }
        }
        fp.close();
        std::set<float> Y, X, Z;
        this->vertex = (struct Vertex *)malloc((int)this->indexes.size() * sizeof(struct Vertex));
        for (int i = 0; i < indexes.size(); i++)
        {
            this->vertex[i] = v[this->indexes.at(i) - 1];
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
        this->n_vertexes = this->indexes.size();
        //cout << X.size() << " " << Y.size() << " " << Z.size() << std::endl;
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

bool Object::checkCollide(std::vector<Object> l)
{
    sort(this->bodyCollider.begin(), this->bodyCollider.end());
    for (Object o : l)
    {
        if (o.s != s)
        {
            cout << o.MVP[3][1] << MVP[3][1]<<std::endl;
            cout << o.max.y << " "<<min.y << std::endl;
            // exit(1);
            if (min.y-o.max.y < 0)
            {
                return true;
            }
        }
    }
    return false;
}

void Object::setBodyCollider()
{
    bodyCollider.push_back(max);
    bodyCollider.push_back(min);
    struct Vertex p[6];
    p[0].x = max.x;
    p[0].y = max.y;
    p[0].z = min.z;
    p[1].x = max.x;
    p[1].y = min.y;
    p[1].z = min.z;
    p[2].x = min.x;
    p[2].y = min.y;
    p[2].z = max.z;
    p[3].x = min.x;
    p[3].y = max.y;
    p[3].z = max.z;
    p[4].x = min.x;
    p[4].y = max.y;
    p[4].z = min.z;
    p[5].x = max.x;
    p[5].y = min.y;
    p[5].z = max.z;
    for (int i = 0; i < 6; i++)
    {
        bodyCollider.push_back(p[i]);
    }
}

void Object::Translation(glm::mat4 T)
{
    cout << T << std::endl;
    MVP = MVP + T;
    for (struct Vertex v : bodyCollider)
    {
        v.x+=T[3][0];
        v.y+=T[3][1];
        v.z+=T[3][2];
    }
    max.x+=T[3][0];max.y+=T[3][1];max.z+=T[3][2];
    min.x+=T[3][0];min.y+=T[3][1];min.z+=T[3][2];

}

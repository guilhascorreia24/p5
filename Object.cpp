#include <iostream>
#include <string>
using namespace std;
#include <fstream>
#include "Object.h"
#include <stdlib.h>
#include <vector>
std::vector<int> indexes;
struct Vertex Object::getvertex(string s, string del = " ")
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

void Object::getfaces(string s, string del)
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

void Object::setVertexes(const char *f)
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
                //cout << v[i - 1].x << " " << v[i - 1].y << " " << v[i - 1].z << std::endl;
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
            //cout << indexes.at(i)-1 << std::endl;
            //cout << v[indexes.at(i)-1].x << " " << v[indexes.at(i)-1].y << " " << vf[indexes.at(i)-1].z << std::endl;
            this->vertices[i]=v[indexes.at(i)-1];
            //cout << vf[i].x << " " << vf[i].y << " " << vf[i].z << std::endl;
        }
        this->n_vertexes=indexes.size();
        
    }
}
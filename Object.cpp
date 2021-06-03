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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <windows.h>
using namespace std;
//std::vector<int> indexes; //indice dos vertices

struct Vertex Object::getvertex(string s, string del) //parse de uma coordenada
{
    int start = 0;
    int end = s.find(del);
    float *coord = (float *)calloc(3, 3 * sizeof(float));
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
        int start_sub = 0;
        int end_sub = k.find("/");
        this->indexes_v.push_back(stoi(k.substr(0, end_sub)));
        start_sub = end_sub + string("/").size();
        end_sub = k.find("/", start_sub);
        string p = k.substr(start_sub, end_sub - start_sub);
        this->indexes_vt.push_back(stoi(k.substr(start_sub, end_sub - start_sub)));
        start_sub = end_sub + string("/").size();
        end_sub = k.find("/", start_sub);
         p = k.substr(start_sub, end_sub - start_sub);
        this->indexes_vn.push_back(stoi(k.substr(start_sub, end_sub - start_sub)));
        start = end + del.size();
        end = s.find(del, start);
        //cout<<k<<std::endl;
        // cout<<p<<std::endl;
    }
    string k = s.substr(start, end - start);
    int start_sub = 0;
    int end_sub = k.find("/");
    this->indexes_v.push_back(stoi(k.substr(0, end_sub)));
    start_sub = end_sub + string("/").size();
    end_sub = k.find("/", start_sub);
    this->indexes_vt.push_back(stoi(k.substr(start_sub, end_sub - start_sub)));
    start_sub = end_sub + string("/").size();
    end_sub = k.find("/", start_sub);
    string p = k.substr(start_sub, end_sub - start_sub);
    this->indexes_vn.push_back(stoi(k.substr(start_sub, end_sub - start_sub)));
    //cout<<k<<" "<<p<<std::endl;
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
        std::vector<struct Texture> texture;
        std::vector<struct Vertex> vn;

        int i = 0;
        string line;
        int f = 0, c = 0;
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
            if (line.substr(0, line.find(" ")).compare("vt") == 0)
            {
                struct Vertex text = this->getvertex(line, " ");
                texture.push_back(Texture(text.x, text.y));
            }
            if (line.substr(0, line.find(" ")).compare("vn") == 0)
            {
                vn.push_back(this->getvertex(line, " "));
            }
        }
        fp.close();
        std::set<float> Y, X, Z;
        this->vertex = (struct VertexColorTexture *)malloc((int)this->indexes_v.size() * sizeof(struct VertexColorTexture));
        for (int i = 0; i < indexes_v.size(); i += 3)
        {

            this->vertex[i] = VertexColorTexture(v[this->indexes_v.at(i) - 1], Vertex(1, 0, 0), Texture(texture.at(this->indexes_vt.at(i) - 1).y, texture.at(this->indexes_vt.at(i) - 1).x), vn.at(this->indexes_vn.at(i) - 1));
            this->vertex[i + 1] = VertexColorTexture(v[this->indexes_v.at(i + 1) - 1], Vertex(0, 1, 0), Texture(texture.at(this->indexes_vt.at(i + 1) - 1).y, texture.at(this->indexes_vt.at(i + 1) - 1).x),vn.at(this->indexes_vn.at(i+1)-1));
            this->vertex[i + 2] = VertexColorTexture(v[this->indexes_v.at(i + 2) - 1], Vertex(0, 0, 1), Texture(texture.at(this->indexes_vt.at(i + 2) - 1).y, texture.at(this->indexes_vt.at(i + 2) - 1).x),vn.at(this->indexes_vn.at(i+2)-1));
            struct Faces f;
            f.v[0] = this->vertex[i].v;
            f.v[1] = this->vertex[i + 1].v;
            f.v[2] = this->vertex[i + 2].v;
            faces.push_back(f);
            Y.insert(vertex[i].v.y);
            X.insert(vertex[i].v.x);
            Z.insert(vertex[i].v.z);
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
        this->n_vertexes = this->indexes_v.size();
    }
}
Object::Object()
{
    //glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    //glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    MVP = Scenery::Projection * Scenery::View;
}

void Object::setTexture(const char *c)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);                        // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
                                                                  // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    char fullFilename[MAX_PATH];
    GetFullPathName(c, MAX_PATH, fullFilename, nullptr);
    //cout << fullFilename << std::endl;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    data = stbi_load(fullFilename, &texture_width, &texture_height, &nrChannels, 0);
}

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
    v.x = atual[0];
    v.y = atual[1];
    v.z = atual[2];
    return v;
}

string Object::tostring()
{
    return to_string(atual[0]) + " " + to_string(atual[1]) + " " + to_string(atual[2]);
}

bool Object::Collisions(std::vector<Object> objs)
{
    bool collide = false;
    for (Object o : objs)
    {
        if (atual[0] - o.min.x > pow(10, -6) && atual[0] - o.max.x < -pow(10, -6) && atual[2] - o.min.z > pow(10, -6) && atual[2] - o.max.z < -pow(10, -6))
        {
            if ((abs(atual[1] - o.atual[1]) < (o.height / 2) + (height / 2)))
            {
                /*cout<<o.min.x<<";"<<o.min.z<<" "<<o.max.x<<";"<<o.max.z<<std::endl;
                cout<<tostring()<<std::endl;
                cout<<atual[0]-o.max.x<<std::endl;
                printf("colide up\n");*/
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
bool Object::Collide(Object o)
{
    if (atual[0] > o.min.x && atual[0] < o.max.x && atual[2] > o.min.z && atual[2] < o.max.z)
    {
        if ((abs(atual[1] - o.atual[1]) < (o.height / 2) + (height / 2)))
        {
            //cout<<o.min.x<<";"<<o.min.z<<" "<<o.max.x<<";"<<o.max.z<<std::endl;
            // cout<<tostring()<<std::endl;
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
    return false;
}
void Object::standUP()
{
    MVP = Scenery::Projection * Scenery::View * glm::translate(glm::mat4(1), atual);
    Model=glm::mat4(1);
    height = max.y - min.y;
    width = max.z - min.z;
    length = max.x - min.x;
}

void Object::loadTextures()
{
    //cout<<data<<std::endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //stbi_image_free(data);
}
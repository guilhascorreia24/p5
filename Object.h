#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include <set>
#include "Objscomps.h"

struct VertexColorTexture
{
    struct Vertex v;
    struct Vertex c;
    Texture texture;
    VertexColorTexture(struct Vertex v, struct Vertex c, Texture t)
    {
        this->c = c;
        this->v = v;
        this->texture = t;
    }
};
class Object
{
public:
    struct VertexColorTexture *vertex;
    int n_vertexes;
    unsigned char *data;
    glm::mat4 MVP;
    glm::vec3 inicial_pos, atual;
    struct Vertex max, min;
    float height, length, width;
    int texture_width, texture_height;

    Object();
    void setVertexes(const char *c, struct Vertex *v, struct Faces *f);
    void setTexture(const char *c);
    void loadTextures();
    bool equals(Object b);
    double distanceObjects(Object *b);
    struct Vertex getcenter();
    bool Collisions(std::vector<Object> o);
    string tostring();
    bool Collide(Object o);
    void standUP();

private:
    void getfaces(string s, string del);
    struct Vertex getvertex(string s, string del);
    std::vector<int> indexes_v,indexes_vt,indexes_vn;

protected:
    std::vector<struct Faces> faces;
};
#endif

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
struct Vertex
{
    float x, y, z;
    bool operator==(struct Vertex const &b) const
    {
        //cout<<b.x<< " "<<this->x<< std::endl;
        return this->x == b.x && this->y == b.y && this->z == b.z;
    }
    inline bool operator<(const Vertex &a) const
    {
        if (x < a.x)
        {
            return true;
        }
        if (x == a.x && y < a.y)
        {
            return true;
        }
        if (x == a.x && y == a.y && z < a.z)
            return true;
        return false;
    }
    string tostring()
    {
        return to_string(x) + " " + to_string(y) + " " + to_string(z);
    }
};
struct Faces
{
    struct Vertex v[3];
    struct Vertex c;
    string tostring()
    {
        return v[0].tostring() + " " + v[1].tostring() + " " + v[2].tostring() + " ";
    }
    bool equals(struct Faces b)
    {
        return v[0].operator==(b.v[0]) && v[1].operator==(b.v[1]) && v[2].operator==(b.v[2]);
    }
};
class Object
{
public:
    struct Vertex *vertex;
    int n_vertexes;
    float *colors;
    std::vector<struct Faces> faces;
    glm::mat4 MVP;
    glm::vec3 inicial_pos,atual;
    struct Vertex max, min;
    float height, length, width;

    Object();
    void setVertexes(const char *c, struct Vertex *v, struct Faces *f);
    void setTexture(float r, float g, float b);
    bool equals(Object b);
    double distanceObjects(Object* b);
    struct Vertex getcenter();
    //bool Collisions(std::vector<Object> o);

private:
    void getfaces(string s, string del);
    struct Vertex getvertex(string s, string del);
    std::vector<int> indexes;
};
#endif

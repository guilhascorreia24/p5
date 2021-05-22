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
    inline operator<(const Vertex &a) const
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
};
class Object
{
public:
    struct Vertex *vertex;
    std::vector<struct Vertex> bodyCollider;
    int id, n_vertexes;
    float *colors;
    glm::mat4 MVP;
    glm::vec3 inicial_pos;
    float height,width,length;

    struct Vertex getCenterObject();
    string ToString();
    bool checkCollide(std::vector<Object> o);

private:
    void getfaces(string s, string del);
    struct Vertex getvertex(string s, string del);
    std::vector<int> indexes;

protected:
    struct Vertex max, min;
    void setVertexes(const char *f, struct Vertex *v);
    void setTexture(float r, float g, float b);
    void setBodyCollider();
};
#endif

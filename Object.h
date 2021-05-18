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

class Object
{

public:
    struct Vertex getCenterObject();
    struct Vertex min_point();
    struct Vertex max_point();
    bool checkColider(Object b);
    void setTexture(float r, float g, float b);
    void setVertexes(const char *f);

    struct Vertex *vertex;
    int n_vertexes;
    float *colors;
    glm::mat4 MVP;

private:
    void getfaces(string s, string del);
    struct Vertex getvertex(string s, string del);
    std::vector<int> indexes;
};
#endif

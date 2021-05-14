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
struct Vertex
{
    float x, y, z;
};

class Object
{
public:
    void setVertexes(const char *f);

    struct Vertex *vertices;
    int n_vertexes;
    float *colors;
    glm::mat4 MVP;

private:
    void getfaces(string s, string del);
    struct Vertex getvertex(string s, string del);
};

class Block : public Object
{
    float yVel = 0;

public:
    void Falling();
};
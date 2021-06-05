#ifndef SHEEP_H
#define SHEEP_H
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
#include "Object.h"
class Sheep : public Object
{
    float time = 0, vel = 0;

public:
    glm::vec3 rotations;
    glm::vec3 rotate_vertical,rotate_lateral;
    Sheep(const char *f,const char *t);
    Sheep(){};
    Sheep(struct Vertex c,struct Vertex min,struct Vertex max);
    void Moving(float t);

    private:
        glm::mat4 Translations(int key,float move,glm::vec3 r);

};
#endif
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
#include "Body.h"
#include "Plataform.h"
class Sheep : public Object
{

public:
    Body body,head_mems;
    glm::vec3 last_limit_floor;
    Sheep(const char *head_c,const char *head_t,const char *body_c,const char *body_t);
    Sheep(){}
    void Moves_Random(Plataform c);
    void Moves_to_block(glm::vec3 t);
    glm::mat4 Translate(glm::vec3 g);
    glm::mat4 Rotation(float radius,glm::vec3 r);

};
#endif
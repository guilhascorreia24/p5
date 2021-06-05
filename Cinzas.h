#ifndef CINZAS_H
#define CINZAS_H
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
#include "Object.h"
//glm::mat4 Projection,View;
class Cinzas : public Object
{
    public:
        Cinzas(const char *c,const char *f);
        Cinzas(){};

};
#endif
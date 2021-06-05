#ifndef BODY_H
#define BODY_H
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
class Body : public Object{
    public:
        Body(const char *c,const char *t);
        Body(){}
};
#endif
#ifndef BLOCK_H
#define BLOCK_H
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
class Block : public Object
{
    float time = 0, vel = 0;

public:
    //typedef Object super;
    Block(const char *f);
    void Falling(float t);
    

};
#endif
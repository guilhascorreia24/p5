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

    Block(const char *f);
    Block(){};
    Block(struct Vertex c,struct Vertex min,struct Vertex max);
    void block_reset();
    void Falling(float t);
    void Moves(int key);
    bool operator==(Block const &b)
    {
        return MVP[3][0] == b.MVP[3][0] && MVP[3][1] == b.MVP[3][1] && MVP[3][2] == b.MVP[3][2];
    }
};
#endif
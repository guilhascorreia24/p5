#ifndef PLATAFORM_H
#define PLATAFORM_H
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
#include "Block.h"
#include "Object.h"

class Plataform : public Object
{

public:
    std::vector<Block> blocks;
    Plataform(const char *f);
    Plataform(){};
    bool overBlock(Block b);

private:
    double distanceOther(glm::mat4 t);
    void superiorFaces(struct Faces f);
    void setBlocks();
};
#endif
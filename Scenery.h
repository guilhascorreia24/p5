#ifndef SCENERY_H
#define SCENERY_H
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
#include "Plataform.h"
#include "Block.h"
//glm::mat4 Projection,View;
 inline glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
 inline glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
class Scenery
{

public:
    std::vector<Object> objs;
    glm::mat4 MVP;
    Plataform plat;
    Plataform floor;
    Plataform sky;
    Block block;
    Scenery(glm::mat4 MVP, Block b, Plataform p);
    Scenery() {}
    void addObj(Object b);
    void addPlataform(Plataform b);
    void addBlock(Block b);
    bool BlockOverEdgesPrataform();
    bool CheckWin();
};
#endif
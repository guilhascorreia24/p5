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
#include "Sheep.h"
#include "Cinzas.h"
//glm::mat4 Projection,View;
class Scenery
{

public:
    std::vector<Object> objs;
    glm::mat4 MVP;
    Plataform plat;
    Plataform floor;
    Plataform sky;
    Block block;
    Sheep sheep;
    Scenery(glm::mat4 MVP, Block b, Plataform p,Plataform floor);
    Scenery(glm::mat4 MVP, Block b, Plataform p,Plataform floor, Sheep s);
    Scenery() {}
    Cinzas cinzas;

    void addObj(Object b);
    void addPlataform(Plataform b);
    void addBlock(Block b);
    void addSheep(Sheep b);
    bool BlockOverEdgesPrataform();
    void setCinzas(Cinzas c);

    static glm::mat4 Projection;
    static glm::mat4 View;

private:
    bool OverPlatformlying_down(glm::vec3 b);
    bool OntheBorders(glm::vec3 a,glm::vec3 b,glm::vec3 move);
    bool onLavaBlock(glm::vec3 b);
};
#endif
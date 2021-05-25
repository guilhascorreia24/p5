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
class Scenery
{
    public:
        std::vector<Object> objs;
        glm::mat4 MVP;
        Plataform plat;
        Block block;
        Scenery(glm::mat4 MVP,Block b,Plataform p){
            this->MVP=MVP;
            block=b;
            plat=p;
            block.MVP=MVP*glm::translate(glm::mat4(1), block.atual);
            plat.MVP=MVP*glm::translate(glm::mat4(1), plat.atual);
            addPlataform(plat);
        };
        Scenery(){}
        void addObj(Object b);
        void addPlataform(Plataform b);
        void addBlock(Block b);
};
#endif
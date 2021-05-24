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
class Scenery
{
    public:
        std::vector<Object> objs;
        glm::mat4 MVP;
        Scenery(glm::mat4 MVP){
            this->MVP=MVP;
        };
        void addObj(Object b);
        void addPlataform(Plataform b);
};
#endif
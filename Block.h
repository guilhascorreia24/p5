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
#include "body.h"
class Block : public Object
{
    float time = 0, vel = 0;

public:
    glm::vec3 rotations;
    glm::vec3 rotate_vertical,rotate_lateral;
    Body farpas;
    bool burn=false;
    Block(const char *f,const char *t);
    Block(){};
    Block(struct Vertex c,struct Vertex min,struct Vertex max);
    void block_reset();
    void Falling(float t);
    void Moves(int key);
    void reset();
    bool operator==(Block const &b)
    {
        return atual[0]==b.atual[0] && atual[1]==b.atual[1] && atual[2]==b.atual[2];
    }
    void setWalk(float x){this->walk=x;}
    void setFarpas(const char *c,const char *t);

    private:
        float walk;
        glm::mat4 Rotations(int key);
        //glm::mat4 BlockRoll();
        void setRotations(float r,glm::vec3 ori);
        glm::mat4 Translations(int key,float move,glm::vec3 r);

};
#endif
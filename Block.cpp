#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include "Block.h"
using namespace std;
Block::Block(const char *f)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    this->setVertexes(f,v);
    this->setTexture(0.8, 0.8, 0.8);
    this->setBodyCollider();
    length=max.x-min.x;
    height=max.y-min.y;
    width=max.z-min.z;
}

void Block::Falling(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.0001;
    this->vel += acc * dt;
    //float p = MVP[3][1]+this->vel * dt;
    glm::mat4 f=glm::translate(glm::mat4(1),glm::vec3(0,this->vel * dt,0));
    MVP=MVP*f;
}

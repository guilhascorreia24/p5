#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include "Block.h"
using namespace std;
Block::Block(const char *c)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(0.8, 0.8, 0.8);
    length = max.x - min.x;
    height = max.y - min.y;
    width = max.z - min.z;
}
void Block::block_reset()
{
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    MVP = Projection * View;
    MVP = MVP * glm::translate(glm::mat4(1), inicial_pos);
}

void Block::Falling(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.01;
    this->vel += acc * dt;
    glm::mat4 f = glm::translate(glm::mat4(1), glm::vec3(0, this->vel * dt, 0));
    MVP = MVP * f;
    atual=glm::vec3(atual[0],atual[1]+this->vel * dt,atual[2]);
}

void Block::Moves(int key)
{
    glm::mat4 f;
    if (key == GLFW_KEY_UP)
    {
        f = glm::translate(glm::mat4(1), glm::vec3(0, 0, -width));
    }
    if (key == GLFW_KEY_DOWN)
    {
        f = glm::translate(glm::mat4(1), glm::vec3(0, 0, width));
    }
    if (key == GLFW_KEY_LEFT)
    {
        f = glm::translate(glm::mat4(1), glm::vec3(-width, 0, 0));
    }
    if (key == GLFW_KEY_RIGHT)
    {
        f = glm::translate(glm::mat4(1), glm::vec3(width, 0, 0));
    }
    glm::mat4 u = MVP * f;
    MVP = u;
    //cout << MVP << std::endl;
}
Block::Block(struct Vertex c, struct Vertex min,struct Vertex max)
{
    max=max;
    min=min;
    height=max.y-min.y;
    width=max.z-min.z;
    length=max.x-min.x;
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    MVP=Projection*View;
    inicial_pos=glm::vec3(c.x,0,c.z);
    atual=inicial_pos;
    MVP=MVP*glm::translate(glm::mat4(1),inicial_pos);
}




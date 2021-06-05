#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include <math.h>
#include "Sheep.h"
#include "Scenery.h"
using namespace std;
Sheep::Sheep(const char *c, const char *t)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(t);
    length = max.x - min.x;
    height = max.y - min.y;
    width = max.z - min.z;
    rotate_vertical = glm::vec3(1, 0, 0);
    rotate_lateral = glm::vec3(0, 0, 1);
    Model=glm::mat4(1);
}

void Sheep::Moving(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.001;
    this->vel += acc * dt;
    standUP();
    glm::mat4 f;
    //cout<<rotate_lateral<< rotate_vertical<<std::endl;
    //cout<<rotations<<std::endl;
    f = glm::translate(glm::mat4(1), glm::vec3(0, this->vel * dt, 0));
    Model = Model * f;
    if (rotate_vertical == glm::vec3(0, 1, 0))
    {
        Model = Model * glm::rotate(glm::mat4(1), glm::radians(rotations[2]), glm::vec3(0, 0, 1));
    }
    else if (rotate_lateral == glm::vec3(0, 1, 0))
    {
        Model = Model * glm::rotate(glm::mat4(1), glm::radians(rotations[0]), glm::vec3(1, 0, 0));
    }
    atual = glm::vec3(atual[0], atual[1] + this->vel * dt, atual[2]);
    standUP();
}


Sheep::Sheep(struct Vertex c, struct Vertex min, struct Vertex max)
{
    this->max = max;
    this->min = min;
    height = max.y - min.y;
    width = max.z - min.z;
    length = max.x - min.x;
    Object b = Object();
    MVP = b.MVP;
    int r = 1000000;
    inicial_pos = glm::vec3(c.x, c.y, c.z);
    atual = inicial_pos;
    atual[0] = round(atual[0] * r) / r;
    atual[2] = round(atual[2] * r) / r;
    atual[1] = round(atual[1] * r) / r;
    MVP = MVP * glm::translate(glm::mat4(1), inicial_pos);
    rotate_vertical = glm::vec3(0, 1, 0);
    rotate_lateral = glm::vec3(0, 0, 1);
}

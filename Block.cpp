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
    orientation = glm::vec3(0, 1, 0);
}
void Block::block_reset()
{
    MVP = MVP * glm::translate(glm::mat4(1), inicial_pos);
}

void Block::Falling(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.001;
    this->vel += acc * dt;
    if (orientation[1] == 1)
    {
        glm::mat4 f = glm::translate(glm::mat4(1), glm::vec3(0, this->vel * dt, 0));
        MVP = MVP * f;
        atual = glm::vec3(atual[0], atual[1] + this->vel * dt, atual[2]);
    }
    else
    {
        glm::mat4 f = glm::translate(glm::mat4(1), glm::vec3((this->vel * dt), 0, 0));
        MVP = MVP * f;
        atual = glm::vec3(atual[0] + this->vel * dt, atual[1], atual[2]);
    }
}

void Block::Moves(int key)
{
    printf("antes\n");
    cout<<MVP<<std::endl;
    glm::mat4 f = glm::mat4(1), r = glm::mat4(1);
    float radius = 90;
    float walk = width + width / 2;
    if (key == GLFW_KEY_UP)
    {
        f = glm::translate(f, glm::vec3(0, 0, -walk));
        atual = glm::vec3(atual[0], atual[1], atual[2] - walk);
        r = glm::rotate(r, glm::radians(-radius), glm::vec3(1,0,0));
    }
    if (key == GLFW_KEY_DOWN)
    {
        f = glm::translate(f, glm::vec3(0, 0, walk));
        atual = glm::vec3(atual[0], atual[1], atual[2] + walk);
        r = glm::rotate(r, glm::radians(radius), glm::vec3(1,0,0));
    }
    if (key == GLFW_KEY_LEFT)
    {
        f = glm::translate(f, glm::vec3(-walk, 0, 0));
        atual = glm::vec3(atual[0] - walk, atual[1], atual[2]);
       r = glm::rotate(r, glm::radians(-radius),  glm::vec3(0, 0, 1));
    }
    if (key == GLFW_KEY_RIGHT)
    {
        f = glm::translate(f, glm::vec3(walk, 0, 0));
        atual = glm::vec3(atual[0] + walk, atual[1], atual[2]);
        r = glm::rotate(r, glm::radians(radius), glm::vec3(0, 0, 1));
    }
    glm::mat4 u = MVP *f ;
    MVP = u ;
    printf("depoois\n");
    cout << MVP << std::endl;
}

Block::Block(struct Vertex c, struct Vertex min, struct Vertex max)
{
    this->max = max;
    this->min = min;
    height = max.y - min.y;
    width = max.z - min.z;
    length = max.x - min.x;
    Object b = Object();
    MVP = b.MVP;
    inicial_pos = glm::vec3(c.x, 0, c.z);
    atual = inicial_pos;
    MVP = MVP * glm::translate(glm::mat4(1), inicial_pos);
    orientation = glm::vec3(0, 1, 0);
}
bool Block::Collisions(std::vector<Object> objs)
{
    bool collide = false;
    for (Object o : objs)
    {
        //cout<<o.min.x<<";"<<o.min.z<<" "<<o.max.x<<";"<<o.max.z<<std::endl;
        //cout<<atual[0]<<";"<<atual[2]<<std::endl;
        if (atual[0] >= o.min.x && atual[0] < o.max.x && atual[2] >= o.min.z && atual[2] < o.max.z)
        {
            if ((abs(atual[1] - o.atual[1]) < (o.height / 2) + (height / 2) && abs(atual[1] - o.atual[1]) > (o.height / 2) + (height / 2) - 0.5))
                return true;
        }
    }
    return collide;
}

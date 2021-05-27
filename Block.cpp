#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include <math.h>
#include "Block.h"
#include "Scenery.h"
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
    rotate_vertical = glm::vec3(1, 0, 0);
    rotate_lateral = glm::vec3(0, 0, 1);
}
void Block::reset()
{
    inicial_pos[1] = 5;
    atual = inicial_pos;
    MVP = MVP * glm::translate(glm::mat4(1), atual);
    rotate_vertical = glm::vec3(1, 0, 0);
    rotate_lateral = glm::vec3(0, 0, 1);
}
void Block::Falling(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.001;
    this->vel += acc * dt;
    standUP();
    glm::mat4 f = glm::translate(glm::mat4(1), glm::vec3(0, this->vel * dt, 0));
    MVP = MVP * f;
    atual = glm::vec3(atual[0], atual[1] + this->vel * dt, atual[2]);
}
void Block::Moves(int key)
{
    float walk = 1.5, ver;
    float radius = 90;
    //glm::vec3 r=rotation;
    standUP();
    printf("\n");
    glm::mat4 f = glm::mat4(1), r = glm::mat4(1);
    if (key == GLFW_KEY_UP)
    {
        f = glm::translate(f, glm::vec3(0, 0, -walk));
        atual = glm::vec3(atual[0], atual[1], atual[2] - walk);
        if (rotate_lateral != glm::vec3(0, 1, 0) && rotate_vertical != glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver = height / 2;
            f = glm::translate(f, glm::vec3(0, -ver, -walk / 2));
            r = glm::rotate(r, glm::radians(-radius), rotate_vertical);

            rotate_lateral = glm::vec3(0, 1, 0);
            rotate_vertical = glm::vec3(1, 0, 0);
            atual = glm::vec3(atual[0], atual[1] - ver, atual[2] - walk / 2);
        }
        else if (rotate_lateral == glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(0, ver, -walk / 2));
            atual = glm::vec3(atual[0], atual[1] + ver, atual[2] - walk / 2);
            //r = glm::rotate(r, glm::radians(-radius), rotate_vertical);

            rotate_vertical = glm::vec3(1, 0, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_vertical == glm::vec3(0, 1, 0))
        {

            r = glm::rotate(r, glm::radians(-radius), rotate_lateral);
            r = glm::rotate(r, glm::radians(-radius), rotate_vertical);
        }
        rotations = rotate_vertical * (-radius);
    }
    if (key == GLFW_KEY_DOWN)
    {
        f = glm::translate(f, glm::vec3(0, 0, walk));
        atual = glm::vec3(atual[0], atual[1], atual[2] + walk);
        if (rotate_lateral != glm::vec3(0, 1, 0) && rotate_vertical != glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver = height / 2;
            f = glm::translate(f, glm::vec3(0, -ver, walk / 2));
            r = glm::rotate(r, glm::radians(radius), rotate_vertical);

            rotate_lateral = glm::vec3(0, 1, 0);
            rotate_vertical = glm::vec3(1, 0, 0);
            atual = glm::vec3(atual[0], atual[1] - ver, atual[2] + walk / 2);
        }
        else if (rotate_lateral == glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(0, ver, walk / 2));
            //r = glm::rotate(r, glm::radians(radius), rotate_vertical);
            atual = glm::vec3(atual[0], atual[1] + ver, atual[2] + walk / 2);
            rotate_vertical = glm::vec3(1, 0, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_vertical == glm::vec3(0, 1, 0))
        {
            r = glm::rotate(r, glm::radians(radius), rotate_lateral);
            r = glm::rotate(r, glm::radians(radius), rotate_vertical);
        }
        rotations = rotate_vertical * (radius);
    }
    if (key == GLFW_KEY_LEFT)
    {
        f = glm::translate(f, glm::vec3(-walk, 0, 0));
        atual = glm::vec3(atual[0] - walk, atual[1], atual[2]);
        if (rotate_lateral != glm::vec3(0, 1, 0) && rotate_vertical != glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(-walk / 2, -ver, 0));
            r = glm::rotate(r, glm::radians(-radius), rotate_lateral);
            atual = glm::vec3(atual[0] - walk / 2, atual[1] - ver, atual[2]);
            rotate_vertical = glm::vec3(0, 1, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_vertical == glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(-walk / 2, ver, 0));
            //r = glm::rotate(r, glm::radians(-radius), rotate_lateral);
            atual = glm::vec3(atual[0] - walk / 2, atual[1] + ver, atual[2]);
            rotate_vertical = glm::vec3(1, 0, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_lateral == glm::vec3(0, 1, 0))
        {
            r = glm::rotate(r, glm::radians(radius), rotate_vertical);
            r = glm::rotate(r, glm::radians(-radius), rotate_lateral);
        }
        rotations = rotate_lateral * (-radius);
    }
    if (key == GLFW_KEY_RIGHT)
    {
        f = glm::translate(f, glm::vec3(walk, 0, 0));
        atual = glm::vec3(atual[0] + walk, atual[1], atual[2]);
        if (rotate_lateral != glm::vec3(0, 1, 0) && rotate_vertical != glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(+walk / 2, -ver, 0));
            r = glm::rotate(r, glm::radians(radius), rotate_lateral);
            atual = glm::vec3(atual[0] + walk / 2, atual[1] - ver, atual[2]);
            rotate_vertical = glm::vec3(0, 1, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_vertical == glm::vec3(0, 1, 0))
        {
            float exchange = length;
            length = height;
            height = exchange;
            ver=height/2;
            f = glm::translate(f, glm::vec3(+walk / 2, ver, 0));
            //r = glm::rotate(r, glm::radians(-radius), rotate_lateral);
            atual = glm::vec3(atual[0] + walk / 2, atual[1] + ver, atual[2]);
            rotate_vertical = glm::vec3(1, 0, 0);
            rotate_lateral = glm::vec3(0, 0, 1);
        }
        else if (rotate_lateral == glm::vec3(0, 1, 0))
        {

            r = glm::rotate(r, glm::radians(radius), rotate_vertical);
            r = glm::rotate(r, glm::radians(radius), rotate_lateral);
        }
        rotations = rotate_lateral * (radius);
    }
    //cout << rotations << std::endl;
    cout << rotate_vertical << std::endl;
    cout << rotate_lateral << std::endl;
    glm::mat4 u = MVP * f * r;
    MVP = u;
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
    rotate_vertical = glm::vec3(0, 1, 0);
    rotate_lateral = glm::vec3(0, 0, 1);
}
void Block::standUP()
{
    MVP = Projection * View * glm::translate(glm::mat4(1), atual);
    height = max.y - min.y;
    width = max.z - min.z;
    length = max.x - min.x;
}

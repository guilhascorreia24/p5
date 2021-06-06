#include <iostream>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include "Object.h"
#include "Block.h"
#include "Plataform.h"
#include "Cinzas.h"
#include "Scenery.h"
#include "Sheep.h"
#include <stdlib.h>
#include <vector>
#include <set>
#include <list>
#include <limits>
#include <math.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
//#include <unistd.h>

glm::mat4 Scenery::Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 Scenery::View = glm::lookAt(glm::vec3(5, 15, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

using namespace std;
void Scenery::addObj(Object b)
{
    objs.push_back(b);
}
void Scenery::addPlataform(Plataform b)
{
    plat = b;
    for (Block o : b.blocks)
    {
        //objs.push_back(o);
    }
}
void Scenery::addBlock(Block b)
{
    block = b;
    //objs.push_back(b);
}

void Scenery::addSheep(Sheep b)
{
    sheep = b;
    sheep.MVP = Projection * View;
}
Scenery::Scenery(glm::mat4 MVP, Block b, Plataform p, Plataform f)
{
    this->MVP = MVP;
    block = b;
    plat = p;
    floor = f;
    block.MVP = MVP;
    block.Model = glm::mat4(1);
    block.Model = block.Model * glm::translate(glm::mat4(1), block.atual);
    block.Model = block.Model * glm::rotate(glm::mat4(1), glm::radians(block.rotations[0]), glm::vec3(1, 0, 0));
    block.Model = block.Model * glm::rotate(glm::mat4(1), glm::radians(block.rotations[2]), glm::vec3(0, 0, 1));

    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    this->floor.atual[1] = -8;
    this->floor.MVP = MVP * glm::translate(glm::mat4(1), floor.atual);
    addPlataform(plat);
    objs.push_back(floor);
    block.setWalk(plat.blocks.at(0).width);
    for (int i = 0; i < plat.lavaBlocks.size(); i++)
    {
        plat.lavaBlocks.at(i).MVP = MVP * glm::translate(glm::mat4(1), plat.lavaBlocks.at(i).inicial_pos);
    }
    cinzas.MVP = Projection * View;
    //block.burn=false;
    //addPlataform(floor);
};

Scenery::Scenery(glm::mat4 MVP, Block b, Plataform p, Plataform f, Sheep s)
{
    this->MVP = MVP;
    block = b;
    plat = p;
    floor = f;
    sheep = s;
    sheep.setMVP(MVP);
    sheep.setModel(sheep.Model);
    //sheep.Model=sheep.Translate();
    block.MVP = MVP;
    block.Model = glm::mat4(1);
    block.Model = block.Model * glm::translate(glm::mat4(1), block.atual);
    block.Model = block.Model * glm::rotate(glm::mat4(1), glm::radians(block.rotations[0]), glm::vec3(1, 0, 0));
    block.Model = block.Model * glm::rotate(glm::mat4(1), glm::radians(block.rotations[2]), glm::vec3(0, 0, 1));

    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    this->floor.atual[1] = -8;
    this->floor.MVP = MVP * glm::translate(glm::mat4(1), floor.atual);
    addPlataform(plat);
    objs.push_back(floor);
    block.setWalk(plat.blocks.at(0).width);
    for (int i = 0; i < plat.lavaBlocks.size(); i++)
    {
        plat.lavaBlocks.at(i).MVP = MVP * glm::translate(glm::mat4(1), plat.lavaBlocks.at(i).inicial_pos);
    }
    //addPlataform(floor);
};
bool Scenery::BlockOverEdgesPrataform()
{
    Block o;
    double dist = INT_MAX;
    bool line = false;
    std::vector<Object> obj;
    //cout << block.tostring() << ::endl;
    //cout << plat.final_.tostring() << std::endl;
    float size = plat.blocks.at(0).width;
    for (Object b : plat.blocks)
    {
        obj.push_back(b);
    }
    for (Object b : plat.lavaBlocks)
    {
        obj.push_back(b);
    }
    glm::vec3 frente = block.atual + glm::vec3(0, 0, block.height / 4);
    glm::vec3 tras = block.atual + glm::vec3(0, 0, -block.height / 4);
    glm::vec3 direita = block.atual + glm::vec3(block.height / 4, 0, 0);
    glm::vec3 esquerda = block.atual + glm::vec3(-block.height / 4, 0, 0);
    //cout << block.tostring() << std::endl;
    if (block.Collisions(obj))
    {
        if (block.rotate_lateral == glm::vec3(0, 1, 0))
        {
            return OntheBorders(frente, tras, glm::vec3(0, 0, block.height / 4));
        }
        else if (block.rotate_vertical == glm::vec3(0, 1, 0))
        {
            return OntheBorders(direita, esquerda, glm::vec3(block.height / 4, 0, 0));
        }
        else
        {
            if (onLavaBlock(block.atual))
            {
                block.burn = true;
                cinzas.atual = block.atual;
                cinzas.Model = glm::translate(glm::mat4(1), cinzas.atual);
            }
            return true;
        }
    }
    if (block.rotate_lateral == glm::vec3(0, 1, 0))
    {

        return OntheBorders(frente, tras, glm::vec3(0, 0, block.height / 4));
    }
    if (block.rotate_vertical == glm::vec3(0, 1, 0))
    {
        return OntheBorders(direita, esquerda, glm::vec3(block.height / 4, 0, 0));
    }

    return false;
}

bool Scenery::OverPlatformlying_down(glm::vec3 t)
{
    for (Block b : plat.blocks)
    {
        if (t[0] > b.min.x && t[0] < b.max.x && t[2] > b.min.z && t[2] < b.max.z)
        {
            return true;
        }
    }
    Block b = plat.final_;
    if (t[0] > b.min.x && t[0] < b.max.x && t[2] > b.min.z && t[2] < b.max.z)
    {
        return true;
    }
    return false;
}

bool Scenery::OntheBorders(glm::vec3 a, glm::vec3 b, glm::vec3 move)
{
    //cout << (bool)(OverPlatformlying_down(a) && OverPlatformlying_down(b)) << std::endl;
    if (OverPlatformlying_down(a) && OverPlatformlying_down(b))
    {
        return true;
    }

    if (onLavaBlock(a) || onLavaBlock(b))
    {
        block.burn = true;
        cinzas.atual = block.atual;
        cinzas.Model = glm::translate(glm::mat4(1), cinzas.atual);
        return true;
    }
    if (OverPlatformlying_down(a) && !OverPlatformlying_down(b))
    {
        block.atual -= move;
        return false;
    }
    if (!OverPlatformlying_down(a) && OverPlatformlying_down(b))
    {
        block.atual += move;
        return false;
    }
    return false;
}

bool Scenery::onLavaBlock(glm::vec3 t)
{
    for (Block b : plat.lavaBlocks)
    {
        //cout<<b.min.tostring()<<" "<<b.max.tostring()<<std::endl;
        if (t[0] > b.min.x && t[0] < b.max.x && t[2] > b.min.z && t[2] < b.max.z)
        {
            block.standUP();
            block.Model = glm::translate(glm::mat4(1), b.atual);
            block.MVP = Projection * View;
            block.atual = b.atual + glm::vec3(0, b.max.y, 0);
            return true;
        }
    }
    return false;
}
#include <iostream>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include "Object.h"
#include "Block.h"
#include "Plataform.h"
#include "Scenery.h"
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
#include <unistd.h>
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
        objs.push_back(o);
    }
}
void Scenery::addBlock(Block b)
{
    block = b;
    //objs.push_back(b);
}
Scenery::Scenery(glm::mat4 MVP, Block b, Plataform p)
{
    this->MVP = MVP;
    block = b;
    plat = p;
    cout << block.atual << std::endl;
    block.MVP = MVP * glm::translate(glm::mat4(1), block.atual);
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[0]), glm::vec3(1, 0, 0));
    //block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[1]), glm::vec3(0, 1, 0));
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[2]), glm::vec3(0, 0, 1));
    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    addPlataform(plat);
};
bool Scenery::BlockOverEdgesPrataform()
{
    Block o;
    double dist = INT_MAX;
    bool line = false;
    std::vector<Object> obj;
    int r = 1000000;
    //cout<<block.tostring()<<::endl;
    //cout<<plat.final_.tostring()<<std::endl;
    block.atual[0] = round(block.atual[0] * r) / r;
    block.atual[2] = round(block.atual[2] * r) / r;
    for (Object b : plat.blocks)
    {
        obj.push_back(b);
    }
    //cout << block.tostring() << std::endl;
    if (block.Collisions(obj))
    {
        return true;
    }
    if (block.rotate_vertical == glm::vec3(0, 1, 0))
    {
        struct Vertex v;
        v.x = block.getcenter().x + 1.5 / 2;
        v.y = block.getcenter().y;
        v.z = block.getcenter().z;
        Block b = Block(v, v, v);
        v.x = block.getcenter().x - 1.5 / 2;
        Block b1 = Block(v, v, v);
        //cout << b.tostring() << std::endl;
        //cout << b1.tostring() << std::endl;
        cout << plat.overBlock(b) << " " << plat.overBlock(b1) << std::endl;
        if (plat.overBlock(b) && plat.overBlock(b1))
        {
            return true;
        }
        else if (plat.overBlock(b) && !b.operator==(plat.final_))
        {
            block.standUP();
            //block.atual[0]-=1.5/2;
            block.MVP = glm::translate(block.MVP, glm::vec3(-1.5 / 2, 0, 0));
            return false;
        }
        else if (plat.overBlock(b1) && !b1.operator==(plat.final_))
        {
            block.standUP();
            //block.atual[0]+=1.5/2;
            block.MVP = glm::translate(block.MVP, glm::vec3(1.5 / 2, 0, 0));
            return false;
        }
    }
    if (block.rotate_lateral == glm::vec3(0, 1, 0))
    {
        struct Vertex v;
        v.z = block.getcenter().z + 1.5 / 2;
        v.y = block.getcenter().y;
        v.x = block.getcenter().x;
        Block b = Block(v, v, v);
        v.z = block.getcenter().z - 1.5 / 2;
        Block b1 = Block(v, v, v);
        if (plat.overBlock(b) && plat.overBlock(b1))
        {
            return true;
        }
        else if (plat.overBlock(b) && !b.operator==(plat.final_))
        {
            block.standUP();
            block.atual[2]-=1.5/2;
            block.MVP = glm::translate(block.MVP, glm::vec3(0, 0, -1.5 / 2));
            return false;
        }
        else if (plat.overBlock(b1) && !b1.operator==(plat.final_))
        {
            block.standUP();
            block.atual[2]+=1.5/2;
            block.MVP = glm::translate(block.MVP, glm::vec3(0, 0, 1.5 / 2));
            return false;
        }
    }
    return false;
}

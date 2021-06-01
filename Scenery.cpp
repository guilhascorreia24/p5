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
//#include <unistd.h>

glm::mat4 Scenery::Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));


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
Scenery::Scenery(glm::mat4 MVP, Block b, Plataform p,Plataform f)
{
    this->MVP = MVP;
    block = b;
    plat = p;
    floor=f;
    //cout << block.atual << std::endl;
    block.MVP = MVP * glm::translate(glm::mat4(1), block.atual);
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[0]), glm::vec3(1, 0, 0));
    //block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[1]), glm::vec3(0, 1, 0));
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[2]), glm::vec3(0, 0, 1));
    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    //cout<<floor.atual<<std::endl;
    this->floor.atual[1]=-8;
    this->floor.MVP=MVP*glm::translate(glm::mat4(1), floor.atual);
    addPlataform(plat);
    objs.push_back(floor);
    //addPlataform(floor);
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
        //cout << (bool)(b.atual[0] == plat.final_.atual[0] && b.atual[2] == plat.final_.atual[2]) << std::endl;
        //cout << b.tostring() << " " << b1.tostring() << " " << plat.final_.tostring() << std::endl;
        if (plat.overBlock(b) && plat.overBlock(b1))
        {
            printf("sobre plat\n");
            return true;
        }
        else if (plat.overBlock(b))
        {
            printf("falta 1 vert - plat\n");
            if ((b1.atual[0] == plat.final_.atual[0] && b1.atual[2] == plat.final_.atual[2])||(b.atual[0] == plat.final_.atual[0] && b.atual[2] == plat.final_.atual[2]))
            {
                printf("sob o final\n");
                return true;
            }
            block.standUP();
            block.atual[0]-=1.5/2;
            block.MVP = glm::translate(block.MVP, glm::vec3(-1.5 / 2, 0, 0));
            return false;
        }
        else if (plat.overBlock(b1))
        {
            printf("falta 1 vert + plat\n");
            if ((b1.atual[0] == plat.final_.atual[0] && b1.atual[2] == plat.final_.atual[2])||(b.atual[0] == plat.final_.atual[0] && b.atual[2] == plat.final_.atual[2]))
            {
                printf("sob o final\n");
                return true;
            }
            block.standUP();
            block.atual[0]+=1.5/2;
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
        //cout << b.tostring() << " " << b1.tostring() << " " << plat.final_.tostring() << std::endl;
        if (plat.overBlock(b) && plat.overBlock(b1))
        {
            printf("sobre plat\n");
            return true;
        }
        else if (plat.overBlock(b))
        {
            printf("falta 1 lat - plat\n");
            if ((b1.atual[0] == plat.final_.atual[0] && b1.atual[2] == plat.final_.atual[2])||(b.atual[0] == plat.final_.atual[0] && b.atual[2] == plat.final_.atual[2]))
            {
                printf("sob o final\n");
                return true;
            }
            block.standUP();
            block.atual[2] -= 1.5 / 2;
            block.MVP = glm::translate(block.MVP, glm::vec3(0, 0, -1.5 / 2));
            return false;
        }
        else if (plat.overBlock(b1))
        {
            printf("falta 1 lat +plat\n");
            if ((b1.atual[0] == plat.final_.atual[0] && b1.atual[2] == plat.final_.atual[2])||(b.atual[0] == plat.final_.atual[0] && b.atual[2] == plat.final_.atual[2]))
            {
                printf("sob o final\n");
                return true;
            }
                block.standUP();
            block.atual[2] += 1.5 / 2;
            block.MVP = glm::translate(block.MVP, glm::vec3(0, 0, 1.5 / 2));
            return false;
        }
    }
    return false;
}

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
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[1]), glm::vec3(0, 1, 0));
    block.MVP = block.MVP * glm::rotate(glm::mat4(1), glm::radians(block.rotations[2]), glm::vec3(0, 0, 1));
    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    addPlataform(plat);
};
bool Scenery::BlockOverEdgesPrataform()
{
    Block o;
    double dist = INT_MAX;
    bool line=false;
    for (Block p : plat.blocks)
    {
        if (p.distanceObjects(&block) < dist)
        {
            dist = p.distanceObjects(&block);
            //printf("found\n");
            o = p;
        }
    }
    //cout<<o.min.tostring()<<" "<<o.max.tostring()<<std::endl;
    //cout<<(bool)(block.atual[0]<=o.max.x && block.atual[0]>=o.min.x && block.atual[2]<=o.max.z && block.atual[2]>=o.min.z)<<std::endl;
    if((abs(block.atual[1] - o.atual[1]) < (o.height / 2) + (block.height / 2)) && (block.atual[0]<=o.max.x && block.atual[0]>=o.min.x && block.atual[2]<=o.max.z && block.atual[2]>=o.min.z)){
        line=true;
    }
    return line;
}
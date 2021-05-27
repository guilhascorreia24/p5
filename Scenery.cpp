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
    plat.MVP = MVP * glm::translate(glm::mat4(1), plat.atual);
    addPlataform(plat);
};
bool Scenery::BlockOverEdgesPrataform()
{
    Block o;
    double dist = INT_MAX;
    for (Block p : plat.blocks)
    {
        if (p.distanceObjects(&block) < dist)
        {
            dist = p.distanceObjects(&block);
            o = p;
        }
    }
    int i=0;
    bool line=false;
    for(Block p:plat.blocks){
        if (p.distanceObjects(&o) <= sqrt(pow(o.length,2)*2))
        {
            if(p.max.x==block.atual[0] || p.min.x==block.atual[0] || p.max.z==block.atual[2] || p.min.z==block.atual[2]){
                //printf("on line\n");
                line=true;
            } 
        }
    }
    return line;
}
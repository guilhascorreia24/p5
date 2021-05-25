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
void Scenery::addObj(Object b){
    objs.push_back(b);
}
void Scenery::addPlataform(Plataform b){
    plat=b;
    for(Block o:b.blocks){
        objs.push_back(o);
    }
}
void Scenery::addBlock(Block b){
    block=b;
    //objs.push_back(b);
}
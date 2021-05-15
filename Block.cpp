#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include "Object.h"
using namespace std;

void Block::Falling(){
    float Fritction=0.01,Gravity=9.8;
    this->MVP[3][1]-=Fritction;
}
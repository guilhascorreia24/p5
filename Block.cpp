#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include "Object.h"
using namespace std;

void Block::Falling(float t_now){
    //cout << t_now << std::endl;
    float dt=t_now-this->time;
    float acc=-0.0001;
    this->vel+=acc*dt;
    this->MVP[3][1]+=this->vel*dt;

}


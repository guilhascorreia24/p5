#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include "Block.h"
using namespace std;
Block::Block(const char *f)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    this->setVertexes(f,v);
    this->setTexture(0.8, 0.8, 0.8);
    this->setBodyCollider();
}

void Block::Falling(float t_now)
{
    float dt = t_now - this->time;
    float acc = -0.0001;
    this->vel += acc * dt;
    this->MVP[3][1] += this->vel * dt;
    for(struct Vertex v:bodyCollider){
        v.y+=this->vel * dt;
    }
    max.y+=this->vel * dt;
    min.y+=this->vel * dt;

}

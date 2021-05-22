#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
//#include "Object.h"
#include "Plataform.h"
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

Plataform::Plataform(const char *f)
{
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    this->setVertexes(f, v);
    this->setTexture(0.2, 0.2, 0.2);
    this->setBodyCollider();
    length=max.x-min.x;
    height=max.y-min.y;
    width=max.z-min.z;
}


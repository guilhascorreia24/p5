#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include <math.h>
#include "Body.h"
#include "Scenery.h"
using namespace std;
Body::Body(const char *c,const char *t){
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(t);
    this->MVP=Scenery::Projection*Scenery::View;
    this->Model=glm::mat4(1);
}
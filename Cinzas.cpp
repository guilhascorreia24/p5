#include <iostream>
#include <set>
#include <utility>
#include <string>
#include <fstream>
#include "Cinzas.h"
#include "Objscomps.h"
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
Cinzas::Cinzas(const char *c,const char *t){
    struct Vertex *v = (struct Vertex *)malloc(100000 * sizeof(struct Vertex));
    struct Faces *f = (struct Faces *)malloc(100000 * sizeof(struct Faces));
    this->setVertexes(c, v, f);
    this->setTexture(t);
    Model=glm::mat4(1);
    MVP=Scenery::Projection*Scenery::View;
}
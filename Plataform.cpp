#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
//#include "Object.h"
#include "Plataform.h"
using namespace std;

Plataform::Plataform(const char *f){
    this->setVertexes(f);
}
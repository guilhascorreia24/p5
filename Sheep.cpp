#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <vector>
#include <math.h>
#include "Sheep.h"
#include "Scenery.h"
using namespace std;
Sheep::Sheep(const char *head_c, const char *head_t, const char *body_c, const char *body_t)
{
    this->body = Body(body_c, body_t);
    this->head_mems = Body(head_c, head_t);
    body.MVP = Scenery::Projection * Scenery::View;
    head_mems.MVP = Scenery::Projection * Scenery::View;
    MVP = Scenery::Projection * Scenery::View;
    last_limit_floor = glm::vec3(0, 0, 0);
}
void Sheep::Moves_Random(Plataform p)
{

    Model = Translate(glm::vec3(glfwGetTime(), 0, glfwGetTime()));
}

void Sheep::Moves_to_block()
{
}

glm::mat4 Sheep::Translate(glm::vec3 t)
{
    this->body.Model = body.Model * glm::translate(glm::mat4(1), t);
    this->head_mems.Model = head_mems.Model * glm::translate(glm::mat4(1), t);
    return Model * glm::translate(glm::mat4(1), t);
}

glm::mat4 Sheep::Rotation(float t, glm::vec3 r)
{
    this->body.Model = body.Model * glm::rotate(glm::mat4(1), glm::radians(t), r);
    this->head_mems.Model = head_mems.Model * glm::rotate(glm::mat4(1), glm::radians(t), r);
    return Model * glm::rotate(glm::mat4(1), glm::radians(t), r);
}

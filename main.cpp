#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include <iostream>
#include "Block.h"
#include "Plataform.h"
#include "Scenery.h"
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool collide);
void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods);
void reposition(glm::vec3 v, Scenery *l);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
Block block;
Plataform plat;
glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
Scenery level1;
bool colide = false;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 vertexColor;\n"
                                 "out vec3 fragmentColor;\n"
                                 "uniform mat4 MVP;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = MVP * vec4(aPos, 1.0);\n"
                                 "   fragmentColor = vertexColor;\n"
                                 "}\0";

// declare and define fshader, position in color vector declaration
// are RGBA from [0,1] simply in and out
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragmentColor;\n;"
                                   "out vec3 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = fragmentColor;\n"
                                   "}\n\0";
float xpos1, ypos1;
int main()
{

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // check for vertex shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // check for shader linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  // delete shaders, we don't need them anymore
  block = Block("../../p5/objs/stoneBlock.obj");
  plat = Plataform("../../p5/objs/level1.obj");
  glm::mat4 Model = glm::mat4(1.0f);

  level1 = Scenery(Projection * View * Model, block, plat);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  //------------------------------------------------------------
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO[2];
  unsigned int EBO[2];
  glGenBuffers(1, &VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * level1.block.n_vertexes, level1.block.vertex, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * level1.plat.n_vertexes, level1.plat.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  level1.block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  reposition(block.inicial_pos, &level1);

  //level1.addObj(block);
  while (!glfwWindowShouldClose(window))
  {
    unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    processInput(window, colide);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGenBuffers(1, &EBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ARRAY_BUFFER, level1.block.n_vertexes * 3 * sizeof(float), level1.block.colors, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ARRAY_BUFFER, level1.plat.n_vertexes * 3 * sizeof(float), level1.plat.colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &level1.plat.MVP[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, level1.plat.n_vertexes);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &level1.block.MVP[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, level1.block.n_vertexes);

    for (int i = 0; i < 2; i++)
    {
      glDeleteBuffers(1, &EBO[i]);
    }
    if (!colide)
      level1.block.Falling(glfwGetTime());
    else
    {
      glfwSetTime(0);
    }
    colide = level1.block.Collisions(level1.objs);
    //cout << level.MVP << std::endl;
    //cout << block.MVP << std::endl;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  for (int i = 0; i < 2; i++)
  {

    glDeleteBuffers(1, &VBO[i]);
  }
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window, bool collide)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  glfwSetKeyCallback(window, moveBlock);
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
  {
    View = glm::lookAt(glm::vec3(0, 15, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    level1 = Scenery(Projection * View, block, plat);
  }
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
  {
    View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    level1 = Scenery(Projection * View, block, plat);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_UP || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS && colide)
  {
    level1.block.Moves(key);
  }
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    level1.block.MVP = Projection * View;
    reposition(block.inicial_pos, &level1);
    glfwSetTime(0);
  }
}
void reposition(glm::vec3 v, Scenery* l)
{
  double dist = INT_MAX;
  Block t = Block();
  l->block.MVP = l->block.MVP * glm::translate(glm::mat4(1), l->block.inicial_pos);
  for (Block o : l->plat.blocks)
  {
    if (o.distanceObjects(&l->block) < dist)
    {
      dist = o.distanceObjects(&l->block);
      t = o;
    }
  }
  l->block.MVP = Projection * View;
  l->block.inicial_pos[1] = 5;
  l->block.atual = l->block.inicial_pos;
  l->block.MVP = l->block.MVP * glm::translate(glm::mat4(1), l->block.inicial_pos);
}
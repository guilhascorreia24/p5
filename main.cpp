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
void processInput(GLFWwindow *window);
void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods);
void reposition(glm::vec3 v, Block *b, Plataform p);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
Block block("../../p5/objs/stoneBlock.obj");
Plataform level("../../p5/objs/level1.obj");
std::vector<Object> objs;
glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
glm::mat4 View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

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
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * block.n_vertexes, block.vertex, GL_STATIC_DRAW);

  glGenBuffers(1, &VBO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * level.n_vertexes, level.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glm::mat4 Model = glm::mat4(1.0f);

  block.MVP = Projection * View * Model;
  level.MVP = Projection * View * Model;
  Scenery level1(Projection * View * Model);

  block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  block.block_reset();
  reposition(block.inicial_pos, &block, level);
  block.block_reset();

  //level1.addObj(block);
  level1.addPlataform(level);

  bool colide = false;
  while (!glfwWindowShouldClose(window))
  {
    unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    processInput(window);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glGenBuffers(1, &EBO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ARRAY_BUFFER, block.n_vertexes * 3 * sizeof(float), block.colors, GL_STATIC_DRAW);
    glGenBuffers(1, &EBO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ARRAY_BUFFER, level.n_vertexes * 3 * sizeof(float), level.colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &level.MVP[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, level.n_vertexes);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &block.MVP[0][0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glBindBuffer(GL_ARRAY_BUFFER, EBO[0]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, block.n_vertexes);

    for (int i = 0; i < 2; i++)
    {
      glDeleteBuffers(1, &EBO[i]);
    }
    if (!colide)
      block.Falling(glfwGetTime());
    colide = block.Collisions(level1.objs);
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

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  glfwSetKeyCallback(window, moveBlock);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_LEFT || key == GLFW_KEY_UP || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
  {
    block.Moves(key);
  }
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    objs.clear();
    block.block_reset();
    objs.push_back(level);
    objs.push_back(block);
  }
}
void reposition(glm::vec3 v, Block *b, Plataform p)
{
  double dist = INT_MAX;
  Block t = Block();
  for (Block o : p.blocks)
  {
    if (o.distanceObjects(b) < dist)
    {
      dist = o.distanceObjects(b);
      t = o;
    }
  }
  cout << t.inicial_pos << std::endl;
  b->inicial_pos = t.inicial_pos;
  b->inicial_pos[1] = 5;
  b->atual=b->inicial_pos;
  cout << b->inicial_pos << std::endl;
}
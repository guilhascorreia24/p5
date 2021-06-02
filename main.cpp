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
#include "Objscomps.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool collide);
void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods);
void reposition(glm::vec3 v, Scenery *l);
void atributes(glm::mat4 g, unsigned int VBO);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
unsigned int MatrixID;
Block block;
Plataform plat;
Scenery level1;
bool colide = false;
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 vertexColor;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "out vec3 fragmentColor;\n"
                                 "out vec2 TexCoord;\n"
                                 "uniform mat4 MVP;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = MVP * vec4(aPos, 1.0);\n"
                                 "   fragmentColor = vertexColor;\n"
                                 "    TexCoord = aTexCoord;\n"
                                 "}\0";

// declare and define fshader, position in color vector declaration
// are RGBA from [0,1] simply in and out
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "in vec3 fragmentColor;\n"
                                   "uniform sampler2D texture1;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = texture(texture1, TexCoord) ;\n"
                                   "}\n\0";
/*const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragmentColor;\n;"
                                   "out vec3 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = fragmentColor;\n"
                                   "}\n\0";*/
float xpos1, ypos1;
int main()
{

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  //glEnable(GL_MULTISAMPLE);
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
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // delete shaders, we don't need them anymore
  MatrixID = glGetUniformLocation(shaderProgram, "MVP");
  block = Block("../../p5/objs/stoneBlock.obj", "../../p5/textures/predra.png");
  plat = Plataform("../../p5/objs/level1.obj", "../../p5/textures/vidro.png");
  Plataform floor = Plataform("../../p5/objs/floor1.obj", "../../p5/textures/cimento.png");
  glm::mat4 Model = glm::mat4(1.0f);
  cout << (sizeof(Texture) + sizeof(struct Vertex) * 2) << std::endl;
  level1 = Scenery(Scenery::Projection * Scenery::View * Model, block, plat, floor);
  //------------------------------------------------------------
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO[3];
  glGenBuffers(1, &VBO[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level1.block.n_vertexes, level1.block.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level1.plat.n_vertexes, level1.plat.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level1.floor.n_vertexes, level1.floor.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  level1.block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  reposition(block.inicial_pos, &level1);

  //level1.addObj(block);
  //timerun=glfwGetTime();
  glUseProgram(shaderProgram);
  while (!glfwWindowShouldClose(window))
  {
    processInput(window, colide);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(VAO);

    atributes(level1.plat.MVP, VBO[1]);
    level1.plat.loadTextures();
    glDrawArrays(GL_TRIANGLES, 0, level1.plat.n_vertexes);
    atributes(level1.floor.MVP, VBO[2]);
    level1.floor.loadTextures();
    glDrawArrays(GL_TRIANGLES, 0, level1.floor.n_vertexes);
    atributes(level1.block.MVP, VBO[0]);
    level1.block.loadTextures();
    glDrawArrays(GL_TRIANGLES, 0, level1.block.n_vertexes);

    //cout<<level1.block.atual<<std::endl;
    if (!colide && !level1.block.Collisions(level1.objs))
    {
      //printf("oi\n");
      level1.block.Falling(glfwGetTime());
    }
    else
    {
      glfwSetTime(0);
    }
    colide = level1.BlockOverEdgesPrataform();

    //cout << level.MVP << std::endl;
    //cout << block.MVP << std::endl;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  for (int i = 0; i < 3; i++)
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
    Scenery::View = glm::lookAt(glm::vec3(0, 25, 0.01), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    level1 = Scenery(Scenery::Projection * Scenery::View, level1.block, level1.plat, level1.floor);
  }
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
  {
    Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    level1 = Scenery(Scenery::Projection * Scenery::View, level1.block, level1.plat, level1.floor);
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
    level1.block.MVP = Scenery::Projection * Scenery::View;
    reposition(level1.block.inicial_pos, &level1);
    glfwSetTime(0);
  }
}
void reposition(glm::vec3 v, Scenery *l)
{
  double dist = INT_MAX;
  Block t = Block();
  l->block.inicial_pos[1] = 0;
  l->block.atual = l->block.inicial_pos;
  l->block.MVP = l->MVP * glm::translate(glm::mat4(1), l->block.inicial_pos);
  for (Block o : l->plat.blocks)
  {
    if (o.distanceObjects(&l->block) < dist)
    {
      dist = o.distanceObjects(&l->block);
      t = o;
    }
  }
  l->block.inicial_pos = t.inicial_pos;
  //cout<<l->block.tostring()<<std::endl;
  l->block.MVP = Scenery::Projection * Scenery::View;
  l->block.reset();
}

void atributes(glm::mat4 g, unsigned int VBO)
{
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g[0][0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}


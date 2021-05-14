#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include <iostream>
#include "Object.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void getNormalizedCoords(double xpos, double ypos);
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float radius = 1.0f;
// Input vertex data, different for all executions of this shader.
// Output data color, will be interpolated for each fragment.
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

  // link shaders : puts together the vertex and the fragment shaders
  // into a "shader program object"
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
  Object p;
  p.setVertexes("../../p5/objs/sample.obj");
  for(int i=0;i<p.n_vertexes;i++){
    cout << p.vertices[i].x << " " << p.vertices[i].y << " " << p.vertices[i].z << std::endl;
  }
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*p.n_vertexes, p.vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

  //------------------------------------------------------------
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(
      0,        // attribute. No particular reason for 0, but
                // must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  // 2nd attribute buffer : colors
  /*glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(
      1,        // attribute. No particular reason for 1, but
                // must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );*/

  glm::mat4 S = glm::mat4(1.0f);
  S = glm::scale(S, glm::vec3(0.8f, 0.8f, 0.8f));

  glm::mat4 T = glm::mat4(1.0f);
  glm::mat4 R = glm::mat4(1.0f);
  unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");

  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 View = glm::lookAt(
      glm::vec3(4, 4, 3), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0), // and looks at the origin
      glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
  );
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 MVP = Projection * View * Model;
  printf("mvp\n");
  std::cout << MVP << std::endl;
  double xpos, ypos;
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw our first triangle: using shader program
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // seeing as we only have a single VAO there's no need to bind
    // it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, p.n_vertexes);

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}


void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}
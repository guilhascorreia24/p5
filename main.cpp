#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
bool colored = false;
int max = 100000;

int N = 9;
float colors[] = {0.2f, 0.3f, 0.3f};

// Input vertex data, different for all executions of this shader.
// Output data color, will be interpolated for each fragment.
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 vertexColor;\n"
                                 "out vec3 fragmentColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 " gl_Position = vec4(aPos, 1.0);\n"
                                 " fragmentColor = vertexColor;\n"
                                 "}\0";
// declare and define fshader, position in color vector declaration
// are RGBA from [0,1] simply in and out
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 fragmentColor;\n;"
                                   "out vec3 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   " FragColor = fragmentColor;\n"
                                   "}\n\0";

unsigned long long factorial(int t)
{
  int res = 1, i;
  for (i = 2; i <= t; i++)
    res *= i;
  return res;
}
float get(float t, float xp[])
{
  int k = 0;
  float result = 0;
  while (k < 10)
  {
    float xi = xp[k];
    result += (factorial(N) / (factorial(k) * factorial(N - k))) * pow(1 - t, N - k) * pow(t, k) * xi;
    //printf("%llu\n",(factorial(N) / factorial(k) * factorial(N - k)));
    k++;
  }
  return result;
}
int main()
{

  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Janela Temporizada", NULL,
                                        NULL);

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
  // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
  // vertices give a triangle. A cube has 6 faces with 2 triangles each, so this
  // makes 6*2=12 triangles, and 12*3 vertices
  // ------------------------------------------------------------------
  static const float vertices[] = {
      -1.0f, -1.0f, -1.0f, // left,down,rear - T1
      -1.0f, -1.0f, 1.0f,  // left,down,front
      -1.0f, 1.0f, 1.0f,   // left,up,front
      1.0f, 1.0f, -1.0f,   // right,up,rear - T2
      -1.0f, -1.0f, -1.0f, // left,down,rear
      -1.0f, 1.0f, -1.0f,  // left,up,rear
      1.0f, -1.0f, 1.0f,   // right,down,front - T3
      -1.0f, -1.0f, -1.0f, // left,down,rear
      1.0f, -1.0f, -1.0f,  // right,down,rear
      1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,
      -1.0f, -1.0f, -1.0f,
      -1.0f, 1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,
      1.0f, -1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, 1.0f, -1.0f,
      1.0f, -1.0f, -1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, -1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, -1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, 1.0f, 1.0f,
      -1.0f, 1.0f, 1.0f,
      1.0f, -1.0f, 1.0f};

  // One color for each vertex. They were generated randomly.
  static const float colors[] = {
      0.583f, 0.771f, 0.014f,
      0.609f, 0.115f, 0.436f,
      0.327f, 0.483f, 0.844f,
      0.822f, 0.569f, 0.201f,
      0.435f, 0.602f, 0.223f,
      0.310f, 0.747f, 0.185f,
      0.597f, 0.770f, 0.761f,
      0.559f, 0.436f, 0.730f,
      0.359f, 0.583f, 0.152f,
      0.483f, 0.596f, 0.789f,
      0.559f, 0.861f, 0.639f,
      0.195f, 0.548f, 0.859f,
      0.014f, 0.184f, 0.576f,
      0.771f, 0.328f, 0.970f,
      0.406f, 0.615f, 0.116f,
      0.676f, 0.977f, 0.133f,
      0.971f, 0.572f, 0.833f,
      0.140f, 0.616f, 0.489f,
      0.997f, 0.513f, 0.064f,
      0.945f, 0.719f, 0.592f,
      0.543f, 0.021f, 0.978f,
      0.279f, 0.317f, 0.505f,
      0.167f, 0.620f, 0.077f,
      0.347f, 0.857f, 0.137f,
      0.055f, 0.953f, 0.042f,
      0.714f, 0.505f, 0.345f,
      0.783f, 0.290f, 0.734f,
      0.722f, 0.645f, 0.174f,
      0.302f, 0.455f, 0.848f,
      0.225f, 0.587f, 0.040f,
      0.517f, 0.713f, 0.338f,
      0.053f, 0.959f, 0.120f,
      0.393f, 0.621f, 0.362f,
      0.673f, 0.211f, 0.457f,
      0.820f, 0.883f, 0.371f,
      0.982f, 0.099f, 0.879f};
  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  max = 36 / 3;
  //printf("%d\n",max);
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  //------------------------------------------------------------
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
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
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(
      1,        // attribute. No particular reason for 1, but
                // must match the layout in the shader.
      3,        // size
      GL_FLOAT, // type
      GL_FALSE, // normalized?
      0,        // stride
      (void *)0 // array buffer offset
  );

  /* note that this is allowed, the call to glVertexAttribPointer 
       registered VBO as the vertex attribute's bound vertex buffer 
       object so afterwards we can safely unbind*/
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* You can unbind the VAO afterwards so other VAO calls won't 
       accidentally modify this VAO, but this rarely happens. Modifying 
       other VAOs requires a call to glBindVertexArray anyways so we 
       generally don't unbind VAOs (nor VBOs) when it's not directly 
       necessary.*/
  glBindVertexArray(0);

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
  // Get a handle for our "MVP" uniform
  unsigned int MatrixID = glGetUniformLocation(shaderProgram, "MVP");
  // create transformations
  // Model matrix : an identity matrix (model will be at the origin)
  /*glm::mat4 Model = glm::mat4(1.0f);
Model = glm::rotate(Model, glm::radians(35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
// note that we’re translating the scene in the reverse direction of
// where we want to move
glm::mat4 View = glm::mat4(1.0f);
View = glm::translate(View, glm::vec3(0.0f, 0.0f, -3.0f));
// Projection matrix : 45° Field of View, 4:3 ratio,
// display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::mat4(1.0f);
Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
// Our ModelViewProjection : multiplication of our 3 matrices
// Remember, matrix multiplication is the other way around
glm::mat4 MVP = Projection * View * Model;*/

  // Projection matrix : 45� Field of View, 4:3 ratio,
  // display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
  // View camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(4, 3, -3), // Camera is at (4,3,-3), in World Space
      glm::vec3(0, 0, 0),  // and looks at the origin
      glm::vec3(0, 1, 0)   // Head is up (set to 0,-1,0 to look upside-down)
  );
  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  // Remember, matrix multiplication is the other way around
  glm::mat4 MVP = Projection * View * Model;

  // Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive
  // vertices give a triangle. A cube has 6 faces with 2 triangles each, so this
  // makes 6*2=12 triangles, and 12*3 vertices
  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);
    // render
    // ------
    //glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //grey rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //green? rendering
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle: using shader program
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // update the uniform color
    float timeValue = glfwGetTime();
    float greenValue = sin(timeValue) / 2.0f + 0.3f;
    float redValue = cos(timeValue) / 2.0f + 0.4f;
    float blueValue = tan(timeValue) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    //glUniform4f(vertexColorLocation, redValue, greenValue, blueValue, 1.0f);
    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // also clear the depth buffer now!
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // seeing as we only have a single VAO there's no need to bind
    // it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 10000);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // glBindVertexArray(0); // no need to unbind it every time
    /* glfw: swap buffers and poll IO events (keys pressed/released, 
	 mouse moved etc.)
	 --------------------------------------------------------------*/
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

/* process all input: query GLFW whether relevant keys are pressed/released 
this frame and react accordingly 
-----------------------------------------------------------------------*/

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

/* glfw: whenever the window size changed (by OS or user resize) this
   callback function executes
   -------------------------------------------------------------------*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that
  // width and height will be significantly larger than specified on
  // retina displays.
  glViewport(0, 0, width, height);
}

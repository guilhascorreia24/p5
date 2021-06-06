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
#include "Sheep.h"
#include "Cinzas.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool collide);
void moveBlock(GLFWwindow *window, int key, int scancode, int action, int mods);
void reposition(glm::vec3 v, Scenery *l);
void atributes(Object b, unsigned int VBO);
void moveLights();
void changeLevel(GLFWwindow *window);
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
unsigned int MatrixID;
unsigned int shaderProgram;
Block block;
Plataform plat;
Scenery atual_level, level1, level2, level3;
bool colide_floor = false;
bool colide = false;
int level = 0;
Block lava;
glm::mat4 Model = glm::mat4(1.0f);
float mx = 10.0f;
bool b = true;
glm::vec3 lightPos(10.0f, 5.0f, 5.0f);
const char *fragmentShaderSource = "#version 330 core \n"
                                   "out vec4 FragColor;\n"

                                   "struct Material {\n"
                                   "vec3 ambient;\n"
                                   "vec3 diffuse;\n"
                                   "vec3 specular;\n"
                                   "float shininess;\n"
                                   "};\n"

                                   "struct DirLight {\n"
                                   "vec3 direction;\n"
                                   "vec3 position;\n"
                                   "vec3 ambient;\n"
                                   "vec3 diffuse;\n"
                                   "vec3 specular;\n"
                                   "};\n"

                                   "struct PointLight {\n"
                                   "vec3 position;\n"

                                   "float constant;\n"
                                   "float linear;\n"
                                   "float quadratic;\n"

                                   "vec3 ambient;\n"
                                   "vec3 diffuse;\n"
                                   "vec3 specular;\n"
                                   "};\n"

                                   //  "#define NR_POINT_LIGHTS 1\n"

                                   "in vec3 FragPos;\n"
                                   "in vec3 Normal;\n"
                                   "in vec2 TexCoords;\n"
                                   "uniform int nr_points;\n"
                                   "uniform vec3 viewPos;\n"
                                   "uniform DirLight dirLight;\n"
                                   "uniform PointLight pointLights[7];\n"
                                   "uniform Material material;\n"
                                   "uniform sampler2D texture1;\n"

                                   "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);\n"
                                   "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);\n"

                                   "void main()\n"
                                   "{\n"
                                   "vec3 norm = normalize(Normal);\n"
                                   "vec3 viewDir = normalize(viewPos - FragPos);\n"

                                   "vec3 result = CalcDirLight(dirLight, norm, viewDir);\n"
                                   "for(int i = 0; i < nr_points; i++)\n"
                                   "result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);\n"

                                   "FragColor = texture(texture1, TexCoords) * vec4(result, 1.0);\n"
                                   "}\n"

                                   "vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)\n"
                                   "{\n"
                                   "   vec3 ambient = light.ambient * material.ambient;\n"
                                   "   vec3 lightDir = normalize(light.position - FragPos);\n"
                                   "   vec3 norm = normalize(Normal);\n"
                                   "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                   "   vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
                                   "   vec3 reflectDir = reflect(-lightDir, norm);\n"
                                   "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
                                   "   vec3 specular = light.specular * (spec * material.specular);\n"
                                   "   vec3 result = ambient + diffuse + specular;\n"
                                   //------------------
                                   /* "vec3 lightDir = normalize(-light.direction);\n"
                                   "float diff = max(dot(normal, lightDir), 0.0);\n"
                                   "vec3 reflectDir = reflect(-lightDir, normal);\n"
                                   "float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
                                   "vec3 ambient = light.ambient  * material.ambient *vec3(texture(texture1,TexCoords));\n"
                                   "vec3 diffuse = light.diffuse * diff * material.diffuse * vec3(texture(texture1,TexCoords));\n"
                                   "vec3 specular = light.specular * spec * material.specular * vec3(texture(texture1,TexCoords));\n"
                                   "return (ambient + diffuse + specular);\n"*/
                                   "return result;\n"
                                   "}\n"

                                   "vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)\n"
                                   "{\n"
                                   "vec3 lightDir = normalize(light.position - fragPos);\n"

                                   "float diff = max(dot(normal, lightDir), 0.0);\n"
                                   "vec3 reflectDir = reflect(-lightDir, normal);\n"
                                   "float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
                                   "float distance = length(light.position - fragPos);\n"
                                   "float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  \n"
                                   /* "vec3 ambient = light.ambient * vec3(texture(texture1,TexCoords));\n"
                                   "vec3 diffuse = light.diffuse * diff * vec3(texture(texture1,TexCoords));\n"
                                   "vec3 specular = light.specular * spec * vec3(texture(texture1,TexCoords));\n"*/
                                   "vec3 ambient = light.ambient * material.ambient;\n"
                                   "vec3 diffuse = light.diffuse * diff * material.diffuse;\n"
                                   "vec3 specular = light.specular * spec * material.specular;\n"
                                   "ambient *= attenuation;\n"
                                   "diffuse *= attenuation;\n"
                                   "specular *= attenuation;\n"
                                   "return (ambient + diffuse + specular);\n"
                                   "}\n\0";

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 1) in vec3 vertexColor;\n"
                                 "layout (location = 2) in vec2 aTexCoord;\n"
                                 "layout (location = 3) in vec3 aNormal;\n"
                                 "out vec3 fragmentColor;\n"
                                 "out vec3 FragPos;\n"
                                 "out vec2 TexCoords;\n"
                                 "uniform mat4 MVP;\n"
                                 "out vec3 Normal;\n"
                                 "uniform mat4 Model;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = MVP*Model * vec4(aPos, 1.0);\n"
                                 "   fragmentColor = vertexColor;\n"
                                 "    TexCoords = aTexCoord;\n"
                                 "   Normal = mat3(transpose(inverse(Model))) * aNormal;\n"
                                 "   FragPos = vec3(Model * vec4(aPos, 1.0));\n"
                                 "}\0";
/*
// declare and define fshader, position in color vector declaration
// are RGBA from [0,1] simply in and out
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 FragPos;\n"
                                   "in vec3 Normal;\n"
                                   "in vec2 TexCoords;\n"
                                   "in vec4 FragPosLightSpace;\n"
                                   "uniform vec3 lightPos;\n"
                                   "uniform sampler2D diffuseTexture;\n"
                                   "uniform sampler2D shadowMap;\n"
                                   // material properties
                                   "struct Material {\n"
                                   "  vec3 ambient;\n"
                                   "  vec3 diffuse;\n"
                                   "  vec3 specular;\n"
                                   "  float shininess;\n"
                                   "};\n"
                                   // light source properties
                                   "struct Light {\n"
                                   "  vec3 ambient;\n"
                                   "  vec3 diffuse;\n"
                                   "  vec3 specular;\n"
                                   "  vec3 position;\n"
                                   "};\n"
                                   
                                   "float ShadowCalculation(vec4 fragPosLightSpace)\n"
                                   "{\n"
                                   // perform perspective divide
                                   "vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;\n"
                                   // transform to [0,1] range
                                   "projCoords = projCoords * 0.5 + 0.5;\n"
                                   // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
                                   "float closestDepth = texture(shadowMap, projCoords.xy).r;\n"
                                   // get depth of current fragment from light's perspective
                                   "float currentDepth = projCoords.z;\n"
                                   // check whether current frag pos is in shadow
                                   "float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;\n"

                                   "return shadow;\n"
                                   "}\n"

                                   "uniform vec3 viewPos;\n"
                                   "uniform Light light;\n"
                                   "uniform Material material;\n"
                                   "out vec4 FragColor;\n"
                                   "in vec2 TexCoord;\n"
                                   "in vec3 fragmentColor;\n"
                                   "uniform sampler2D texture1;\n"
                                   "void main()\n"
                                   "{\n"
                                   //
                                   //  ambient component
                                   "   vec3 ambient = light.ambient * material.ambient;\n"
                                   //
                                   // diffuse component
                                   // get incident light vector on fragment
                                   "   vec3 lightDir = normalize(light.position - FragPos);\n"
                                   // get normal at fragment
                                   "   vec3 norm = normalize(Normal);\n"
                                   // get cos of angle between normal and incident light from dot product
                                   "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                   // diffuse illumination = diffuse light * material diffuse properties * cos(angle)
                                   "   vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
                                   //
                                   // specular component
                                   // viewer direction at fragment position
                                   "   vec3 viewDir = normalize(viewPos - FragPos);\n"
                                   // get reflection direction
                                   "   vec3 reflectDir = reflect(-lightDir, norm);\n"
                                   // specular = cos(angle) power to material properties
                                   "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
                                   // diffuse illumination = diffuse light * material diffuse properties * cos(angle)
                                   // specular illumination = light specular * material specular properties * spec coefficient
                                   "   vec3 specular = light.specular * (spec * material.specular);\n"
                                   // sum of components and fragment out
                                   " float shadow = ShadowCalculation(FragPosLightSpace);\n"
                                   "   vec3 result = ambient +(1.0 - shadow)+ diffuse + specular;\n"
                                   "   FragColor = texture(texture1, TexCoord)*vec4(result,1.0f) ;\n"
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

  shaderProgram = glCreateProgram();
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

  const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
  unsigned int depthMapFBO;
  glGenFramebuffers(1, &depthMapFBO);
  // create depth texture
  unsigned int depthMap;
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // attach depth texture as FBO's depth buffer
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // delete shaders, we don't need them anymore
  Block block1 = Block("../../p5/objs/stoneBlock.obj", "../../p5/textures/predra.png");
  //printf("level1\n");
  Plataform plat1 = Plataform("../../p5/objs/level1.obj", "../../p5/textures/vidro.png");
  Plataform floor1 = Plataform("../../p5/objs/floor1.obj", "../../p5/textures/cimento.png");

  Block block2 = Block("../../p5/objs/wood.obj", "../../p5/textures/wood.png");
  //printf("level2\n");
  Plataform plat2 = Plataform("../../p5/objs/level2.obj", "../../p5/textures/relva.png");
  Plataform floor2 = Plataform("../../p5/objs/floor1.obj", "../../p5/textures/terra.png");
  lava = Block("../../p5/objs/lavaBlock.obj", "../../p5/textures/lava.png");
  plat2.setLavaBlocks(lava);
  Cinzas cinzas = Cinzas("../../p5/objs/cinzas.obj", "../../p5/textures/cinzas.png");
  //printf("lava\n");

  Block block3 = Block("../../p5/objs/palha.obj", "../../p5/textures/palha.png");
  //printf("level3\n");
  Plataform plat3 = Plataform("../../p5/objs/level3.obj", "../../p5/textures/vidro.png");
  Plataform floor3 = Plataform("../../p5/objs/floor1.obj", "../../p5/textures/relva.png");
  Sheep sheep = Sheep("../../p5/objs/patas_cabeca.obj", "../../p5/textures/pernas.png", "../../p5/objs/corpo.obj", "../../p5/textures/la_ovelha.jpg");

  //cout << (sizeof(Texture) + sizeof(struct Vertex) * 2) << std::endl;
  //printf("s1\n");
  level1 = Scenery(Scenery::Projection * Scenery::View * Model, block1, plat1, floor1);
  //printf("s2\n");
  level2 = Scenery(Scenery::Projection * Scenery::View * Model, block2, plat2, floor2);
  level2.setCinzas(cinzas);
  //printf("s3\n");
  level3 = Scenery(Scenery::Projection * Scenery::View * Model, block3, plat3, floor3, sheep);
  //level3 = Scenery(Scenery::Projection * Scenery::View * Model, block3, plat3, floor3);
  //------------------------------------------------------------
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  ////printf("oi\n");
  unsigned int VBO[12 + level2.plat.lavaBlocks.size()];
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

  glGenBuffers(1, &VBO[3]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level2.block.n_vertexes, level2.block.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[4]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level2.plat.n_vertexes, level2.plat.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[5]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level2.floor.n_vertexes, level2.floor.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[6]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level2.cinzas.n_vertexes, level2.cinzas.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  cout << level2.plat.lavaBlocks.size() << std::endl;
  for (int i = 7; i < 7 + level2.plat.lavaBlocks.size(); i++)
  {
    glGenBuffers(1, &VBO[i]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level2.plat.lavaBlocks.at(i - 7).n_vertexes, level2.plat.lavaBlocks.at(i - 7).vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
  }

  glGenBuffers(1, &VBO[7 + level2.plat.lavaBlocks.size()]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[7 + level2.plat.lavaBlocks.size()]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level3.block.n_vertexes, level3.block.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[7 + level2.plat.lavaBlocks.size() + 1]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[7 + level2.plat.lavaBlocks.size() + 1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level3.plat.n_vertexes, level3.plat.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[7 + level2.plat.lavaBlocks.size() + 2]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[7 + level2.plat.lavaBlocks.size() + 2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level3.floor.n_vertexes, level3.floor.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[7 + level2.plat.lavaBlocks.size() + 3]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[7 + level2.plat.lavaBlocks.size() + 3]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level3.sheep.body.n_vertexes, level3.sheep.body.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);

  glGenBuffers(1, &VBO[7 + level2.plat.lavaBlocks.size() + 4]);
  glBindBuffer(GL_ARRAY_BUFFER, VBO[7 + level2.plat.lavaBlocks.size() + 4]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(struct VertexColorTexture) * level3.sheep.head_mems.n_vertexes, level3.sheep.head_mems.vertex, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  ////printf("uuuu\n");
  cout << sizeof(VBO) << std::endl;

  level1.block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  reposition(level1.block.inicial_pos, &level1);
  level2.block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  reposition(level2.block.inicial_pos, &level2);
  level3.block.inicial_pos = glm::vec3(-4.85, 0, -1.9033);
  reposition(level3.block.inicial_pos, &level3);
  level3.sheep.inicial_pos = glm::vec3(-5, -6, 5);
  level3.sheep.Model = level3.sheep.Translate(level3.sheep.inicial_pos);
  level3.sheep.Model = level3.sheep.Rotation(-30, glm::vec3(0, 1, 0));
  atual_level = level1;
  //cout << atual_level.block.tostring() << std::endl;

  //atual_level.addObj(block);
  //timerun=glfwGetTime();
  MatrixID = glGetUniformLocation(shaderProgram, "MVP");
  bool sai = false;
  while (!glfwWindowShouldClose(window))
  {
    processInput(window, colide);
    moveLights();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //atual_level.block.Model = atual_level.block.Model * glm::rotate(glm::mat4(1), glm::radians(1.0f), glm::vec3(1, 1, 0));
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    //plataforma
    atributes(atual_level.plat, VBO[1 + level]);
    atual_level.plat.loadTextures();
    glDrawArrays(GL_TRIANGLES, 0, atual_level.plat.n_vertexes);

    //chao
    atributes(atual_level.floor, VBO[2 + level]);
    atual_level.floor.loadTextures();
    glDrawArrays(GL_TRIANGLES, 0, atual_level.floor.n_vertexes);

    //block or cinzas
    if (atual_level.block.burn && level == 3)
    {
      atributes(atual_level.cinzas, VBO[3 + level]);
      atual_level.cinzas.loadTextures();
      glDrawArrays(GL_TRIANGLES, 0, atual_level.cinzas.n_vertexes);
    }
    else
    {
      atributes(atual_level.block, VBO[0 + level]);
      atual_level.block.loadTextures();
      glDrawArrays(GL_TRIANGLES, 0, atual_level.block.n_vertexes);
    }

    //sheep
    if (level == 14)
    {
      //printf("sheep ini draw\n");
      atributes(atual_level.sheep.body, VBO[level + 3]);
      //printf("finish atributes\n");
      atual_level.sheep.body.loadTextures();
      //printf("finish loadtext1\n");
      glDrawArrays(GL_TRIANGLES, 0, atual_level.sheep.body.n_vertexes);
      //printf("head:sheep\n");
      atributes(atual_level.sheep.head_mems, VBO[level + 4]);
      //printf("finish atributes\n");
      atual_level.sheep.head_mems.loadTextures();
      //printf("finish loadtext2\n");
      glDrawArrays(GL_TRIANGLES, 0, atual_level.sheep.head_mems.n_vertexes);
      //printf("sheep\n");
      //atual_level.sheep.Moves_Random(atual_level.plat);
    }

    //blocks lava
    if (atual_level.plat.lavaBlocks.size() != 0)
    {
      for (int i = 0; i < atual_level.plat.lavaBlocks.size(); i++)
      {
        atributes(atual_level.plat.lavaBlocks.at(i), VBO[4 + i + level]);
        atual_level.plat.lavaBlocks.at(i).loadTextures();
        glDrawArrays(GL_TRIANGLES, 0, atual_level.plat.lavaBlocks.at(i).n_vertexes);
      }
    }

    // bind diffuse map
    /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, atual_level.block.texture);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, atual_level.block.texture);*/

    //queda do block
    if (!colide)
    {
      ////printf("oi\n");
      atual_level.block.Falling(glfwGetTime());
    }
    else
    {
      glfwSetTime(0);
    }
    colide = atual_level.BlockOverEdgesPrataform();

    //block fica deitado caso caia no chao
    if (atual_level.block.Collide(atual_level.floor))
    {
      colide = true;
      printf("floor\n");
      atual_level.block.standUP();
      changeLevel(window);
      atual_level.block.MVP = glm::rotate(atual_level.block.MVP, glm::radians(90.0f), glm::vec3(0, 0, 1));
      //atual_level.sheep.inicial_pos = glm::vec3(-5, -6, 5);
      printf("ENTROU");
      atual_level.sheep.Model = atual_level.sheep.Translate(glm::vec3(2, 0, 2));
      sai = true;
      // atual_level.sheep.Model = atual_level.sheep.Rotation(-30,glm::vec3(0,1,0));
      //atual_level.sheep.Moves_to_block(block.atual);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  for (int i = 0; i < 18; i++)
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
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && level != 0)
  {
    //Model = glm::mat4(1);
    atual_level = level1;
    Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    atual_level = Scenery(Scenery::Projection * Scenery::View, atual_level.block, atual_level.plat, atual_level.floor);
    reposition(atual_level.block.inicial_pos, &atual_level);
    glfwSetTime(0);
    mx = 10.0f;
    level = 0;
  }
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && level != 3)
  {
    Model = glm::mat4(1);
    atual_level = level2;
    Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    atual_level = Scenery(Scenery::Projection * Scenery::View, atual_level.block, atual_level.plat, atual_level.floor);
    atual_level.setCinzas(level2.cinzas);
    reposition(atual_level.block.inicial_pos, &atual_level);
    glfwSetTime(0);
    mx = 10.0f;
    level = 3;
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && level != 14)
  {
    Model = glm::mat4(1);
    atual_level = level3;
    Scenery::View = glm::lookAt(glm::vec3(5, 15, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    atual_level = Scenery(Scenery::Projection * Scenery::View, level3.block, level3.plat, level3.floor, level3.sheep);
    //printf("level3\n");
    reposition(atual_level.block.inicial_pos, &atual_level);
    glfwSetTime(0);
    mx = 10.0f;
    level = 14;
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
    atual_level.block.Moves(key);
  }
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
  {
    Model = glm::mat4(1);
    mx = 10.0f;
    atual_level.block.MVP = Scenery::Projection * Scenery::View;
    atual_level.block.burn = false;
    reposition(atual_level.block.inicial_pos, &atual_level);
    glfwSetTime(0);
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
  {
    //Model = glm::mat4(1);
    Scenery::View = glm::lookAt(glm::vec3(0, 15, 0.01), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    Cinzas c = atual_level.cinzas;
    atual_level = Scenery(Scenery::Projection * Scenery::View, atual_level.block, atual_level.plat, atual_level.floor);
    atual_level.setCinzas(c);
    //cout << atual_level.block.tostring() << std::endl;
  }

  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
  {
    //Model = glm::mat4(1);
    Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    Cinzas c = atual_level.cinzas;
    atual_level = Scenery(Scenery::Projection * Scenery::View, atual_level.block, atual_level.plat, atual_level.floor);
    atual_level.setCinzas(c);
  }
}
void reposition(glm::vec3 v, Scenery *l)
{

  double dist = INT_MAX;
  Block t = Block();
  l->block.inicial_pos[1] = 0;
  l->block.atual = l->block.inicial_pos;
  l->block.Model = l->block.Model * glm::translate(glm::mat4(1), l->block.inicial_pos);
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

void atributes(Object g, unsigned int VBO)
{
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &g.MVP[0][0]);
  ////printf("going do Model\n");
  glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "Model"), 1, GL_FALSE, &g.Model[0][0]);
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 50.0f);
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.direction"), 1, glm::value_ptr(normalize(-lightPos)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "dirLight.position"), 1, glm::value_ptr(lightPos));

  if (level == 3)
    glUniform1i(glGetUniformLocation(shaderProgram, "nr_points"), 7);
  else
    glUniform1i(glGetUniformLocation(shaderProgram, "nr_points"), 0);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(0).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[0].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[0].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[1].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(1).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[1].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[1].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[1].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[1].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[2].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(2).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[2].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[2].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[2].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[2].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[3].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(3).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[3].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[3].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[3].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[3].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[4].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(4).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[4].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[4].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[4].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[4].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[4].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[4].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[5].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(5).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[5].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[5].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[5].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[5].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[5].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[5].quadratic"), 0.032f);

  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[6].position"), 1, glm::value_ptr(level2.plat.lavaBlocks.at(6).atual));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[6].ambient"), 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[6].diffuse"), 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
  glUniform3fv(glGetUniformLocation(shaderProgram, "pointLights[6].specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[6].constant"), 1.0f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[6].linear"), 0.09f);
  glUniform1f(glGetUniformLocation(shaderProgram, "pointLights[6].quadratic"), 0.032f);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  ////printf("after light and model\n");
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  // normal attribute
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(struct VertexColorTexture), (void *)(8 * sizeof(float)));
  glEnableVertexAttribArray(3);
  // //printf("finish atributes\n");
}

void moveLights()
{
  //std::cout << "valor do mx: " << mx << std::endl;
  if (mx == 10.0f)
  {
    b = true;
  }
  else if (mx == -10.0f)
  {
    b = false;
  }

  if (b == true)
  {
    mx -= 0.02;
  }
  else
  {
    mx += 0.02;
  }
  mx = roundf(mx * 100) / 100;
  lightPos = glm::vec3((float)mx, 5.0f, 5.0f);
}

void changeLevel(GLFWwindow *window)
{
  // 1 ---- 0
  // 2 ---- 3
  // 3 ---- 14
  Block b = atual_level.plat.final_;
  if (atual_level.block.atual[0] > b.min.x && atual_level.block.atual[0] < b.max.x && atual_level.block.atual[2] > b.min.z && atual_level.block.atual[2] < b.max.z)
  {
    //Sleep(600);
    if (level == 0)
    {
      Model = glm::mat4(1);
      atual_level = level2;
      Scenery::View = glm::lookAt(glm::vec3(5, 10, 15), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
      atual_level = Scenery(Scenery::Projection * Scenery::View, atual_level.block, atual_level.plat, atual_level.floor);
      atual_level.setCinzas(level2.cinzas);
      reposition(atual_level.block.inicial_pos, &atual_level);
      glfwSetTime(0);
      mx = 10.0f;
      level = 3;
    }

    else if (level == 3)
    {
      Model = glm::mat4(1);
      atual_level = level3;
      Scenery::View = glm::lookAt(glm::vec3(5, 15, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
      atual_level = Scenery(Scenery::Projection * Scenery::View, level3.block, level3.plat, level3.floor, level3.sheep);
      //printf("level3\n");
      reposition(atual_level.block.inicial_pos, &atual_level);
      glfwSetTime(0);
      mx = 10.0f;
      level = 14;
    }

    else if(level == 14)
      glfwSetWindowShouldClose(window, true);
  }
}

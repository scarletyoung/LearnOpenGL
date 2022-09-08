#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shader.h"
#include "Camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastX = -1.0f;
float lastY = -1.0f;
bool isFirst = true;
//float yaw = -90.0f;
//float pitch = 0.0f;
//float fov = 45.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

inline float Clamp(float value, float bottom, float top)
{
  return std::max(std::min(value, top), bottom);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.OnKeyPress(CameraMovement::Forward, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.OnKeyPress(CameraMovement::Backward, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.OnKeyPress(CameraMovement::Left, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.OnKeyPress(CameraMovement::Right, deltaTime);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
  if (isFirst)
  {
    lastX = xPos;
    lastY = yPos;
    isFirst = false;
  }
  float xOffset = xPos - lastX;
  float yOffset = lastY - yPos;
  lastX = xPos;
  lastY = yPos;
  camera.OnMouseMovement(xOffset, yOffset);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
  camera.OnMouseScroll(yOffset);
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
  if (!window)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(window, MouseCallback);
  glfwSetScrollCallback(window, ScrollCallback);
  
  float vertices[] = {
    -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
                                               
    -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
                                               
    -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
                                               
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
                                               
    -0.5f, -0.5f, -0.5f, -1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, -1.0f,  1.0f,  0.0f, 1.0f,
                                               
    -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, -1.0f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
  };
  unsigned int indices[] = {
    0,1,3,
    1,2,3,

    4,5,7,
    5,6,7,
    
    8,9,11,
    9,10,11,

    12,13,15,
    13,14,15,
    
    16,17,19,
    17,18,19,

    20,21,23,
    21,22,23,
  };
  unsigned int vbo, vao, ebo;
  glCreateVertexArrays(1, &vao);
  glCreateBuffers(1, &vbo);
  glCreateBuffers(1, &ebo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  /*
   * 最后一次参数有以下几个选择。 
   * GL_STATIC_DRAW: 数据设置一次，最多被GPU使用几次
   * GL_STREAM_DRAW: 数据设置一次，可以使用许多次
   * GL_DYNAMIC_DRAW: 数据变化很多，可以使用许多次
   */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned int lightVbo, lightVao, lightEbo;
  glCreateVertexArrays(1, &lightVao);
  glCreateBuffers(1, &lightVbo);
  glCreateBuffers(1, &lightEbo);
  glBindVertexArray(lightVao);
  glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Shader lightShader("assets/shaders/lightShader.glsl");
  Shader shader("assets/shaders/baseShader.glsl");

  lightShader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);

  int width, height, channel;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load("assets/textures/container2.png", &width, &height, &channel, 4);
  
  unsigned int diffuseTexture;
  glCreateTextures(GL_TEXTURE_2D, 1, &diffuseTexture);
  glTextureStorage2D(diffuseTexture, 1, GL_RGBA8, width, height);
  glTextureParameteri(diffuseTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(diffuseTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(diffuseTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(diffuseTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureSubImage2D(diffuseTexture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(diffuseTexture);
  stbi_image_free(data);

  data = stbi_load("assets/textures/container2_specular.png", &width, &height, &channel, 4);
  unsigned int specularTexture;
  glCreateTextures(GL_TEXTURE_2D, 1, &specularTexture);
  glTextureStorage2D(specularTexture, 1, GL_RGBA8, width, height);
  glTextureParameteri(specularTexture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(specularTexture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(specularTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(specularTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureSubImage2D(specularTexture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(specularTexture);
  stbi_image_free(data);

  glBindTextureUnit(0, diffuseTexture);
  glBindTextureUnit(1, specularTexture);

  //shader.SetVec3("material.ambient", 0.1f, 0.05f, 0.031f);
  //shader.SetVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
  //shader.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
  shader.SetInt("material.diffuse", 0);
  shader.SetInt("material.specular", 1);
  shader.SetFloat("material.shininess", 32.0);

  glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  glm::vec3 pointLightPositions[] = {
    glm::vec3(0.7f,  0.2f,  2.0f),
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3(0.0f,  0.0f, -3.0f)
  };

  //directional light
  shader.SetVec3("dLight.direction", -0.2f, -1.0f, -0.3f);
  shader.SetVec3("dLight.ambient", 0.2f, 0.2f, 0.2f);
  shader.SetVec3("dLight.diffuse", 0.4f, 0.4f, 0.4f);
  shader.SetVec3("dLight.specular", 0.5f, 0.5f, 0.5f);
  // piont light 1
  shader.SetVec3("pLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
  shader.SetVec3("pLights[0].ambient", 0.05f, 0.05f, 0.05f);
  shader.SetVec3("pLights[0].diffuse", 0.8f, 0.8f, 0.8f);
  shader.SetVec3("pLights[0].specular", 1.0f, 1.0f, 1.0f);
  shader.SetFloat("pLights[0].constant", 1.0f);
  shader.SetFloat("pLights[0].linear", 0.09f);
  shader.SetFloat("pLights[0].quad", 0.032f);
  // piont light 2
  shader.SetVec3("pLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
  shader.SetVec3("pLights[1].ambient", 0.05f, 0.05f, 0.05f);
  shader.SetVec3("pLights[1].diffuse", 0.8f, 0.8f, 0.8f);
  shader.SetVec3("pLights[1].specular", 1.0f, 1.0f, 1.0f);
  shader.SetFloat("pLights[1].constant", 1.0f);
  shader.SetFloat("pLights[1].linear", 0.09f);
  shader.SetFloat("pLights[1].quad", 0.032f);
  // piont light 3
  shader.SetVec3("pLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
  shader.SetVec3("pLights[2].ambient", 0.05f, 0.05f, 0.05f);
  shader.SetVec3("pLights[2].diffuse", 0.8f, 0.8f, 0.8f);
  shader.SetVec3("pLights[2].specular", 1.0f, 1.0f, 1.0f);
  shader.SetFloat("pLights[2].constant", 1.0f);
  shader.SetFloat("pLights[2].linear", 0.09f);
  shader.SetFloat("pLights[2].quad", 0.032f);
  // piont light 4
  shader.SetVec3("pLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
  shader.SetVec3("pLights[3].ambient", 0.05f, 0.05f, 0.05f);
  shader.SetVec3("pLights[3].diffuse", 0.8f, 0.8f, 0.8f);
  shader.SetVec3("pLights[3].specular", 1.0f, 1.0f, 1.0f);
  shader.SetFloat("pLights[3].constant", 1.0f);
  shader.SetFloat("pLights[3].linear", 0.09f);
  shader.SetFloat("pLights[3].quad", 0.032f);
  // spot light
  shader.SetVec3("sLight.ambient", 0.0f, 0.0f, 0.0f);
  shader.SetVec3("sLight.diffuse", 1.0f, 1.0f, 1.0f);
  shader.SetVec3("sLight.specular", 1.0f, 1.0f, 1.0f);
  shader.SetFloat("sLight.constant", 1.0f);
  shader.SetFloat("sLight.linear", 0.09f);
  shader.SetFloat("sLight.quad", 0.032f);
  shader.SetFloat("sLight.cutoff", glm::cos(glm::radians(12.5f)));
  shader.SetFloat("sLight.outerCutoff", glm::cos(glm::radians(15.0f)));

  while (!glfwWindowShouldClose(window))
  {
    ProcessInput(window);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projection = camera.GetProjectionMatrix();
    glm::mat4 view = camera.GetViewMatrix();

    lightShader.Bind();
    
    lightShader.SetMat4("view", view);
    lightShader.SetMat4("projection", projection);
    glBindVertexArray(lightVao);
    for (int i = 0; i < 4; ++i)
    {
      glm::mat4 lightModel(1.0f);
      lightModel = glm::translate(lightModel, pointLightPositions[i]);
      lightModel = glm::scale(lightModel, glm::vec3(0.2f));
      lightShader.SetMat4("model", lightModel);
      glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
    }

    shader.Bind();
    auto camerePos = camera.GetPosition();
    shader.SetVec3("sLight.position", camerePos.x, camerePos.y, camerePos.z);
    auto front = camera.GetFront();
    shader.SetVec3("sLight.direction", front.x, front.y, front.z);
    shader.SetVec3("viewPos", camerePos.x, camerePos.y, camerePos.z);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    glBindVertexArray(vao);
    for (int i = 0; i < 10; i++)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetMat4("model", model);
      shader.SetMat4("nModel", glm::transpose(glm::inverse(model)));
      glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
    }
    
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();
  return 0;
}
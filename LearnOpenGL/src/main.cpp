#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  Shader shader("assets/shaders/baseShader.glsl");
  stbi_set_flip_vertically_on_load(true);
  int width, height, channel;
  unsigned char* data = stbi_load("assets/textures/wall.jpg", &width, &height, &channel, 4);
  unsigned int texture, texture2;
  glCreateTextures(GL_TEXTURE_2D, 1, &texture);
  /*
   * 这里第三个参数是internalformat，不能用GL_RGB，它是format
   */
  glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(texture);
  stbi_image_free(data);

  data = stbi_load("assets/textures/awesomeface.png", &width, &height, &channel, 4);
  glCreateTextures(GL_TEXTURE_2D, 1, &texture2);
  glTextureStorage2D(texture2, 1, GL_RGBA8, width, height);
  glTextureParameteri(texture2, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTextureParameteri(texture2, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTextureParameteri(texture2, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTextureParameteri(texture2, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTextureSubImage2D(texture2, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateTextureMipmap(texture2);
  stbi_image_free(data);

  glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

  while (!glfwWindowShouldClose(window))
  {
    processInput(window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = glfwGetTime();
    shader.Bind();
    glm::mat4 model = glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.5f, 1.0f, 0.0f));
    shader.SetInt("vTexture", 0);
    shader.SetInt("vTexture2", 1);
    shader.SetMat4("model", model);
    shader.SetMat4("view", view);
    shader.SetMat4("projection", projection);
    
    glBindTextureUnit(0, texture);
    glBindTextureUnit(1, texture2);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, nullptr);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteTextures(1, &texture);
  glfwTerminate();
  return 0;
}
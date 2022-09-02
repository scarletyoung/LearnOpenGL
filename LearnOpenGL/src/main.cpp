#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

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
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f 
  };
  unsigned int indices[] = {
    0,1,3,
    1,2,3
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos, 1.0);\n"
    "}\0";
  
  const char* fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "  FragColor = vec4(1.0, 0.5, 0.2, 1.0);"
    "}\0";

  unsigned int vs, fs;
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertexShaderSource, nullptr);
  glCompileShader(vs);
  int success;
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    GLint len = 0;
    glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &len);
    std::vector<GLchar> infoLog(len);
    glGetShaderInfoLog(vs, len, &len, &infoLog[0]);
    glDeleteShader(vs);
    std::cout << "Compile Shader Error:" << infoLog.data() << std::endl;
    return -1;
  }

  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if (success == GL_FALSE)
  {
    GLint len = 0;
    glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &len);
    std::vector<GLchar> infoLog(len);
    glGetShaderInfoLog(fs, len, &len, &infoLog[0]);
    glDeleteShader(vs);
    glDeleteShader(fs);
    std::cout << "Compile Shader Error:" << infoLog.data() << std::endl;
    return -1;
  }

  unsigned int program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (success == GL_FALSE)
  {
    int len = 0;
    glGetProgramiv(fs, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> infoLog(len);
    glGetProgramInfoLog(fs, len, &len, &infoLog[0]);
    glDeleteShader(vs);
    glDeleteShader(fs);
    std::cout << "Link Shader Error:" << infoLog.data() << std::endl;
    return -1;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);

  while (!glfwWindowShouldClose(window))
  {
    // input
            // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(program);
    glBindVertexArray(vao); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time 

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteProgram(program);
  glfwTerminate();
  return 0;
}
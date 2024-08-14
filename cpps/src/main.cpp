#include <glad/gl.h>

// Fix include order; glad must be included before GLFW
#include <GLFW/glfw3.h>

#include <iostream>

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

int main() {
  // Initialize GLFW
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW!" << std::endl;
    return -1;
  }

  // Set GLFW window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFW window
  GLFWwindow* window =
      glfwCreateWindow(800, 600, "Red Triangle", nullptr, nullptr);
  if (!window) {
    std::cerr << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Load OpenGL functions using GLAD
  if (!gladLoadGL(glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD!" << std::endl;
    return -1;
  }

  // Define the viewport dimensions
  glViewport(0, 0, 800, 600);

  // Compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  // Check for vertex shader compile errors
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  // Check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Link shaders to create a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Clean up shaders as they are linked into our program now and no longer
  // necessary
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Set up vertex data and buffers and configure vertex attributes
  float vertices[] = {
      0.0f,  0.5f,  0.0f,  // Top vertex
      -0.5f, -0.5f, 0.0f,  // Bottom left vertex
      0.5f,  -0.5f, 0.0f   // Bottom right vertex
  };

  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind VBO and VAO for safety
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Rendering commands here
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  // Terminate GLFW
  glfwTerminate();
  return 0;
}
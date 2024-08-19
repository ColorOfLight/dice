#include <GLES3/gl3.h>  // OpenGL ES 3.0 for WebGL 2.0
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <iostream>

// Vertex shader source code
const char* vertexShaderSource = R"(#version 300 es
    layout(location = 0) in vec4 aPos;
    void main()
    {
        gl_Position = aPos;
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(#version 300 es
    precision mediump float;
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

// Globals for the rendering loop
GLuint shaderProgram;
GLuint VAO;

void setupGraphics() {
  // Compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  // Check for vertex shader compile errors
  GLint success;
  GLchar infoLog[512];
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
  shaderProgram = glCreateProgram();
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
  GLfloat vertices[] = {
      0.0f,  0.5f,  0.0f,  // Top vertex
      -0.5f, -0.5f, 0.0f,  // Bottom left vertex
      0.5f,  -0.5f, 0.0f   // Bottom right vertex
  };

  GLuint VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Unbind the VBO and VAO for safety
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void renderFrame() {
  // Rendering commands here
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the triangle
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glBindVertexArray(0);
}

int main() {
  // Initialize the WebGL context using Emscripten
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.majorVersion = 2;  // WebGL 2.0
  attr.minorVersion = 0;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
      emscripten_webgl_create_context("#canvas", &attr);
  if (!context) {
    std::cerr << "Failed to create WebGL context!" << std::endl;
    return -1;
  }
  emscripten_webgl_make_context_current(context);

  // Set up graphics resources
  setupGraphics();

  // Main loop
  emscripten_set_main_loop(renderFrame, 0, 1);

  return 0;
}

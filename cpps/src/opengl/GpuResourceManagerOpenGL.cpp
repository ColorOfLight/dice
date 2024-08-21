/*
 * MIT License
 *
 * Copyright (c) 2024 Seongho Park
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "./GpuResourceManagerOpenGL.h"

#include <string>

GpuResourceManagerOpenGL::~GpuResourceManagerOpenGL() { cleanup(); }

unsigned int GpuResourceManagerOpenGL::createVertexObject(
    std::vector<float> vertices, unsigned int vertex_count) {
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 4, vertices.data(),
               GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);

  // Texture Coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(2);

  // Unbind VBO and VAO for safety
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  VertexObject vertex_object = {VAO, VBO, vertex_count};
  vertex_objects[vertex_object_index] = vertex_object;

  return vertex_object_index++;
}

unsigned int GpuResourceManagerOpenGL::createShaderProgram(ShaderType type) {
  std::string shader_prefix = SHADER_PREFIX;
  std::string fragment_precision = FRAGMENT_PRECISION;

  switch (type) {
    case ShaderType::BASIC: {
      std::string vertex_shader_content = R"(
          layout (location = 0) in vec3 aPos;
          void main()
          {
              gl_Position = vec4(aPos, 1.0);
          }
      )";

      // Fragment shader source code
      std::string fragment_shader_content = R"(
          out vec4 FragColor;
          void main()
          {
              FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
          }
      )";

      std::string full_vertex_source = shader_prefix + vertex_shader_content;
      std::string full_fragment_source =
          shader_prefix + fragment_precision + fragment_shader_content;

      const char* vertex_shader_source = full_vertex_source.c_str();
      const char* fragment_shader_source = full_fragment_source.c_str();

      return createShaderProgramWithSources(vertex_shader_source,
                                            fragment_shader_source);
    }
    default:
      throw std::runtime_error("ShaderProgramType is not supported");
  }

  throw std::runtime_error("Failed to create a shader program");
}

unsigned int GpuResourceManagerOpenGL::createShaderProgramWithSources(
    const char* vertex_shader_source, const char* fragment_shader_source) {
  int success;
  char infoLog[512];

  // Compile the vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
  glCompileShader(vertexShader);

  // Check for vertex shader compile errors
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" +
                             std::string(infoLog));
  }

  // Compile the fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
  glCompileShader(fragmentShader);

  // Check for fragment shader compile errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" +
                             std::string(infoLog));
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
    throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                             std::string(infoLog));
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}

void GpuResourceManagerOpenGL::deleteShaderProgram(ShaderType type) {
  unsigned int shader_program = shader_programs[type];
  glDeleteProgram(shader_program);
}

void GpuResourceManagerOpenGL::deleteVertexObject(unsigned int index) {
  VertexObject vertex_object = vertex_objects[index];
  glDeleteVertexArrays(1, &vertex_object.vao);
  glDeleteBuffers(1, &vertex_object.vbo);
}

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
#include <vector>

#include "./shader_source.h"

GpuResourceManagerOpenGL::~GpuResourceManagerOpenGL() { cleanup(); }

VertexObjectKey GpuResourceManagerOpenGL::createVertexObject(
    const Geometry& geometry) {
  auto& vertices = geometry.vertices;
  auto& indices = geometry.indices;

  unsigned int vertex_count =
      indices.has_value() ? indices.value().size() : vertices.size();

  std::vector<float> raw_vertices = std::vector<float>(vertices.size() * 8);
  for (int i = 0; i < vertices.size(); i++) {
    raw_vertices[i * 8] = vertices[i].position.x;
    raw_vertices[i * 8 + 1] = vertices[i].position.y;
    raw_vertices[i * 8 + 2] = vertices[i].position.z;
    raw_vertices[i * 8 + 3] = vertices[i].normal.x;
    raw_vertices[i * 8 + 4] = vertices[i].normal.y;
    raw_vertices[i * 8 + 5] = vertices[i].normal.z;
    raw_vertices[i * 8 + 6] = vertices[i].texture_coord.x;
    raw_vertices[i * 8 + 7] = vertices[i].texture_coord.y;
  }

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, raw_vertices.size() * 4, raw_vertices.data(),
               GL_STATIC_DRAW);

  if (indices.has_value()) {
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.value().size() * 4,
                 indices.value().data(), GL_STATIC_DRAW);
  }

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(4 * 3));
  glEnableVertexAttribArray(1);

  // Texture Coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void*)(4 * (3 + 3)));
  glEnableVertexAttribArray(2);

  // Unbind VBO and VAO for safety
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  VertexObject vertex_object = {VAO, VBO, vertex_count};

  if (indices.has_value()) {
    vertex_object.ebo = EBO;
  }

  vertex_objects[vertex_object_index] = vertex_object;

  return vertex_object_index++;
}

ShaderProgramId GpuResourceManagerOpenGL::createShaderProgram(
    MaterialType type) {
  ShaderSource shader_source = getShaderSource(type);

  return createShaderProgramWithSources(
      shader_source.vertex_shader_source.c_str(),
      shader_source.fragment_shader_source.c_str());
}

ShaderProgramId GpuResourceManagerOpenGL::createShaderProgramWithSources(
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

void GpuResourceManagerOpenGL::deleteShaderProgram(MaterialType type) {
  ShaderProgramId shader_program_id = shader_program_ids[type];
  glDeleteProgram(shader_program_id);
}

void GpuResourceManagerOpenGL::deleteVertexObject(VertexObjectKey index) {
  VertexObject vertex_object = vertex_objects[index];
  glDeleteVertexArrays(1, &vertex_object.vao);
  glDeleteBuffers(1, &vertex_object.vbo);
  if (vertex_object.ebo.has_value()) {
    glDeleteBuffers(1, &vertex_object.ebo.value());
  }
}

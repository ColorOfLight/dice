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

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "./shader_source.h"

GpuResourceManagerOpenGL::~GpuResourceManagerOpenGL() { cleanup(); }

VertexObject GpuResourceManagerOpenGL::createVertexObject(
    const Geometry* geometry) {
  GLuint vao_id, vbo_id, ebo_id;

  glGenVertexArrays(1, &vao_id);
  glGenBuffers(1, &vbo_id);
  glGenBuffers(1, &ebo_id);

  unsigned int vertex_count = geometry->getIndices().size();

  VertexObject vertex_object = {vao_id, vbo_id, ebo_id, vertex_count};

  return vertex_object;
}

void GpuResourceManagerOpenGL::updateVertexObject(const Geometry* geometry) {
  auto& vertex_object = vertex_objects[geometry];

  auto& vertices = geometry->getVertices();
  auto& indices = geometry->getIndices();

  GLuint vao_id = vertex_object.vao_id;
  GLuint vbo_id = vertex_object.vbo_id;
  GLuint ebo_id = vertex_object.ebo_id;
  unsigned int vertex_count = vertex_object.vertex_count;

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

  // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and
  // then configure vertex attributes(s).
  glBindVertexArray(vao_id);

  glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
  glBufferData(GL_ARRAY_BUFFER, raw_vertices.size() * 4, raw_vertices.data(),
               GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(),
               GL_STATIC_DRAW);

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

  // Unbind buffers safely
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

CameraUniformBufferId GpuResourceManagerOpenGL::createCameraUniformBuffer(
    const Camera* camera) {
  GLuint camera_uniform_buffer_id;
  glGenBuffers(1, &camera_uniform_buffer_id);

  return camera_uniform_buffer_id;
}

void GpuResourceManagerOpenGL::updateCameraUniformBuffer(const Camera* camera) {
  CameraUniformBufferId camera_uniform_buffer_id =
      camera_uniform_buffer_ids[camera];
  glBindBuffer(GL_UNIFORM_BUFFER, camera_uniform_buffer_id);
  glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(camera->getViewMatrix()));
  glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                  glm::value_ptr(camera->getProjectionMatrix()));

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

ModelUniformBufferId GpuResourceManagerOpenGL::createModelUniformBuffer(
    const Mesh* mesh) {
  GLuint model_uniform_buffer_id;
  glGenBuffers(1, &model_uniform_buffer_id);

  return model_uniform_buffer_id;
}

void GpuResourceManagerOpenGL::updateModelUniformBuffer(const Mesh* mesh) {
  ModelUniformBufferId model_uniform_buffer_id = model_uniform_buffer_ids[mesh];
  glBindBuffer(GL_UNIFORM_BUFFER, model_uniform_buffer_id);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                  glm::value_ptr(mesh->getModelMatrix()));

  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void GpuResourceManagerOpenGL::deleteShaderProgram(MaterialType type) {
  ShaderProgramId shader_program_id = shader_program_ids[type];
  glDeleteProgram(shader_program_id);
}

void GpuResourceManagerOpenGL::deleteVertexObject(const Geometry* geometry) {
  VertexObject vertex_object = vertex_objects[geometry];
  glDeleteVertexArrays(1, &vertex_object.vao_id);
  glDeleteBuffers(1, &vertex_object.vbo_id);
  glDeleteBuffers(1, &vertex_object.ebo_id);
}

void GpuResourceManagerOpenGL::deleteCameraUniformBuffer(const Camera* camera) {
  CameraUniformBufferId camera_uniform_buffer_id =
      camera_uniform_buffer_ids[camera];
  glDeleteBuffers(1, &camera_uniform_buffer_id);
}

void GpuResourceManagerOpenGL::deleteModelUniformBuffer(const Mesh* mesh) {
  ModelUniformBufferId model_uniform_buffer_id = model_uniform_buffer_ids[mesh];
  glDeleteBuffers(1, &model_uniform_buffer_id);
}

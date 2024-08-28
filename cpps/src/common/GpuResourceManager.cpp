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

#include "./GpuResourceManager.h"

GpuResourceManager::~GpuResourceManager() {
  // Empty definition
}

void GpuResourceManager::upsertVertexObject(const Geometry* geometry) {
  if (vertex_objects.find(geometry) == vertex_objects.end()) {
    vertex_objects[geometry] = createVertexObject(geometry);
  }
  updateVertexObject(geometry);
}

void GpuResourceManager::upsertCameraUniformBuffer(const Camera* camera) {
  if (camera_uniform_buffer_ids.find(camera) ==
      camera_uniform_buffer_ids.end()) {
    camera_uniform_buffer_ids[camera] = createCameraUniformBuffer(camera);
  }
  updateCameraUniformBuffer(camera);
}

void GpuResourceManager::upsertModelUniformBuffer(const Mesh* mesh) {
  if (model_uniform_buffer_ids.find(mesh) == model_uniform_buffer_ids.end()) {
    model_uniform_buffer_ids[mesh] = createModelUniformBuffer(mesh);
  }
  updateModelUniformBuffer(mesh);
}

ShaderProgramId GpuResourceManager::getShaderProgram(MaterialType type) {
  if (shader_program_ids.find(type) == shader_program_ids.end()) {
    shader_program_ids[type] = createShaderProgram(type);
  }
  return shader_program_ids[type];
}

const VertexObject& GpuResourceManager::getVertexObject(
    const Geometry* geometry) {
  return vertex_objects[geometry];
}

const CameraUniformBufferId GpuResourceManager::getCameraUniformBufferId(
    const Camera* camera) {
  return camera_uniform_buffer_ids[camera];
}

const ModelUniformBufferId GpuResourceManager::getModelUniformBufferId(
    const Mesh* mesh) {
  return model_uniform_buffer_ids[mesh];
}

const UniformBufferId GpuResourceManager::getUniformBufferId(void* key) {
  return uniform_buffer_ids[key];
}

void GpuResourceManager::cleanup() {
  for (auto& [type, _] : shader_program_ids) {
    deleteShaderProgram(type);
  }

  for (auto& [index, _] : vertex_objects) {
    deleteVertexObject(index);
  }

  for (auto& [index, _] : camera_uniform_buffer_ids) {
    deleteCameraUniformBuffer(index);
  }

  for (auto& [index, _] : model_uniform_buffer_ids) {
    deleteModelUniformBuffer(index);
  }
}

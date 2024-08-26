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

#pragma once

#include <map>
#include <memory>
#include <vector>

#include "./Camera.h"
#include "./Geometry.h"
#include "./Material.h"
#include "./util.h"

// Types for key of unordered_map
typedef unsigned int VertexObjectKey;

// Id for graphic resources
typedef unsigned int ShaderProgramId;
typedef unsigned int CameraUniformBufferId;

struct VertexObject {
  unsigned int vao_id;
  unsigned int vbo_id;
  unsigned int ebo_id;
  unsigned int vertex_count;
};

class GpuResourceManager {
 public:
  GpuResourceManager() {};
  virtual ~GpuResourceManager() = 0;

  ShaderProgramId getShaderProgram(MaterialType type);

  virtual VertexObject createVertexObject(const Geometry* geometry) = 0;
  virtual void updateVertexObject(const Geometry* geometry) = 0;
  void upsertVertexObject(const Geometry* geometry);
  const VertexObject& getVertexObject(const Geometry* geometry);

  virtual CameraUniformBufferId createCameraUniformBuffer(
      const Camera* camera) = 0;
  virtual void updateCameraUniformBuffer(const Camera* camera) = 0;
  void upsertCameraUniformBuffer(const Camera* camera);
  const CameraUniformBufferId getCameraUniformBufferId(const Camera* camera);

  void cleanup();

 protected:
  std::unordered_map<MaterialType, ShaderProgramId> shader_program_ids;
  UnorderedPointerMap<Geometry, VertexObject> vertex_objects;
  UnorderedPointerMap<Camera, CameraUniformBufferId> camera_uniform_buffer_ids;

 private:
  virtual ShaderProgramId createShaderProgram(MaterialType type) = 0;
  virtual void deleteShaderProgram(MaterialType type) = 0;
  virtual void deleteVertexObject(const Geometry* index) = 0;
  virtual void deleteCameraUniformBuffers(const Camera* index) = 0;
};

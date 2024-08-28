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
#include "./Mesh.h"
#include "./UniformDataObject.h"
#include "./util.h"

// Types for key of unordered_map
typedef unsigned int VertexObjectKey;

// Id for graphic resources
typedef unsigned int ShaderProgramId;

typedef unsigned int UniformBufferId;

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
  const VertexObject& getVertexObject(const Geometry* geometry);

  const UniformBufferId getUniformBufferId(
      const UniformDataObject* uniform_data_object);

  void upsertVertexObject(const Geometry* geometry);
  void upsertUniformBuffer(const UniformDataObject* uniform_data_object);

  void cleanup();

 private:
  virtual ShaderProgramId createShaderProgram(MaterialType type) = 0;
  virtual VertexObject createVertexObject(const Geometry* geometry) = 0;

  virtual UniformBufferId createUniformBuffer() = 0;

  virtual void updateVertexObject(const Geometry* geometry) = 0;
  virtual void updateUniformBuffer(UniformBufferId uniform_buffer_id,
                                   const void* data_ptr, size_t size) = 0;

  virtual void deleteShaderProgram(MaterialType type) = 0;
  virtual void deleteVertexObject(const Geometry* index) = 0;
  virtual void deleteUniformBuffer(UniformBufferId uniform_buffer_id) = 0;

 protected:
  std::unordered_map<MaterialType, ShaderProgramId> shader_program_ids;
  UnorderedPointerMap<Geometry, VertexObject> vertex_objects;
  UnorderedPointerMap<UniformDataObject, UniformBufferId> uniform_buffer_ids;
};

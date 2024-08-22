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

#include "./Geometry.h"
#include "./Material.h"

// Types for key of unordered_map
typedef unsigned int VertexObjectKey;

// Id for graphic resources
typedef unsigned int ShaderProgramId;

struct VertexObject {
  unsigned int vao;
  unsigned int vbo;
  unsigned int vertex_count;
  std::optional<unsigned int> ebo;
};

class GpuResourceManager {
 public:
  GpuResourceManager() : shader_program_ids() {};
  virtual ~GpuResourceManager() = 0;

  ShaderProgramId getShaderProgram(MaterialType type);
  virtual VertexObjectKey createVertexObject(const Geometry& geometry) = 0;
  const VertexObject& getVertexObject(VertexObjectKey index);

  void cleanup();

 protected:
  std::unordered_map<MaterialType, ShaderProgramId> shader_program_ids;
  std::unordered_map<VertexObjectKey, VertexObject> vertex_objects;
  unsigned int vertex_object_index = 0;

 private:
  virtual ShaderProgramId createShaderProgram(MaterialType type) = 0;
  virtual void deleteShaderProgram(MaterialType type) = 0;
  virtual void deleteVertexObject(VertexObjectKey index) = 0;
};

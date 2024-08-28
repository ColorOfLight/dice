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

#include <vector>

#include "./GpuResourceManager.h"
#include "./opengl.h"

class GpuResourceManagerOpenGL : public GpuResourceManager {
 public:
  GpuResourceManagerOpenGL() = default;
  ~GpuResourceManagerOpenGL() override;

 private:
  ShaderProgramId createShaderProgram(MaterialType type) override;
  ShaderProgramId createShaderProgramWithSources(
      const char* vertex_shader_source, const char* fragment_shader_source);

  VertexObject createVertexObject(const Geometry* geometry) override;
  void updateVertexObject(const Geometry* geometry) override;

  UniformBufferId createUniformBuffer() override;
  void updateUniformBuffer(UniformBufferId uniform_buffer_id,
                           const void* data_ptr, size_t size) override;

  void deleteShaderProgram(MaterialType type) override;
  void deleteVertexObject(const Geometry* geometry) override;
  void deleteUniformBuffer(UniformBufferId uniform_buffer_id) override;
};

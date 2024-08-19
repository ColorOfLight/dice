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

#if TARGET == OPENGL_GLAD_GLFW
#include <glad/gl.h>
#elif TARGET == WEBGL_EMSCRIPTEN
#include <GLES3/gl3.h>
#endif

class GpuResourceManagerOpenGL : public GpuResourceManager {
 public:
  GpuResourceManagerOpenGL() = default;
  ~GpuResourceManagerOpenGL() override;

  unsigned int createVertexObject(std::vector<float> vertices,
                                  unsigned int vertex_count) override;

 private:
  unsigned int createShaderProgram(ShaderType type) override;
  unsigned int createShaderProgramWithSources(
      const char* vertex_shader_source, const char* fragment_shader_source);
  void deleteShaderProgram(ShaderType type) override;
  void deleteVertexObject(unsigned int index) override;
};

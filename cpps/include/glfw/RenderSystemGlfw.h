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

#include <glad/gl.h>

// Fix include order; glad must be included before GLFW
#include <GLFW/glfw3.h>

#include <functional>

#include "./GpuResourceManager.h"
#include "./RenderSystem.h"
#include "./SceneManager.h"

class RenderSystemGlfw : public RenderSystem {
 public:
  RenderSystemGlfw(int width, int height);
  ~RenderSystemGlfw() override;

  void updateWindowSize(int width, int height) override;
  void runRenderLoop(
      const std::function<void(float, float)>& render_func) override;
  void drawTriangles(ShaderProgramId shader_program_id,
                     const VertexObject& vertex_object,
                     const std::unordered_map<UniformBlockType, unsigned int>
                         uniform_buffer_map) override;
  void setClearColor(const glm::vec4& color) override;

 private:
  GLFWwindow* window;
};

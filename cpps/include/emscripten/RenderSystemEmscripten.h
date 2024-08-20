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

#include <GLES3/gl3.h>  // OpenGL ES 3.0 for WebGL 2.0
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#include <functional>

#include "./GpuResourceManager.h"
#include "./RenderSystem.h"
#include "./SceneManager.h"

class RenderSystemEmscripten : public RenderSystem {
 public:
  RenderSystemEmscripten(int width, int height);
  ~RenderSystemEmscripten() override;

  void updateWindowSize(int width, int height) override;
  void runRenderLoop(std::function<void()> render_func) override;
  void drawTriangles(unsigned int shader_program,
                     const VertexObject& vertex_object) override;

 private:
};
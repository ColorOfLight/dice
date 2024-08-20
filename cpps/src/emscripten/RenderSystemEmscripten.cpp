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

#include "./RenderSystemEmscripten.h"

RenderSystemEmscripten::RenderSystemEmscripten(int width, int height) {
  // Initialize the WebGL context using Emscripten
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.majorVersion = 2;  // WebGL 2.0
  attr.minorVersion = 0;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
      emscripten_webgl_create_context("#canvas", &attr);
  if (!context) {
    throw std::runtime_error("Failed to create WebGL context!");
  }

  emscripten_set_canvas_element_size("#canvas", width, height);

  emscripten_webgl_make_context_current(context);
}

RenderSystemEmscripten::~RenderSystemEmscripten() {}

void RenderSystemEmscripten::updateWindowSize(int width, int height) {
  emscripten_set_canvas_element_size("#canvas", width, height);
}

std::function<void()> RenderSystemEmscripten::render_function_bridge;

void RenderSystemEmscripten::runRenderLoop(std::function<void()> render_func) {
  render_function_bridge = render_func;
  emscripten_set_main_loop(renderLoopCallbackBridge, 0, 1);
}

void RenderSystemEmscripten::renderLoopCallbackBridge() {
  if (render_function_bridge) {
    render_function_bridge();
  }
}

void RenderSystemEmscripten::drawTriangles(unsigned int shader_program,
                                           const VertexObject& vertex_object) {
  glUseProgram(shader_program);
  glBindVertexArray(vertex_object.vao);
  glDrawArrays(GL_TRIANGLES, 0, vertex_object.vertex_count);
}
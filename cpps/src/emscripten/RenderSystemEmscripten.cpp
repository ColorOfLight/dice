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

// Store the std::function in a static/global variable
static std::function<void()> stored_function;

extern "C" void renderFrame() {
  // Clear the color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (stored_function) {
    stored_function();  // Call the stored std::function
  }
}

// Convert std::function<void()> to void(*)()
void* getRenderFrame(const std::function<void()>& func) {
  stored_function = func;
  return (void*)renderFrame;
}

RenderSystemEmscripten::RenderSystemEmscripten(int width, int height) {
  // Initialize the WebGL context using Emscripten
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);
  attr.majorVersion = 2;  // WebGL 2.0
  attr.minorVersion = 0;

  emscripten_set_canvas_element_size("#canvas", width, height);

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context =
      emscripten_webgl_create_context("#canvas", &attr);
  if (!context) {
    throw std::runtime_error("Failed to create WebGL context!");
  }

  emscripten_webgl_make_context_current(context);

  // Enable depth testing (Z-buffer)
  glEnable(GL_DEPTH_TEST);
}

RenderSystemEmscripten::~RenderSystemEmscripten() {}

void RenderSystemEmscripten::updateWindowSize(int width, int height) {
  emscripten_set_canvas_element_size("#canvas", width, height);
}

void RenderSystemEmscripten::runRenderLoop(std::function<void()> render_func) {
  void (*render_frame)() = (void (*)())getRenderFrame(render_func);

  emscripten_set_main_loop(render_frame, 0, 1);
}

void RenderSystemEmscripten::drawTriangles(
    ShaderProgramId shader_program_id, const VertexObject& vertex_object,
    const std::vector<unsigned int> uniform_buffer_ids) {
  glUseProgram(shader_program_id);
  glBindVertexArray(vertex_object.vao_id);

  for (int i = 0; i < uniform_buffer_ids.size(); i++) {
    glBindBufferBase(GL_UNIFORM_BUFFER, i, uniform_buffer_ids[i]);
  }

  unsigned int cameraBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "CameraBlock");
  glUniformBlockBinding(shader_program_id, cameraBlockIndex, 0);

  unsigned int modelBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "ModelBlock");
  glUniformBlockBinding(shader_program_id, modelBlockIndex, 1);

  glDrawElements(GL_TRIANGLES, vertex_object.vertex_count, GL_UNSIGNED_INT,
                 (void*)0);
}
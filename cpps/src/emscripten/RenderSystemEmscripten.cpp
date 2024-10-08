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
static std::function<void(float, float)> stored_function;
static double start_time = emscripten_get_now();
static double prev_time = start_time;

extern "C" void renderFrame() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double current_time = emscripten_get_now();
  float elapsed_time = static_cast<float>(current_time - start_time);
  float delta_time = static_cast<float>(current_time - prev_time);

  if (stored_function) {
    stored_function(elapsed_time, delta_time);
  }

  prev_time = current_time;
}

// Convert std::function<void()> to void(*)()
void* getRenderFrame(const std::function<void(float, float)>& func) {
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

void RenderSystemEmscripten::runRenderLoop(
    const std::function<void(float, float)>& render_func) {
  void (*render_frame)() = (void (*)())getRenderFrame(render_func);

  emscripten_set_main_loop(render_frame, 0, 1);
}

void RenderSystemEmscripten::drawTriangles(
    ShaderProgramId shader_program_id, const VertexObject& vertex_object,
    const std::unordered_map<UniformBlockType, unsigned int>
        uniform_buffer_map) {
  glUseProgram(shader_program_id);
  glBindVertexArray(vertex_object.vao_id);

  int uniform_binging_point = 0;
  for (auto& [block_type, buffer_id] : uniform_buffer_map) {
    glBindBufferBase(GL_UNIFORM_BUFFER, uniform_binging_point, buffer_id);
    unsigned int block_index = glGetUniformBlockIndex(
        shader_program_id, getUniformBlockName(block_type).c_str());
    glUniformBlockBinding(shader_program_id, block_index,
                          uniform_binging_point);
    uniform_binging_point++;
  }

  glDrawElements(GL_TRIANGLES, vertex_object.vertex_count, GL_UNSIGNED_INT, 0);
}

void RenderSystemEmscripten::setClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

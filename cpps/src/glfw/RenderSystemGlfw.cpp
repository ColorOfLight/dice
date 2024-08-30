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

#include "./RenderSystemGlfw.h"

RenderSystemGlfw::RenderSystemGlfw(int width, int height) {
  // Initialize GLFW
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW!");
  }

  // Set GLFW window properties
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create a GLFW window
  window = glfwCreateWindow(width, height, "Dice", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window!");
  }

  glfwMakeContextCurrent(window);

  // Load OpenGL functions using GLAD
  if (!gladLoadGL(glfwGetProcAddress)) {
    throw std::runtime_error("Failed to initialize GLAD!");
  }

  glEnable(GL_DEPTH_TEST);
}

RenderSystemGlfw::~RenderSystemGlfw() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void RenderSystemGlfw::updateWindowSize(int width, int height) {
  glfwSetWindowSize(window, width, height);
}

void RenderSystemGlfw::runRenderLoop(
    const std::function<void(float, float)>& render_func) {
  double start_time = glfwGetTime() * 1000.0;
  double current_time;
  double prev_time = start_time;

  while (!glfwWindowShouldClose(window)) {
    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Rendering commands here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    current_time = glfwGetTime() * 1000.0;

    render_func(static_cast<float>(current_time - start_time),
                static_cast<float>(current_time - prev_time));

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();

    prev_time = current_time;
  }
}

void RenderSystemGlfw::drawTriangles(
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

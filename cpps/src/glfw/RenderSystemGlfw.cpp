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

void RenderSystemGlfw::runRenderLoop(std::function<void()> render_func) {
  // // Main loop
  while (!glfwWindowShouldClose(window)) {
    // Input
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    // Rendering commands here
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_func();

    // Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
}

void RenderSystemGlfw::drawTriangles(
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

  unsigned int ambientLightBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "AmbientLightBlock");
  glUniformBlockBinding(shader_program_id, ambientLightBlockIndex, 1);

  unsigned int directionalLightBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "DirectionalLightBlock");
  glUniformBlockBinding(shader_program_id, directionalLightBlockIndex, 2);

  unsigned int modelBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "ModelBlock");
  glUniformBlockBinding(shader_program_id, modelBlockIndex, 3);

  unsigned int materialBlockIndex =
      glGetUniformBlockIndex(shader_program_id, "MaterialBlock");
  glUniformBlockBinding(shader_program_id, materialBlockIndex, 4);

  glDrawElements(GL_TRIANGLES, vertex_object.vertex_count, GL_UNSIGNED_INT, 0);
}
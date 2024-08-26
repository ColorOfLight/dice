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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "./SceneObject.h"

class Camera : public SceneObject {
 public:
  Camera() {
    setView(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  }

  void setView(const glm::vec3& eye, const glm::vec3& center,
               const glm::vec3& up) {
    view_matrix = glm::lookAt(eye, center, up);
    view_vector = eye - center;

    need_to_update = true;
  }

  const glm::mat4& getProjectionMatrix() const { return projection_matrix; };
  const glm::mat4& getViewMatrix() const { return view_matrix; };

 protected:
  glm::mat4 projection_matrix;

 private:
  glm::mat4 view_matrix;
  glm::vec3 view_vector;
};

class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(float fov, float aspect_ratio, float near = 0.1,
                    float far = 100) {
    setProjection(fov, aspect_ratio, near, far);
  }

  void setProjection(float fov, float aspect_ratio, float near, float far) {
    projection_matrix = glm::perspective(fov, aspect_ratio, near, far);

    need_to_update = true;
  }
};

class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(float left, float right, float bottom, float top,
                     float near = 0.1, float far = 1) {
    setProjection(left, right, bottom, top, near, far);
  }

  void setProjection(float left, float right, float bottom, float top,
                     float near, float far) {
    projection_matrix = glm::ortho(left, right, bottom, top, near, far);

    need_to_update = true;
  }
};
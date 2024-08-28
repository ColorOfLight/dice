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
#include "./UniformDataObject.h"

struct CameraUniformData {
  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;
  alignas(16) glm::vec3 eye;
};

class Camera : public SceneObject, public UniformDataObject {
 public:
  Camera()
      : UniformDataObject(&camera_uniform_data, sizeof(CameraUniformData)) {
    lookAt(glm::vec3(0, 0, 1), glm::vec3(0, 0, 0));
    camera_uniform_data.projection_matrix = glm::mat4(1.0f);
  }

  void lookAt(const glm::vec3& eye, const glm::vec3& center);
  void lookAt(const glm::vec3& eye, const glm::vec3& center,
              const glm::vec3& up);

  const glm::mat4& getProjectionMatrix() const {
    return camera_uniform_data.projection_matrix;
  };
  const glm::mat4& getViewMatrix() const {
    return camera_uniform_data.view_matrix;
  };

 protected:
  CameraUniformData camera_uniform_data;
};

class PerspectiveCamera : public Camera {
 public:
  PerspectiveCamera(float fov, float aspect_ratio, float near = 0.1,
                    float far = 100) {
    setProjection(fov, aspect_ratio, near, far);
  }

  void setProjection(float fov, float aspect_ratio, float near, float far) {
    camera_uniform_data.projection_matrix =
        glm::perspective(fov, aspect_ratio, near, far);

    needs_to_update = true;
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
    camera_uniform_data.projection_matrix =
        glm::ortho(left, right, bottom, top, near, far);

    needs_to_update = true;
  }
};
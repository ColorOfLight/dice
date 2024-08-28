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

#include "./SceneObject.h"
#include "./UniformDataObject.h"

class Light : public SceneObject, public UniformDataObject {
 public:
  Light(void* uniform_data_ptr, size_t uniform_data_size)
      : UniformDataObject(uniform_data_ptr, uniform_data_size) {}
};

struct AmbientLightUniformData {
  alignas(16) glm::vec3 color;
  alignas(16) float intensity;
};

class AmbientLight : public Light {
 public:
  AmbientLight(float intensity, const glm::vec3& color)
      : Light(&uniform_data, sizeof(AmbientLightUniformData)),
        uniform_data({color, intensity}) {}
  void setIntensity(float intensity);
  void setColor(const glm::vec3& color);

 private:
  AmbientLightUniformData uniform_data;
};

struct DirectionalLightUniformData {
  alignas(16) glm::vec3 color;
  alignas(16) glm::vec3 direction;
  alignas(16) float intensity;
};

class DirectionalLight : public Light {
 public:
  DirectionalLight(float intensity, const glm::vec3& color,
                   const glm::vec3& direction)
      : Light(&uniform_data, sizeof(DirectionalLightUniformData)),
        uniform_data({color, direction, intensity}) {}
  void setIntensity(float intensity);
  void setColor(const glm::vec3& color);
  void setDirection(const glm::vec3& direction);

 private:
  DirectionalLightUniformData uniform_data;
};

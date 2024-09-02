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
#include <glm/gtc/quaternion.hpp>

#include "./Geometry.h"
#include "./Material.h"
#include "./SceneObject.h"
#include "./UniformDataObject.h"

struct MeshUniformData {
  glm::mat4 model_matrix;
};

class Mesh : public SceneObject, public UniformDataObject {
 public:
  Mesh(Geometry& geometry, Material& material)
      : geometry(geometry),
        material(material),
        UniformDataObject(&uniform_data, sizeof(MeshUniformData)) {}

  const glm::mat4& getModelMatrix() const { return uniform_data.model_matrix; }

  void translate(const glm::vec3& translation);
  void scale(const glm::vec3& scaling);
  void rotate(float angle, const glm::vec3& axis);

 private:
  void updateModelMatrix();

 public:
  std::reference_wrapper<Geometry> geometry;
  std::reference_wrapper<Material> material;

 private:
  MeshUniformData uniform_data = {glm::mat4(1.0f)};
  glm::vec3 scale_vector = glm::vec3(1.0f);
  glm::vec3 translate_vector = glm::vec3(0.0f);
  glm::quat rotate_quaternion = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
};

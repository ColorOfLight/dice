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
#include <memory>

#include "./SceneObject.h"
#include "./UniformDataObject.h"

enum class MaterialType : size_t {
  BASIC = 0,
  NORMAL = 1,
  TEXTURE_COORD = 2,
  SINGLE_COLOR = 3
};

class Material : public SceneObject, public UniformDataObject {
 public:
  Material(void* uniform_data_ptr = nullptr, size_t uniform_data_size = 0)
      : UniformDataObject(uniform_data_ptr, uniform_data_size) {};

  MaterialType getType() const { return type; };

 protected:
  MaterialType type;
};

class BasicMaterial : public Material {
 public:
  BasicMaterial() { type = MaterialType::BASIC; };
};

class NormalMaterial : public Material {
 public:
  NormalMaterial() { type = MaterialType::NORMAL; };
};

class TextureCoordMaterial : public Material {
 public:
  TextureCoordMaterial() { type = MaterialType::TEXTURE_COORD; };
};

struct SingleColorMaterialUniformData {
  glm::vec3 color;
};

class SingleColorMaterial : public Material {
 public:
  SingleColorMaterial(glm::vec3 color = glm::vec3(1.0f))
      : Material(&uniform_data, sizeof(SingleColorMaterialUniformData)),
        uniform_data({color}) {
    type = MaterialType::SINGLE_COLOR;
  };
  void setColor(const glm::vec3& color);

 private:
  SingleColorMaterialUniformData uniform_data;
};

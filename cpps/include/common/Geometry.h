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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <optional>
#include <vector>

#include "./SceneObject.h"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texture_coord;
};

class Geometry : public SceneObject {
 public:
  Geometry() {};

  const std::vector<Vertex>& getVertices() const { return vertices; };
  const std::optional<std::vector<unsigned int>>& getIndices() const {
    return indices;
  };

 protected:
  std::vector<Vertex> vertices;
  std::optional<std::vector<unsigned int>> indices;
};

class TriangleGeometry : public Geometry {
 public:
  TriangleGeometry() {
    vertices = {
        {{0.0f, 0.5f, 0.0f}, {0.0f, 0.1f, 0.0f}, {0.5f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.1f, 0.0f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.1f, 0.0f}, {0.0f, 0.0f}},
    };
  };
};

class CubeGeometry : public Geometry {
 public:
  CubeGeometry(float width = 1.0f, float height = 1.0f, float depth = 1.f,
               int width_segments = 1, int height_segments = 1,
               int depth_segments = 1);
};

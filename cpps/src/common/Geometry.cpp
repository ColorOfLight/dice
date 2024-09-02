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

#include "./Geometry.h"

#include <glm/glm.hpp>

std::vector<Vertex> generateCubePlaneVertices(
    const glm::vec3& right, const glm::vec3& up, float right_size,
    float up_size, float front_size, int width_segments, int height_segments) {
  int vertex_count = (width_segments + 1) * (height_segments + 1);
  std::vector<Vertex> vertices(vertex_count);

  glm::vec3 front =
      glm::cross(right, up);  // Calculate the front direction (normal)

  for (int j = 0; j <= height_segments; ++j) {
    for (int i = 0; i <= width_segments; ++i) {
      float xPos = right_size * (static_cast<float>(i) / width_segments - 0.5f);
      float yPos = up_size * (static_cast<float>(j) / height_segments - 0.5f);

      glm::vec3 position =
          right * xPos + up * yPos + front * (front_size / 2.0f);

      Vertex vertex = {
          .position = position,
          .normal = front,
          .texture_coord = glm::vec2(static_cast<float>(i) / width_segments,
                                     static_cast<float>(j) / height_segments),
      };

      vertices[j * (width_segments + 1) + i] = vertex;
    }
  }

  return vertices;
}

std::vector<unsigned int> generatePlaneIndices(int right_segments,
                                               int up_segments,
                                               int offset = 0) {
  int index_count = right_segments * up_segments *
                    6;  // Each quad has 6 indices (2 triangles)
  std::vector<unsigned int> indices(index_count);

  int index = 0;
  for (int j = 0; j < up_segments; ++j) {
    for (int i = 0; i < right_segments; ++i) {
      int left_bottom_index = j * (right_segments + 1) + i;
      int right_bottom_index = j * (right_segments + 1) + i + 1;
      int left_top_index = (j + 1) * (right_segments + 1) + i;
      int right_top_index = (j + 1) * (right_segments + 1) + i + 1;

      // First triangle (left_bottom_index, right_bottom_index, left_top_index)
      indices[index++] = left_bottom_index + offset;
      indices[index++] = right_bottom_index + offset;
      indices[index++] = left_top_index + offset;

      // Second triangle (right_bottom_index, right_top_index, left_top_index)
      indices[index++] = right_bottom_index + offset;
      indices[index++] = right_top_index + offset;
      indices[index++] = left_top_index + offset;
    }
  }

  return indices;
}

CubeGeometry::CubeGeometry(float width, float height, float depth,
                           int width_segments, int height_segments,
                           int depth_segments) {
  std::vector<Vertex> front_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width, height,
      depth, width_segments, height_segments);

  std::vector<Vertex> back_vertices = generateCubePlaneVertices(
      glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width, height,
      depth, width_segments, height_segments);

  std::vector<Vertex> right_vertices = generateCubePlaneVertices(
      glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), depth, height,
      width, depth_segments, height_segments);

  std::vector<Vertex> left_vertices = generateCubePlaneVertices(
      glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), depth, height,
      width, depth_segments, height_segments);

  std::vector<Vertex> top_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), width, depth,
      height, width_segments, depth_segments);

  std::vector<Vertex> bottom_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), width, depth,
      height, width_segments, depth_segments);

  std::vector<Vertex> vertices;
  vertices.insert(vertices.end(), front_vertices.begin(), front_vertices.end());
  vertices.insert(vertices.end(), back_vertices.begin(), back_vertices.end());
  vertices.insert(vertices.end(), right_vertices.begin(), right_vertices.end());
  vertices.insert(vertices.end(), left_vertices.begin(), left_vertices.end());
  vertices.insert(vertices.end(), top_vertices.begin(), top_vertices.end());
  vertices.insert(vertices.end(), bottom_vertices.begin(),
                  bottom_vertices.end());

  int index_offset = 0;

  std::vector<unsigned int> front_indices =
      generatePlaneIndices(width_segments, height_segments, index_offset);

  index_offset += front_vertices.size();

  std::vector<unsigned int> back_indices =
      generatePlaneIndices(width_segments, height_segments, index_offset);

  index_offset += back_vertices.size();

  std::vector<unsigned int> right_indices =
      generatePlaneIndices(depth_segments, height_segments, index_offset);

  index_offset += right_vertices.size();

  std::vector<unsigned int> left_indices =
      generatePlaneIndices(depth_segments, height_segments, index_offset);

  index_offset += left_vertices.size();

  std::vector<unsigned int> top_indices =
      generatePlaneIndices(width_segments, depth_segments, index_offset);

  index_offset += top_vertices.size();

  std::vector<unsigned int> bottom_indices =
      generatePlaneIndices(width_segments, depth_segments, index_offset);

  std::vector<unsigned int> indices;
  indices.insert(indices.end(), front_indices.begin(), front_indices.end());
  indices.insert(indices.end(), back_indices.begin(), back_indices.end());
  indices.insert(indices.end(), right_indices.begin(), right_indices.end());
  indices.insert(indices.end(), left_indices.begin(), left_indices.end());
  indices.insert(indices.end(), top_indices.begin(), top_indices.end());
  indices.insert(indices.end(), bottom_indices.begin(), bottom_indices.end());

  this->vertices = std::move(vertices);
  this->indices = std::move(indices);
}

PlaneGeometry::PlaneGeometry(float width, float height, int width_segments,
                             int height_segments) {
  vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width, height,
      0.0f, width_segments, height_segments);

  indices = generatePlaneIndices(width_segments, height_segments);
}

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
    const glm::vec3& right, const glm::vec3& up, float rightSize, float upSize,
    float frontSize, int widthSegments, int heightSegments) {
  int vertexCount = (widthSegments + 1) * (heightSegments + 1);
  std::vector<Vertex> vertices(vertexCount);

  glm::vec3 front =
      glm::cross(right, up);  // Calculate the front direction (normal)

  for (int j = 0; j <= heightSegments; ++j) {
    for (int i = 0; i <= widthSegments; ++i) {
      float xPos = rightSize * (static_cast<float>(i) / widthSegments - 0.5f);
      float yPos = upSize * (static_cast<float>(j) / heightSegments - 0.5f);

      glm::vec3 positionVector =
          right * xPos + up * yPos + front * (frontSize / 2.0f);

      Vertex vertex = {
          .position = positionVector,
          .normal = front,
          .texture_coord = glm::vec2(static_cast<float>(i) / widthSegments,
                                     static_cast<float>(j) / heightSegments),
      };

      vertices[j * (widthSegments + 1) + i] = vertex;
    }
  }

  return vertices;
}

std::vector<unsigned int> generatePlaneIndices(int rightSegments,
                                               int upSegments, int offset = 0) {
  int indexCount =
      rightSegments * upSegments * 6;  // Each quad has 6 indices (2 triangles)
  std::vector<unsigned int> indices(indexCount);

  int index = 0;
  for (int j = 0; j < upSegments; ++j) {
    for (int i = 0; i < rightSegments; ++i) {
      int leftBottom = j * (rightSegments + 1) + i;
      int rightBottom = j * (rightSegments + 1) + i + 1;
      int leftTop = (j + 1) * (rightSegments + 1) + i;
      int rightTop = (j + 1) * (rightSegments + 1) + i + 1;

      // First triangle (leftBottom, rightBottom, leftTop)
      indices[index++] = leftBottom + offset;
      indices[index++] = rightBottom + offset;
      indices[index++] = leftTop + offset;

      // Second triangle (rightBottom, rightTop, leftTop)
      indices[index++] = rightBottom + offset;
      indices[index++] = rightTop + offset;
      indices[index++] = leftTop + offset;
    }
  }

  return indices;
}

CubeGeometry::CubeGeometry(float width, float height, float depth,
                           unsigned int widthSegments,
                           unsigned int heightSegments,
                           unsigned int depthSegments) {
  std::vector<Vertex> front_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width, height,
      depth, widthSegments, heightSegments);

  std::vector<Vertex> back_vertices = generateCubePlaneVertices(
      glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), width, height,
      depth, widthSegments, heightSegments);

  std::vector<Vertex> right_vertices = generateCubePlaneVertices(
      glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), depth, height,
      width, depthSegments, heightSegments);

  std::vector<Vertex> left_vertices = generateCubePlaneVertices(
      glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), depth, height,
      width, depthSegments, heightSegments);

  std::vector<Vertex> top_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), width, depth,
      height, widthSegments, depthSegments);

  std::vector<Vertex> bottom_vertices = generateCubePlaneVertices(
      glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), width, depth,
      height, widthSegments, depthSegments);

  std::vector<Vertex> vertices;
  vertices.insert(vertices.end(), front_vertices.begin(), front_vertices.end());
  vertices.insert(vertices.end(), back_vertices.begin(), back_vertices.end());
  vertices.insert(vertices.end(), right_vertices.begin(), right_vertices.end());
  vertices.insert(vertices.end(), left_vertices.begin(), left_vertices.end());
  vertices.insert(vertices.end(), top_vertices.begin(), top_vertices.end());
  vertices.insert(vertices.end(), bottom_vertices.begin(),
                  bottom_vertices.end());

  int indexOffset = 0;

  std::vector<unsigned int> front_indices =
      generatePlaneIndices(widthSegments, heightSegments, indexOffset);

  indexOffset += front_vertices.size();

  std::vector<unsigned int> back_indices =
      generatePlaneIndices(widthSegments, heightSegments, indexOffset);

  indexOffset += back_vertices.size();

  std::vector<unsigned int> right_indices =
      generatePlaneIndices(depthSegments, heightSegments, indexOffset);

  indexOffset += right_vertices.size();

  std::vector<unsigned int> left_indices =
      generatePlaneIndices(depthSegments, heightSegments, indexOffset);

  indexOffset += left_vertices.size();

  std::vector<unsigned int> top_indices =
      generatePlaneIndices(widthSegments, depthSegments, indexOffset);

  indexOffset += top_vertices.size();

  std::vector<unsigned int> bottom_indices =
      generatePlaneIndices(widthSegments, depthSegments, indexOffset);

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
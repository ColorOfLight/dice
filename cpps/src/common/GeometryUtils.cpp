/*
 * MIT License
 *
 * Copyright (c) 2024 Seongho Park
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "S
 * oftware"), to deal
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

#include "./GeometryUtils.h"

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <vector>

Geometry GeometryUtils::loadObjToGeometry(const std::string& obj_path) {
  // Create an instance of the Assimp importer class
  Assimp::Importer importer;

  // Load the OBJ file with minimal processing needed
  const aiScene* scene =
      importer.ReadFile(obj_path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                      aiProcess_GenNormals);

  // Check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    throw std::runtime_error(importer.GetErrorString());
  }

  // Assuming we're only interested in the first mesh (OBJ typically has one
  // mesh)
  aiMesh* mesh = scene->mMeshes[0];

  // Extract vertices
  std::vector<Vertex> vertices = std::vector<Vertex>(mesh->mNumVertices);
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y,
                                mesh->mVertices[i].z);
    vertex.normal = mesh->HasNormals()
                        ? glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                    mesh->mNormals[i].z)
                        : glm::vec3(0.0f);
    vertex.texture_coord = mesh->mTextureCoords[0]
                               ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                           mesh->mTextureCoords[0][i].y)
                               : glm::vec2(0.0f);

    vertices[i] = vertex;
  }

  // Extract indices (faces)
  std::vector<unsigned int> indices;
  for (int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    if (face.mNumIndices <= 2) {
      throw std::runtime_error("Face must have at least 3 indices");
    }

    auto& first_index = face.mIndices[0];
    for (int j = 1; j < face.mNumIndices - 1; j++) {
      indices.push_back(first_index);
      indices.push_back(face.mIndices[j]);
      indices.push_back(face.mIndices[j + 1]);
    }
  }

  // Return the Geometry object
  return Geometry(vertices, indices);
}

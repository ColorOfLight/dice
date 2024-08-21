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

#include "./Root.h"

#include <vector>

void Root::addMesh(const Geometry& geometry, MaterialType material_type) {
  unsigned int vertex_object_index =
      gpu_resource_manager->createVertexObject(geometry.data);

  scene_manager->meshes.push_back(Mesh(vertex_object_index, material_type));
}

void Root::renderScene() {
  auto renderItems = [this]() -> void {
    auto& mesh = scene_manager->meshes[0];

    unsigned int shader_program =
        gpu_resource_manager->getShaderProgram(mesh.material_type);
    auto& vertex_object =
        gpu_resource_manager->getVertexObject(mesh.vertex_object_index);

    render_system->drawTriangles(shader_program, vertex_object);
  };

  render_system->runRenderLoop(renderItems);
}

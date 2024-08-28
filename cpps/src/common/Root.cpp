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

void Root::updateGpuResources() {
  auto& camera = scene_manager->camera.get();
  if (camera.needs_to_update) {
    gpu_resource_manager->upsertCameraUniformBuffer(&camera);
    camera.needs_to_update = false;
  }

  for (auto& mesh : scene_manager->meshes) {
    if (mesh.needs_to_update) {
      gpu_resource_manager->upsertModelUniformBuffer(&mesh);
      mesh.needs_to_update = false;
    }

    auto& geometry = mesh.geometry.get();
    if (geometry.needs_to_update) {
      gpu_resource_manager->upsertVertexObject(&geometry);
      geometry.needs_to_update = false;
    }
  }
}

void Root::renderScene() {
  auto renderItems = [this]() -> void {
    updateGpuResources();

    for (auto& mesh : scene_manager->meshes) {
      auto camera_uniform_buffer_id = gpu_resource_manager->getUniformBufferId(
          &scene_manager->camera.get());
      auto mesh_uniform_buffer_id =
          gpu_resource_manager->getUniformBufferId(&mesh);

      for (auto& mesh : scene_manager->meshes) {
        ShaderProgramId shader_program_id =
            gpu_resource_manager->getShaderProgram(
                mesh.material.get().getType());
        auto& vertex_object =
            gpu_resource_manager->getVertexObject(&mesh.geometry.get());
        std::vector<unsigned int> uniform_buffer_ids = {
            camera_uniform_buffer_id, mesh_uniform_buffer_id};

        render_system->drawTriangles(shader_program_id, vertex_object,
                                     uniform_buffer_ids);
      }
    }
  };

  render_system->runRenderLoop(renderItems);
}

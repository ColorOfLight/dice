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

#include <map>
#include <vector>

#include "./UniformBlock.h"

void Root::updateGpuResources() {
  auto& camera = scene_manager->camera.get();
  if (camera.needs_to_update) {
    gpu_resource_manager->upsertUniformBuffer(&camera);
    camera.needs_to_update = false;
  }

  auto& ambient_light = scene_manager->ambient_light.get();
  if (ambient_light.needs_to_update) {
    gpu_resource_manager->upsertUniformBuffer(&ambient_light);
    ambient_light.needs_to_update = false;
  }

  auto& directional_light = scene_manager->directional_light.get();
  if (directional_light.needs_to_update) {
    gpu_resource_manager->upsertUniformBuffer(&directional_light);
    directional_light.needs_to_update = false;
  }

  for (auto& mesh_ref : scene_manager->meshes) {
    auto& mesh = mesh_ref.get();

    if (mesh.needs_to_update) {
      gpu_resource_manager->upsertUniformBuffer(&mesh);
      mesh.needs_to_update = false;
    }

    auto& geometry = mesh.geometry.get();
    if (geometry.needs_to_update) {
      gpu_resource_manager->upsertVertexObject(&geometry);
      geometry.needs_to_update = false;
    }

    auto& material = mesh.material.get();
    if (material.needs_to_update) {
      gpu_resource_manager->upsertUniformBuffer(&material);
      material.needs_to_update = false;
    }
  }
}

void Root::renderScene(const std::function<void(float, float)>& loop_func) {
  auto renderItems = [this, &loop_func](float elapsed_ms,
                                        float delta_ms) -> void {
    loop_func(elapsed_ms, delta_ms);

    updateGpuResources();

    auto camera_uniform_buffer_id =
        gpu_resource_manager->getUniformBufferId(&scene_manager->camera.get());
    auto ambient_light_uniform_buffer_id =
        gpu_resource_manager->getUniformBufferId(
            &scene_manager->ambient_light.get());
    auto directional_light_uniform_buffer_id =
        gpu_resource_manager->getUniformBufferId(
            &scene_manager->directional_light.get());

    for (auto& mesh_ref : scene_manager->meshes) {
      auto& mesh = mesh_ref.get();

      ShaderProgramId shader_program_id =
          gpu_resource_manager->getShaderProgram(mesh.material.get().getType());
      auto& vertex_object =
          gpu_resource_manager->getVertexObject(&mesh.geometry.get());

      auto uniform_block_types =
          getUniformBlockTypes(mesh.material.get().getType());

      std::unordered_map<UniformBlockType, unsigned int> uniform_buffer_map;

      for (auto& uniform_buffer_type : uniform_block_types) {
        switch (uniform_buffer_type) {
          case UniformBlockType::CAMERA:
            uniform_buffer_map[uniform_buffer_type] = camera_uniform_buffer_id;
            break;
          case UniformBlockType::MODEL:
            uniform_buffer_map[uniform_buffer_type] =
                gpu_resource_manager->getUniformBufferId(&mesh);
            break;
          case UniformBlockType::MATERIAL:
            uniform_buffer_map[uniform_buffer_type] =
                gpu_resource_manager->getUniformBufferId(&mesh.material.get());
            break;
          case UniformBlockType::AMBIENT_LIGHT:
            uniform_buffer_map[uniform_buffer_type] =
                ambient_light_uniform_buffer_id;
            break;
          case UniformBlockType::DIRECTIONAL_LIGHT:
            uniform_buffer_map[uniform_buffer_type] =
                directional_light_uniform_buffer_id;
            break;
          default:
            throw std::runtime_error(
                "You should define the uniform block types for the material "
                "type.");
        }
      }

      render_system->drawTriangles(shader_program_id, vertex_object,
                                   uniform_buffer_map);
    }
  };

  render_system->runRenderLoop(renderItems);
}

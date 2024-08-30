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

#include <functional>

#include "./Camera.h"
#include "./Geometry.h"
#include "./Light.h"
#include "./Material.h"
#include "./Mesh.h"
#include "./Root.h"

int main() {
  int width = 800;
  int height = 600;
  float aspect_ratio = static_cast<float>(width) / height;

  std::unique_ptr<PerspectiveCamera> camera =
      std::make_unique<PerspectiveCamera>(glm::radians(90.0f), aspect_ratio);
  camera.get()->lookAt(glm::vec3(1, 1, 3), glm::vec3(0, 0, 0));

  std::unique_ptr<AmbientLight> ambient_light =
      std::make_unique<AmbientLight>(0.2f, glm::vec3(1.f, 1.f, 1.f));
  std::unique_ptr<DirectionalLight> directional_light =
      std::make_unique<DirectionalLight>(0.5f, glm::vec3(1.f, 1.f, 1.f),
                                         glm::vec3(0.f, 0.f, -1.f));

  Root root({width, height, *camera, *ambient_light, *directional_light});

  std::unique_ptr<CubeGeometry> cube_geometry =
      std::make_unique<CubeGeometry>();

  std::unique_ptr<TextureCoordMaterial> texture_coord_material =
      std::make_unique<TextureCoordMaterial>();
  std::unique_ptr<SingleColorMaterial> blue_color_material =
      std::make_unique<SingleColorMaterial>(glm::vec3(0.f, 0.f, 1.f));
  std::unique_ptr<PhongMaterial> phong_material =
      std::make_unique<PhongMaterial>();

  std::unique_ptr<Mesh> mesh1 =
      std::make_unique<Mesh>(*cube_geometry, *texture_coord_material);
  std::unique_ptr<Mesh> mesh2 =
      std::make_unique<Mesh>(*cube_geometry, *phong_material);
  std::unique_ptr<Mesh> mesh3 =
      std::make_unique<Mesh>(*cube_geometry, *blue_color_material);

  mesh1.get()->translate(glm::vec3(-2, 0, 0));
  mesh3.get()->translate(glm::vec3(2, 0, 0));
  mesh3.get()->rotate(glm::radians(45.0f), glm::vec3(1, 0, 0));

  root.scene_manager->meshes.push_back(*mesh1);
  root.scene_manager->meshes.push_back(*mesh2);
  root.scene_manager->meshes.push_back(*mesh3);

  std::function<void(float, float)> loop_func = [&](float delta_time,
                                                    float elapsed_time) {
    // TODO: Implement the loop function
  };

  root.renderScene(loop_func);

  return 0;
}

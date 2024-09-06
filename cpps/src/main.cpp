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

#include <btBulletDynamicsCommon.h>

#include <functional>
#include <glm/glm.hpp>

#include "./Camera.h"
#include "./Entity.h"
#include "./Geometry.h"
#include "./GeometryUtils.h"
#include "./Light.h"
#include "./Material.h"
#include "./Mesh.h"
#include "./PhysicsModule.h"
#include "./Root.h"

int main() {
  int width = 800;
  int height = 600;
  float aspect_ratio = static_cast<float>(width) / height;

  std::unique_ptr<PerspectiveCamera> camera =
      std::make_unique<PerspectiveCamera>(glm::radians(90.0f), aspect_ratio);
  camera.get()->lookAt(glm::vec3(0.5, 0.5, 2), glm::vec3(0, 0, 0));

  std::unique_ptr<AmbientLight> ambient_light =
      std::make_unique<AmbientLight>(0.2f, glm::vec3(1.f, 1.f, 1.f));
  std::unique_ptr<DirectionalLight> directional_light =
      std::make_unique<DirectionalLight>(0.5f, glm::vec3(1.f, 1.f, 1.f),
                                         glm::vec3(0.f, 0.f, -1.f));

  Root root({width, height, *camera, *ambient_light, *directional_light});
  root.setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

  std::string holder_model_path = "assets/models/holder.obj";

  std::unique_ptr<Geometry> holder_geometry = std::make_unique<Geometry>(
      GeometryUtils::loadObjToGeometry(holder_model_path));
  std::unique_ptr<CubeGeometry> cube_geometry =
      std::make_unique<CubeGeometry>(0.1f, 0.1f, 0.1f);

  std::unique_ptr<PlaneGeometry> plane_geometry =
      std::make_unique<PlaneGeometry>(4.0f, 4.0f);

  std::unique_ptr<PhongMaterial> phong_material =
      std::make_unique<PhongMaterial>();
  std::unique_ptr<SingleColorMaterial> single_color_material =
      std::make_unique<SingleColorMaterial>();

  std::unique_ptr<Mesh> holder =
      std::make_unique<Mesh>(*holder_geometry, *phong_material);
  std::unique_ptr<Mesh> cube =
      std::make_unique<Mesh>(*cube_geometry, *phong_material);

  std::unique_ptr<Mesh> plane =
      std::make_unique<Mesh>(*plane_geometry, *single_color_material);

  plane.get()->rotate(glm::radians(-90.0f), glm::vec3(1, 0, 0));
  plane.get()->translate(glm::vec3(0, -2, 0));
  holder.get()->translate(glm::vec3(0, -1, 0));

  std::unique_ptr<btCollisionShape> cube_collision_shape =
      std::make_unique<btBoxShape>(btBoxShape(btVector3(0.1f, 0.1f, 0.1f)));
  std::unique_ptr<btMotionState> cube_motion_state =
      std::make_unique<btDefaultMotionState>(btDefaultMotionState(
          btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));
  std::unique_ptr<PhysicsModule> cube_physics_module =
      std::make_unique<PhysicsModule>(1.f, btVector3(0, 0, 0),
                                      std::move(cube_collision_shape),
                                      std::move(cube_motion_state));

  std::unique_ptr<btCollisionShape> holder_collision_shape =
      std::make_unique<btBoxShape>(btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));
  std::unique_ptr<btMotionState> holder_motion_state =
      std::make_unique<btDefaultMotionState>(btDefaultMotionState(
          btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0))));
  std::unique_ptr<PhysicsModule> holder_physics_module =
      std::make_unique<PhysicsModule>(1.f, btVector3(0, 0, 0),
                                      std::move(holder_collision_shape),
                                      std::move(holder_motion_state));

  // Plane Collision shape with width 8.0f and height 8.0f
  std::unique_ptr<btCollisionShape> plane_collision_shape =
      std::make_unique<btBoxShape>(btBoxShape(btVector3(4.0f, 4.0f, 0.01f)));
  std::unique_ptr<btMotionState> plane_motion_state =
      std::make_unique<btDefaultMotionState>(btDefaultMotionState(
          btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -3, 0))));
  std::unique_ptr<PhysicsModule> plane_physics_module =
      std::make_unique<PhysicsModule>(0.f, btVector3(0, 0, 0),
                                      std::move(plane_collision_shape),
                                      std::move(plane_motion_state));

  std::unique_ptr<Entity> cube_entity =
      std::make_unique<Entity>(std::move(cube), std::move(cube_physics_module));
  std::unique_ptr<Entity> plane_entity = std::make_unique<Entity>(
      std::move(plane), std::move(plane_physics_module));
  std::unique_ptr<Entity> holder_entity = std::make_unique<Entity>(
      std::move(holder), std::move(holder_physics_module));

  cube_entity.get()->syncPhysicsWithMesh();
  plane_entity.get()->syncPhysicsWithMesh();
  holder_entity.get()->syncPhysicsWithMesh();

  root.scene_manager->addEntity(*cube_entity);
  root.scene_manager->addEntity(*plane_entity);
  root.scene_manager->addEntity(*holder_entity);

  std::function<void(float, float)> loop_func = [&](float elapsed_ms,
                                                    float delta_ms) {
    //
  };

  root.renderScene(loop_func);

  return 0;
}

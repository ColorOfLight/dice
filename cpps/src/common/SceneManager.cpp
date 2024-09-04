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

#include "./SceneManager.h"

SceneManager::SceneManager(
    std::reference_wrapper<Camera> camera,
    std::reference_wrapper<AmbientLight> ambient_light,
    std::reference_wrapper<DirectionalLight> directional_light)
    : camera(camera),
      ambient_light(ambient_light),
      directional_light(directional_light) {
  bt_broadphase = std::make_unique<btDbvtBroadphase>();
  bt_collision_configuration =
      std::make_unique<btDefaultCollisionConfiguration>();
  bt_dispatcher =
      std::make_unique<btCollisionDispatcher>(bt_collision_configuration.get());
  bt_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
  bt_dynamics_world = std::make_unique<btDiscreteDynamicsWorld>(
      bt_dispatcher.get(), bt_broadphase.get(), bt_solver.get(),
      bt_collision_configuration.get());

  bt_dynamics_world.get()->setGravity(btVector3(0, -9.81, 0));  // Set gravity
}

void SceneManager::addEntity(std::reference_wrapper<Entity> entity) {
  auto& physics_module = entity.get().physics_module;
  auto rigid_body = (*physics_module).getRigidBody();

  entities.push_back(entity);
  bt_dynamics_world.get()->addRigidBody(&rigid_body.get());
}

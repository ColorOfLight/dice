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

#include <btBulletDynamicsCommon.h>

#include <vector>

#include "./Camera.h"
#include "./Entity.h"
#include "./Geometry.h"
#include "./Light.h"
#include "./Mesh.h"

class SceneManager {
 public:
  SceneManager(std::reference_wrapper<Camera> camera,
               std::reference_wrapper<AmbientLight> ambient_light,
               std::reference_wrapper<DirectionalLight> directional_light);

  std::vector<std::reference_wrapper<Mesh>> meshes;
  std::vector<std::reference_wrapper<Entity>> entities;
  std::reference_wrapper<Camera> camera;
  std::reference_wrapper<AmbientLight> ambient_light;
  std::reference_wrapper<DirectionalLight> directional_light;

  // Bullet Physics
  std::unique_ptr<btBroadphaseInterface> bt_broadphase;
  std::unique_ptr<btDefaultCollisionConfiguration> bt_collision_configuration;
  std::unique_ptr<btCollisionDispatcher> bt_dispatcher;
  std::unique_ptr<btSequentialImpulseConstraintSolver> bt_solver;
  std::unique_ptr<btDiscreteDynamicsWorld> bt_dynamics_world;
};

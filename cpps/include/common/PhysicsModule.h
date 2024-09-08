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

#include <glm/glm.hpp>
#include <memory>

#include "./Geometry.h"

class PhysicsModule {
 public:
  PhysicsModule(btScalar mass, btVector3 inertia,
                std::unique_ptr<btCollisionShape> collision_shape,
                std::unique_ptr<btMotionState> motion_state);
  PhysicsModule(btScalar mass, btVector3 inertia, const btTransform& transform)
      : mass(mass),
        inertia(inertia),
        bt_motion_state(std::make_unique<btDefaultMotionState>(transform)) {}

  glm::vec3 getPosition() const;
  glm::quat getRotation() const;
  std::reference_wrapper<btRigidBody> getRigidBody() { return *bt_rigid_body; }

  void setPosition(const glm::vec3& position);
  void setRotation(const glm::quat& rotation);

 protected:
  std::unique_ptr<btCollisionShape> bt_collision_shape;
  std::unique_ptr<btMotionState> bt_motion_state;
  std::unique_ptr<btRigidBody> bt_rigid_body;

  btScalar mass;
  btVector3 inertia;
};

class BoxShapePhysicsModule : public PhysicsModule {
 public:
  BoxShapePhysicsModule(float mass, const btVector3& inertia,
                        const CubeGeometry& cube_geometry,
                        const btTransform& transform);
};

class TriangleMeshPhysicsModule : public PhysicsModule {
 public:
  TriangleMeshPhysicsModule(const Geometry& geometry, float mass,
                            const btVector3& inertia,
                            const btTransform& transform);

 private:
  std::unique_ptr<btTriangleIndexVertexArray> mesh_index_vertex_array;
};

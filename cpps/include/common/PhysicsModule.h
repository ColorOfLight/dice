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

#include "./Material.h"

class PhysicsModule {
 public:
  PhysicsModule(btScalar mass, btVector3 inertia,
                std::unique_ptr<btCollisionShape> collision_shape,
                std::unique_ptr<btMotionState> motion_state);

  glm::vec3 getPosition() const;
  glm::quat getRotation() const;
  std::reference_wrapper<btRigidBody> getRigidBody() { return *bt_rigid_body; }

  void setPosition(const glm::vec3& position);
  void setRotation(const glm::quat& rotation);

 private:
  std::unique_ptr<btCollisionShape> bt_collision_shape;
  std::unique_ptr<btMotionState> bt_motion_state;
  std::unique_ptr<btRigidBody> bt_rigid_body;

  btScalar mass;
  btVector3 inertia;
};

// btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0),
// 1);

// btDefaultMotionState* groundMotionState = new btDefaultMotionState(
//     btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
// btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,
// groundMotionState,
//                                                            groundShape,
//                                                            btVector3(0, 0,
//                                                            0));
// btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
// dynamicsWorld->addRigidBody(groundRigidBody);

// btCollisionShape* cubeShape =
//     new btBoxShape(btVector3(1, 1, 1));  // Cube with half-extents of 1

// btScalar mass = 1;
// btVector3 fallInertia(0, 0, 0);
// cubeShape->calculateLocalInertia(mass, fallInertia);

// btDefaultMotionState* cubeMotionState1 = new btDefaultMotionState(
//     btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 20, 0)));
// btRigidBody::btRigidBodyConstructionInfo cubeRigidBodyCI1(mass,
//                                                           cubeMotionState1,
//                                                           cubeShape,
//                                                           fallInertia);
// btRigidBody* cubeRigidBody1 = new btRigidBody(cubeRigidBodyCI1);
// dynamicsWorld->addRigidBody(cubeRigidBody1);

// btDefaultMotionState* cubeMotionState2 = new btDefaultMotionState(
//     btTransform(btQuaternion(0, 0, 0, 1), btVector3(2, 30, 0)));
// btRigidBody::btRigidBodyConstructionInfo cubeRigidBodyCI2(mass,
//                                                           cubeMotionState2,
//                                                           cubeShape,
//                                                           fallInertia);
// btRigidBody* cubeRigidBody2 = new btRigidBody(cubeRigidBodyCI2);
// dynamicsWorld->addRigidBody(cubeRigidBody2);
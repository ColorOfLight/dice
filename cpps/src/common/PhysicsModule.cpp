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

#include "./PhysicsModule.h"

PhysicsModule::PhysicsModule(btScalar mass, btVector3 inertia,
                             std::unique_ptr<btCollisionShape> collision_shape,
                             std::unique_ptr<btMotionState> motion_state)
    : mass(mass),
      inertia(inertia),
      bt_collision_shape(std::move(collision_shape)),
      bt_motion_state(std::move(motion_state)) {
  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(
      mass, bt_motion_state.get(), bt_collision_shape.get(), inertia);

  bt_rigid_body = std::make_unique<btRigidBody>(rigid_body_ci);
  if (mass > 0) {
    this->bt_collision_shape.get()->calculateLocalInertia(mass, inertia);
  }
}

glm::vec3 PhysicsModule::getPosition() {
  btTransform transform = bt_rigid_body.get()->getWorldTransform();
  btVector3 origin = transform.getOrigin();

  return glm::vec3(origin.getX(), origin.getY(), origin.getZ());
}

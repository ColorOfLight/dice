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

#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <BulletCollision/Gimpact/btGImpactShape.h>

#include <glm/gtc/quaternion.hpp>
#include <vector>

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

glm::vec3 PhysicsModule::getPosition() const {
  btTransform transform = bt_rigid_body.get()->getWorldTransform();
  btVector3 origin = transform.getOrigin();

  return glm::vec3(origin.getX(), origin.getY(), origin.getZ());
}

glm::quat PhysicsModule::getRotation() const {
  btTransform transform = bt_rigid_body.get()->getWorldTransform();
  btQuaternion rotation = transform.getRotation();

  return glm::quat(rotation.getW(), rotation.getX(), rotation.getY(),
                   rotation.getZ());
}

void PhysicsModule::setPosition(const glm::vec3& position) {
  btTransform transform = bt_rigid_body.get()->getWorldTransform();
  btVector3 origin = transform.getOrigin();

  origin.setX(position.x);
  origin.setY(position.y);
  origin.setZ(position.z);

  transform.setOrigin(origin);
  bt_rigid_body.get()->setWorldTransform(transform);
}

void PhysicsModule::setRotation(const glm::quat& rotation) {
  btTransform transform = bt_rigid_body.get()->getWorldTransform();
  btQuaternion quaternion(rotation.x, rotation.y, rotation.z, rotation.w);

  transform.setRotation(quaternion);
  bt_rigid_body.get()->setWorldTransform(transform);
}

BoxShapePhysicsModule::BoxShapePhysicsModule(float mass,
                                             const btVector3& inertia,
                                             const CubeGeometry& cube_geometry,
                                             const btTransform& transform)
    : PhysicsModule(
          mass, inertia,
          std::make_unique<btBoxShape>(btVector3(cube_geometry.getHalfWidth(),
                                                 cube_geometry.getHalfHeight(),
                                                 cube_geometry.getHalfDepth())),
          std::make_unique<btDefaultMotionState>(transform)) {}

TriangleMeshPhysicsModule::TriangleMeshPhysicsModule(
    const Geometry& geometry, float mass, const btVector3& inertia,
    const btTransform& transform)
    : PhysicsModule(mass, inertia, transform) {
  auto& geometry_vertices = geometry.getVertices();

  std::vector<btVector3> vertices =
      std::vector<btVector3>(geometry_vertices.size());
  for (int i = 0; i < geometry_vertices.size(); i++) {
    vertices[i] = btVector3(geometry_vertices[i].position.x,
                            geometry_vertices[i].position.y,
                            geometry_vertices[i].position.z);
  }

  const auto& indices = geometry.getIndices();

  // Create a btIndexedMesh and set its data
  btIndexedMesh indexed_mesh;
  indexed_mesh.m_numTriangles = indices.size() / 3;
  indexed_mesh.m_triangleIndexBase =
      reinterpret_cast<const unsigned char*>(indices.data());
  indexed_mesh.m_triangleIndexStride = 3 * sizeof(int);
  indexed_mesh.m_numVertices = vertices.size();
  indexed_mesh.m_vertexBase =
      reinterpret_cast<const unsigned char*>(vertices.data());
  indexed_mesh.m_vertexStride = 3 * sizeof(btScalar);

  mesh_index_vertex_array = std::make_unique<btTriangleIndexVertexArray>();
  mesh_index_vertex_array->addIndexedMesh(indexed_mesh, PHY_INTEGER);

  auto triangle_mesh_shape =
      std::make_unique<btGImpactMeshShape>(mesh_index_vertex_array.get());
  triangle_mesh_shape->updateBound();
  triangle_mesh_shape->setMargin(btScalar(0.04));

  bt_collision_shape = std::move(triangle_mesh_shape);

  btRigidBody::btRigidBodyConstructionInfo rigid_body_ci(
      mass, bt_motion_state.get(), bt_collision_shape.get(), inertia);

  bt_rigid_body = std::make_unique<btRigidBody>(rigid_body_ci);
  if (mass > 0) {
    this->bt_collision_shape.get()->calculateLocalInertia(this->mass,
                                                          this->inertia);
  }
}

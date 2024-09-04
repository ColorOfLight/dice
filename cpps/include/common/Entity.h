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

#include "./Mesh.h"
#include "./PhysicsModule.h"

class Entity {
 public:
  Entity(Mesh&& mesh, PhysicsModule&& physics_module)
      : mesh(std::make_unique<Mesh>(std::move(mesh))),
        physics_module(
            std::make_unique<PhysicsModule>(std::move(physics_module))) {};

  Entity(std::unique_ptr<Mesh> mesh,
         std::unique_ptr<PhysicsModule> physics_module)
      : mesh(std::move(mesh)), physics_module(std::move(physics_module)) {};

  void syncMeshWithPhysics();
  void syncPhysicsWithMesh();

  std::unique_ptr<Mesh> mesh;
  std::unique_ptr<PhysicsModule> physics_module;
};

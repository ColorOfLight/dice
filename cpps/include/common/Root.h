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

#include <functional>
#include <memory>

#include "./Camera.h"
#include "./Geometry.h"
#include "./GpuResourceManager.h"
#include "./Light.h"
#include "./Material.h"
#include "./RenderSystem.h"
#include "./SceneManager.h"

struct RootOptions {
  int initial_width;
  int initial_height;
  std::reference_wrapper<Camera> camera;
  std::reference_wrapper<AmbientLight> ambient_light;
  std::reference_wrapper<DirectionalLight> directional_light;
};

class Root {
 public:
  Root(const RootOptions& options);

  void renderScene(const std::function<void(float, float)>& loop_func);
  void setClearColor(const glm::vec4& color);

 private:
  void updateGpuResources();
  void simulateDynamicsWorld(float delta_ms);
  void syncEntityMeshesWithPhysics();

 public:
  std::unique_ptr<SceneManager> scene_manager;

 private:
  std::unique_ptr<RenderSystem> render_system;
  std::unique_ptr<GpuResourceManager> gpu_resource_manager;
};

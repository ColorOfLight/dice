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

#include "./Light.h"

void AmbientLight::setIntensity(float intensity) {
  uniform_data.intensity = intensity;
  needs_to_update = true;
}

void AmbientLight::setColor(const glm::vec3& color) {
  uniform_data.color = color;
  needs_to_update = true;
}

void DirectionalLight::setIntensity(float intensity) {
  uniform_data.intensity = intensity;
  needs_to_update = true;
}

void DirectionalLight::setColor(const glm::vec3& color) {
  uniform_data.color = color;
  needs_to_update = true;
}

void DirectionalLight::setDirection(const glm::vec3& direction) {
  uniform_data.direction = direction;
  needs_to_update = true;
}

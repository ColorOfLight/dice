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

#include "./Material.h"

void SingleColorMaterial::setColor(const glm::vec3& color) {
  uniform_data.color = color;
  needs_to_update = true;
}

void PhongMaterial::setDiffuse(float diffuse) {
  uniform_data.diffuse = diffuse;
  needs_to_update = true;
}

void PhongMaterial::setColor(const glm::vec3& color) {
  uniform_data.color = color;
  needs_to_update = true;
}

void PhongMaterial::setSpecular(float specular) {
  uniform_data.specular = specular;
  needs_to_update = true;
}

void PhongMaterial::setAlpha(float alpha) {
  uniform_data.alpha = alpha;
  needs_to_update = true;
}

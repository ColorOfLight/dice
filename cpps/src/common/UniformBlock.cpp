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

#include "./UniformBlock.h"

std::string getUniformBlockName(UniformBlockType type) {
  switch (type) {
    case UniformBlockType::CAMERA:
      return "CameraBlock";
    case UniformBlockType::MODEL:
      return "ModelBlock";
    case UniformBlockType::MATERIAL:
      return "MaterialBlock";
    case UniformBlockType::AMBIENT_LIGHT:
      return "AmbientLightBlock";
    case UniformBlockType::DIRECTIONAL_LIGHT:
      return "DirectionalLightBlock";
    default:
      throw std::runtime_error(
          "You should define the name of the uniform block type.");
  }
}

std::vector<UniformBlockType> getUniformBlockTypes(MaterialType material_type) {
  switch (material_type) {
    case MaterialType::BASIC:
      return {UniformBlockType::CAMERA, UniformBlockType::MODEL};
    case MaterialType::NORMAL:
      return {UniformBlockType::CAMERA, UniformBlockType::MODEL};
    case MaterialType::TEXTURE_COORD:
      return {UniformBlockType::CAMERA, UniformBlockType::MODEL};
    case MaterialType::SINGLE_COLOR:
      return {UniformBlockType::CAMERA, UniformBlockType::MODEL,
              UniformBlockType::MATERIAL};
    case MaterialType::PHONG:
      return {UniformBlockType::CAMERA, UniformBlockType::MODEL,
              UniformBlockType::MATERIAL, UniformBlockType::AMBIENT_LIGHT,
              UniformBlockType::DIRECTIONAL_LIGHT};
    default:
      throw std::runtime_error(
          "You should define the uniform block types for the material type.");
  }
}

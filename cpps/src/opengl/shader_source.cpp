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

#include "./shader_source.h"

#include "./opengl.h"

std::string basic_vertex_source = R"(
    layout (std140) uniform CameraBlock
    {
        mat4 view_matrix;
        mat4 projection_matrix;
    };

    layout (std140) uniform ModelBlock
    {
        mat4 model_matrix;
    };

    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec3 aNormal;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 vNormal;
    out vec2 vTexCoord;

    void main()
    {
        gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPosition, 1.0);
        vNormal = aNormal;
        vTexCoord = aTexCoord;
    }
)";

std::string basic_fragment_source = R"(
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Red color
    }
)";

std::string normal_fragment_source = R"(
    out vec4 FragColor;

    in vec3 vNormal;

    void main()
    {
        vec3 normalized_normal = normalize(vNormal);
        vec3 positive_normal = (normalized_normal + 1.0) * 0.5;
        FragColor = vec4(positive_normal, 1.0);
    }
)";

std::string texture_coord_fragment_source = R"(
    out vec4 FragColor;

    in vec2 vTexCoord;

    void main()
    {
        FragColor = vec4(vTexCoord, 0.0, 1.0);
    }
)";

ShaderSource getShaderSource(MaterialType type) {
  std::string shader_prefix = SHADER_PREFIX;
  std::string fragment_precision = FRAGMENT_PRECISION;

  std::string base_vertex_source;
  std::string base_fragment_source;

  switch (type) {
    case MaterialType::BASIC:
      base_vertex_source = basic_vertex_source;
      base_fragment_source = basic_fragment_source;
      break;
    case MaterialType::NORMAL:
      base_vertex_source = basic_vertex_source;
      base_fragment_source = normal_fragment_source;
      break;
    case MaterialType::TEXTURE_COORD:
      base_vertex_source = basic_vertex_source;
      base_fragment_source = texture_coord_fragment_source;
      break;
    default:
      throw std::runtime_error("Invalid MaterialType");
  }

  std::string vertex_shader_source = shader_prefix + base_vertex_source;
  std::string fragment_shader_source =
      shader_prefix + fragment_precision + base_fragment_source;

  return {std::move(vertex_shader_source), std::move(fragment_shader_source)};
}
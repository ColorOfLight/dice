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
        mat4 u_camera_viewMatrix;
        mat4 u_camera_projectionMatrix;
        vec3 u_camera_eye;
    };

    layout (std140) uniform ModelBlock
    {
        mat4 u_model_matrix;
    };

    layout (location = 0) in vec3 a_position;
    layout (location = 1) in vec3 a_normal;
    layout (location = 2) in vec2 a_texCoord;

    out vec3 v_position;
    out vec3 v_normal;
    out vec2 v_texCoord;

    void main()
    {
        gl_Position = u_camera_projectionMatrix * u_camera_viewMatrix * u_model_matrix * vec4(a_position, 1.0);

        vec4 modelPosition = u_model_matrix * vec4(a_position, 1.0);
        v_position = modelPosition.xyz;

        mat3 normalMatrix = transpose(inverse(mat3(u_model_matrix)));
        v_normal = normalize(normalMatrix * a_normal);

        v_texCoord = a_texCoord;
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

    in vec3 v_normal;

    void main()
    {
        vec3 normalizedNormal = normalize(v_normal);
        vec3 positiveNormal = (normalizedNormal + 1.0) * 0.5;
        FragColor = vec4(positiveNormal, 1.0);
    }
)";

std::string texture_coord_fragment_source = R"(
    out vec4 FragColor;

    in vec2 v_texCoord;

    void main()
    {
        FragColor = vec4(v_texCoord, 0.0, 1.0);
    }
)";

std::string single_color_fragment_source = R"(
    layout (std140) uniform MaterialBlock
    {
        vec3 u_material_color;
    };

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(u_material_color, 1.0);
    }
)";

// TODO: Implement Phong shading
std::string phong_fragment_source = R"(
    layout (std140) uniform CameraBlock
    {
        mat4 u_camera_viewMatrix;
        mat4 u_camera_projectionMatrix;
        vec3 u_camera_eye;
    };

    layout (std140) uniform MaterialBlock
    {
        vec3 u_material_color;
        float u_material_diffuse;
        float u_material_specular;
        float u_material_alpha;
    };

    layout (std140) uniform AmbientLightBlock
    {
        vec3 u_ambient_color;
        float u_ambient_intensity;
    };

    layout (std140) uniform DirectionalLightBlock
    {
        vec3 u_directional_color;
        vec3 u_directional_direction;
        float u_directional_intensity;
    };

    in vec3 v_normal;
    in vec3 v_position;

    out vec4 FragColor;

    void main()
    {
        vec3 ambientColor = u_material_color * u_ambient_color * u_ambient_intensity * u_material_diffuse;

        vec3 normal = normalize(v_normal);
        vec3 lightVector = normalize(-u_directional_direction);
        vec3 directionalDiffuseColor = u_material_diffuse * max(dot(normal, lightVector), 0.0) * u_material_color * u_directional_color * u_directional_intensity;

        vec3 viewVector = normalize(u_camera_eye - v_position);
        vec3 reflection = reflect(u_directional_direction, normal);
        vec3 directionalSpecularColor = u_material_specular * pow(max(0.0, dot(reflection, viewVector)), u_material_alpha) * u_directional_color * u_directional_intensity;
        
        vec3 color = ambientColor + directionalDiffuseColor + directionalSpecularColor;
        FragColor = vec4(color, 1.0);
    }
)";

ShaderSource getShaderSource(MaterialType type) {
  std::string shader_prefix = SHADER_PREFIX;

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
    case MaterialType::SINGLE_COLOR:
      base_vertex_source = basic_vertex_source;
      base_fragment_source = single_color_fragment_source;
      break;
    case MaterialType::PHONG:
      base_vertex_source = basic_vertex_source;
      base_fragment_source = phong_fragment_source;
      break;
    default:
      throw std::runtime_error("Invalid MaterialType");
  }

  std::string vertex_shader_source = shader_prefix + base_vertex_source;
  std::string fragment_shader_source = shader_prefix + base_fragment_source;

  return {std::move(vertex_shader_source), std::move(fragment_shader_source)};
}
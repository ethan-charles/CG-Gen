// rasterizer_impl.cpp

#include <cstdint>
#include <vector>
#include <limits>
#include <cmath>
#include <algorithm>
#include "image.hpp"
#include "loader.hpp"
#include "rasterizer.hpp"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/gtx/quaternion.hpp"

// Define sample offsets for MSAA
const std::vector<glm::vec2> MSAA_SAMPLE_OFFSETS = {
    {0.25f, 0.25f},
    {0.75f, 0.25f},
    {0.25f, 0.75f},
    {0.75f, 0.75f}
};

// Function to check if a point is inside a triangle
bool IsPointInsideTriangle(const glm::vec3& point, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
    glm::vec3 edge0 = v1 - v0;
    glm::vec3 edge1 = v2 - v1;
    glm::vec3 edge2 = v0 - v2;

    glm::vec3 c0 = point - v0;
    glm::vec3 c1 = point - v1;
    glm::vec3 c2 = point - v2;

    glm::vec3 cross0 = glm::cross(edge0, c0);
    glm::vec3 cross1 = glm::cross(edge1, c1);
    glm::vec3 cross2 = glm::cross(edge2, c2);

    if (glm::dot(cross0, cross1) >= 0 && glm::dot(cross0, cross2) >= 0) {
        return true;
    }
    return false;
}

// Function to compute barycentric coordinates
glm::vec3 Rasterizer::BarycentricCoordinate(glm::vec2 pos, Triangle trig)
{
    glm::vec3 v0 = glm::vec3(trig.pos[0].x, trig.pos[0].y, 0);
    glm::vec3 v1 = glm::vec3(trig.pos[1].x, trig.pos[1].y, 0);
    glm::vec3 v2 = glm::vec3(trig.pos[2].x, trig.pos[2].y, 0);

    glm::vec3 v0v1 = v1 - v0;
    glm::vec3 v0v2 = v2 - v0;
    glm::vec3 v0p = glm::vec3(pos, 0) - v0;

    float d00 = glm::dot(v0v1, v0v1);
    float d01 = glm::dot(v0v1, v0v2);
    float d11 = glm::dot(v0v2, v0v2);
    float d20 = glm::dot(v0p, v0v1);
    float d21 = glm::dot(v0p, v0v2);

    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return glm::vec3(u, v, w);
}

// Implement MSAA in DrawPixel function
void Rasterizer::DrawPixel(uint32_t x, uint32_t y, Triangle trig, AntiAliasConfig config, uint32_t spp, Image& image, Color color)
{
    if (config == AntiAliasConfig::NONE)
    {
        glm::vec3 pixel_center(x + 0.5f, y + 0.5f, 0.0f);
        if (IsPointInsideTriangle(pixel_center, trig.pos[0], trig.pos[1], trig.pos[2]))
        {
            image.Set(x, y, color);
        }
    }
    else if (config == AntiAliasConfig::SSAA)
    {
        uint32_t sample_count = 0;
        for (uint32_t i = 0; i < spp; ++i)
        {
            float sample_x = static_cast<float>(x) + static_cast<float>(rand()) / RAND_MAX;
            float sample_y = static_cast<float>(y) + static_cast<float>(rand()) / RAND_MAX;
            glm::vec3 sample_point(sample_x, sample_y, 0.0f);

            if (IsPointInsideTriangle(sample_point, trig.pos[0], trig.pos[1], trig.pos[2]))
            {
                ++sample_count;
            }
        }

        float coverage = static_cast<float>(sample_count) / static_cast<float>(spp);
        Color final_color = color * coverage;
        image.Set(x, y, final_color);
    }
    else if (config == AntiAliasConfig::MSAA)
    {
        uint32_t covered_samples = 0;
        std::vector<float> sample_depths(spp, std::numeric_limits<float>::infinity());

        for (uint32_t i = 0; i < spp; ++i)
        {
            float sample_x = static_cast<float>(x) + MSAA_SAMPLE_OFFSETS[i % MSAA_SAMPLE_OFFSETS.size()].x;
            float sample_y = static_cast<float>(y) + MSAA_SAMPLE_OFFSETS[i % MSAA_SAMPLE_OFFSETS.size()].y;
            glm::vec3 sample_point(sample_x, sample_y, 0.0f);

            if (IsPointInsideTriangle(sample_point, trig.pos[0], trig.pos[1], trig.pos[2]))
            {
                ++covered_samples;
                glm::vec3 bary = BarycentricCoordinate(glm::vec2(sample_x, sample_y), trig);
                float depth = glm::dot(bary, glm::vec3(trig.pos[0].z, trig.pos[1].z, trig.pos[2].z));

                // Update per-sample depth
                float currentDepth = this->ZBufferSamples.Get(x, y, i);
                if (depth < currentDepth)
                {
                    this->ZBufferSamples.Set(x, y, i, depth);
                }
            }
        }

        // Perform shading once per pixel
        float coverage = static_cast<float>(covered_samples) / static_cast<float>(spp);
        Color final_color = color * coverage;
        image.Set(x, y, final_color);
    }
    return;
}

// AddModel function remains unchanged
void Rasterizer::AddModel(MeshTransform transform, glm::mat4 rotation)
{
    glm::mat4 Mrot = rotation;
    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), transform.translation);
    glm::mat4 Mscale = glm::scale(glm::mat4(1.0f), transform.scale);

    glm::mat4 Mmodel = Mrot * Mtrans * Mscale;

    this->model.push_back(Mmodel);
    return;
}

// SetView function remains unchanged
void Rasterizer::SetView()
{
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = camera.lookAt;
    glm::vec3 cameraUp = camera.up;

    glm::vec3 g = glm::normalize(cameraLookAt - cameraPos); // Forward
    glm::vec3 f = glm::normalize(glm::cross(g, cameraUp));  // Right
    glm::vec3 h = glm::cross(f, g); // Up

    glm::mat4 Mrot(1.0f);
    Mrot[0][0] = f.x; Mrot[0][1] = h.x; Mrot[0][2] = -g.x; Mrot[0][3] = 0.0f;
    Mrot[1][0] = f.y; Mrot[1][1] = h.y; Mrot[1][2] = -g.y; Mrot[1][3] = 0.0f;
    Mrot[2][0] = f.z; Mrot[2][1] = h.z; Mrot[2][2] = -g.z; Mrot[2][3] = 0.0f;
    Mrot[3][0] = 0.0f; Mrot[3][1] = 0.0f; Mrot[3][2] = 0.0f; Mrot[3][3] = 1.0f;

    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), -cameraPos);

    this->view = Mrot * Mtrans;
    return;
}

// SetProjection function remains unchanged
void Rasterizer::SetProjection()
{
    const Camera& camera = this->loader.GetCamera();
    float nearClip = -camera.nearClip;
    float farClip = -camera.farClip;
    float camera_width = camera.width;
    float camera_height = camera.height;

    glm::mat4 Mpersp = glm::mat4(0.0f);
    Mpersp[0][0] = nearClip;
    Mpersp[1][1] = nearClip;
    Mpersp[2][2] = nearClip + farClip;
    Mpersp[2][3] = 1.0f;
    Mpersp[3][2] = -nearClip * farClip;

    glm::mat4 Mortho_scale = glm::mat4(1.0f);
    Mortho_scale[0][0] = 2.0f / camera_width;
    Mortho_scale[1][1] = 2.0f / camera_height;
    Mortho_scale[2][2] = 2.0f / (nearClip - farClip);

    glm::mat4 Mortho_trans = glm::mat4(1.0f);
    Mortho_trans[3][2] = -(nearClip + farClip) / 2.0f;

    this->projection = Mortho_scale * Mortho_trans * Mpersp;

    return;
}

// SetScreenSpace function remains unchanged
void Rasterizer::SetScreenSpace()
{
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    glm::mat4 Mss = glm::mat4(1.0f);

    Mss[0][0] = width / 2.0f;
    Mss[1][1] = height / 2.0f;
    Mss[2][2] = 1.0f;
    Mss[3][0] = width / 2.0f; Mss[3][1] = height / 2.0f; Mss[3][2] = 0.0f;

    this->screenspace = Mss;

    return;
}

// zBufferDefault remains unchanged
float Rasterizer::zBufferDefault = -1.0f;

// UpdateDepthAtPixel function updated to support MSAA
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{
    // For MSAA, depth is handled per sample in ShadeAtPixel
    // This function can remain unchanged or be adapted if needed
    return;
}

// ShadeAtPixel function updated to implement MSAA
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{
    uint32_t spp = MSAA_SAMPLE_OFFSETS.size();
    uint32_t covered_samples = 0;
    float accumulated_coverage = 0.0f;

    Color accumulated_color(0.0f);

    for (uint32_t i = 0; i < spp; ++i)
    {
        float sample_x = static_cast<float>(x) + MSAA_SAMPLE_OFFSETS[i].x;
        float sample_y = static_cast<float>(y) + MSAA_SAMPLE_OFFSETS[i].y;
        glm::vec3 sample_point(sample_x, sample_y, 0.0f);

        if (IsPointInsideTriangle(sample_point, transformed.pos[0], transformed.pos[1], transformed.pos[2]))
        {
            glm::vec3 bary = BarycentricCoordinate(glm::vec2(sample_x, sample_y), transformed);

            // Compute depth
            float depth = bary.x * transformed.pos[0].z + bary.y * transformed.pos[1].z + bary.z * transformed.pos[2].z;

            // Get current depth from per-sample depth buffer
            float currentDepth = this->ZBufferSamples.Get(x, y, i);

            if (depth > currentDepth)
            {
                this->ZBufferSamples.Set(x, y, i, depth);
                ++covered_samples;
            }
        }
    }

    if (covered_samples > 0)
    {
        // Perform shading once per pixel
        glm::vec2 pos(x + 0.5f, y + 0.5f);
        glm::vec3 bary = BarycentricCoordinate(pos, transformed);

        // Interpolate position and normal
        glm::vec3 interpolatedPos = bary.x * glm::vec3(original.pos[0]) + bary.y * glm::vec3(original.pos[1]) + bary.z * glm::vec3(original.pos[2]);
        glm::vec3 normal = bary.x * original.normal[0] + bary.y * original.normal[1] + bary.z * original.normal[2];
        normal = glm::normalize(normal);

        // Lighting calculations
        std::vector<Light> lights = this->loader.GetLights();
        Color ambientColor = this->loader.GetAmbientColor();
        float specularExponent = this->loader.GetSpecularExponent();
        glm::vec3 cameraPos = this->loader.GetCamera().pos;

        Color result = ambientColor;
        for (const auto& light : lights)
        {
            glm::vec3 l = glm::normalize(light.pos - interpolatedPos);
            glm::vec3 v = glm::normalize(cameraPos - interpolatedPos);
            glm::vec3 h = glm::normalize(l + v);
            float r2 = glm::length2(light.pos - interpolatedPos);

            float NdotL = std::max(glm::dot(normal, l), 0.0f);
            float NdotH = std::max(glm::dot(normal, h), 0.0f);

            Color diffuse = light.color * (light.intensity / r2) * NdotL;
            Color specular = light.color * (light.intensity / r2) * glm::pow(NdotH, specularExponent);

            result = result + diffuse + specular;
        }

        // Apply coverage
        float coverage = static_cast<float>(covered_samples) / static_cast<float>(spp);
        Color final_color = result * coverage;

        // Set the pixel color
        image.Set(x, y, final_color);
    }

    return;
}

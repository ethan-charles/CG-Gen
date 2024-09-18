#include <cstdint>
#include "image.hpp"
#include "loader.hpp"
#include "rasterizer.hpp"

// TODO
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


void Rasterizer::DrawPixel(uint32_t x, uint32_t y, Triangle trig, AntiAliasConfig config, uint32_t spp, Image& image, Color color)
{
    if (config == AntiAliasConfig::NONE)            // if anti-aliasing is off
    {
        glm::vec3 pixel_center(x + 0.5f, y + 0.5f, 0.0f); 
        if (IsPointInsideTriangle(pixel_center, trig.pos[0], trig.pos[1], trig.pos[2]))
        {
            image.Set(x, y, color);
        }
    }
    else if (config == AntiAliasConfig::SSAA)       // if anti-aliasing is on
    {
        uint32_t sample_count = 0;

        // Loop over the samples per pixel (spp)
        for (uint32_t i = 0; i < spp; ++i)
        {
        
            float sample_x = static_cast<float>(x) + static_cast<float>(rand()) / RAND_MAX;
            float sample_y = static_cast<float>(y) + static_cast<float>(rand()) / RAND_MAX;
            glm::vec3 sample_point(sample_x, sample_y, 0.0f);

            
            if (IsPointInsideTriangle(sample_point, trig.pos[0], trig.pos[1], trig.pos[2]))
            {
                ++sample_count;
                // if the pixel is inside the triangle
                
            }
        }

        
        float coverage = static_cast<float>(sample_count) / static_cast<float>(spp);
        Color final_color = color * coverage;
        image.Set(x, y, final_color);
    }


    return;
}

// TODO
void Rasterizer::AddModel(MeshTransform transform, glm::mat4 rotation)
{
    /* model.push_back( model transformation constructed from translation, rotation and scale );*/
    return;
}

// TODO
void Rasterizer::SetView()
{
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = camera.lookAt;
    glm::vec3 cameraUp = camera.up;

    // TODO change this line to the correct view matrix
    this->view = glm::mat4(1.);

    return;
}

// TODO
void Rasterizer::SetProjection()
{
    const Camera& camera = this->loader.GetCamera();

    float nearClip = camera.nearClip;                   // near clipping distance, strictly positive
    float farClip = camera.farClip;                     // far clipping distance, strictly positive
    
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();
    
    // TODO change this line to the correct projection matrix
    this->projection = glm::mat4(1.);

    return;
}

// TODO
void Rasterizer::SetScreenSpace()
{
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    // TODO change this line to the correct screenspace matrix
    this->screenspace = glm::mat4(1.);

    return;
}

// TODO
glm::vec3 Rasterizer::BarycentricCoordinate(glm::vec2 pos, Triangle trig)
{
    glm::vec3 v1 = glm::vec3(trig.pos[0].x, trig.pos[0].y, 0);
    glm::vec3 v2 = glm::vec3(trig.pos[1].x, trig.pos[1].y, 0);
    glm::vec3 v3 = glm::vec3(trig.pos[2].x, trig.pos[2].y, 0);
    
    // 计算三角形的面积
    float area_inv = 1.0f / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));

    // 计算重心坐标
    float alpha = ((v2.y - v3.y) * (pos.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.y)) * area_inv;
    float beta = ((v3.y - v1.y) * (pos.x - v3.x) + (v1.x - v3.x) * (pos.y - v3.y)) * area_inv;
    float gamma = 1.0f - alpha - beta;

    // 返回重心坐标
    return glm::vec3(alpha, beta, gamma);
}

// TODO
float Rasterizer::zBufferDefault = float();
float Rasterizer::zBufferDefault = INFINITY;

// TODO
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{

    float result;
    ZBuffer.Set(x, y, result);
    

    return;
}

// TODO
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{

    Color result;
    image.Set(x, y, result);

    return;
}

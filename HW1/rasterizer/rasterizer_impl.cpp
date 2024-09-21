// Yichen Lu nechy@umich.edu
#include <cstdint>
#include "image.hpp"
#include "loader.hpp"
#include "rasterizer.hpp"
#include "../thirdparty/glm/glm.hpp"
#include "../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../thirdparty/glm/gtc/type_ptr.hpp"
#include "../thirdparty/glm/gtx/quaternion.hpp"



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
        if (IsPointInsideTriangle(pixel_center, trig.pos[0], trig.pos[1], trig.pos[2])) //if the pixel is inside the triangle
        {
            //Write to the corresponding pixel
            image.Set(x, y, color); 
        }
    }
    else if (config == AntiAliasConfig::SSAA)       // if anti-aliasing is on
    {
        uint32_t sample_count = 0;
        for (uint32_t i = 0; i < spp; ++i)
        {
            //Generate sample points based on spp;
            float sample_x = static_cast<float>(x) + static_cast<float>(rand()) / RAND_MAX;
            float sample_y = static_cast<float>(y) + static_cast<float>(rand()) / RAND_MAX;
            glm::vec3 sample_point(sample_x, sample_y, 0.0f); 

            if (IsPointInsideTriangle(sample_point, trig.pos[0], trig.pos[1], trig.pos[2])) //if the sample point is inside the triangle
            {
                ++sample_count;
            }
        }

        //Write to the pixel, with value based on count and spp;
        float coverage = static_cast<float>(sample_count) / static_cast<float>(spp);
        Color final_color = color * coverage;
        image.Set(x, y, final_color); //
    }
    return;
}

// TODO
void Rasterizer::AddModel(MeshTransform transform, glm::mat4 rotation)
{
    
    glm::mat4 Mrot = rotation;
    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), transform.translation);
    glm::mat4 Mscale = glm::scale(glm::mat4(1.0f), transform.scale);

    glm::mat4 Mmodel = Mrot * Mtrans * Mscale;

    /* model.push_back( model transformation constructed from translation, rotation and scale );*/
    this->model.push_back(Mmodel);
    return;
}

// TODO
void Rasterizer::SetView()
{  
    //Get Camera from Loader;
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = camera.lookAt;
    glm::vec3 cameraUp = camera.up;

    glm::vec3 g = glm::normalize(cameraLookAt - cameraPos); //Forward
    glm::vec3 f = glm::normalize(glm::cross(g, cameraUp));  //Right
    glm::vec3 h = glm::cross(f, g); //Up

    //Create Mrot by orienting the camera towards negative z-axis;
    glm::mat4 Mrot(1.0f);
    Mrot[0][0] = f.x; Mrot[0][1] = h.x; Mrot[0][2] = -g.x; Mrot[0][3] = 0.0f;
    Mrot[1][0] = f.y; Mrot[1][1] = h.y; Mrot[1][2] = -g.y; Mrot[1][3] = 0.0f;
    Mrot[2][0] = f.z; Mrot[2][1] = h.z; Mrot[2][2] = -g.z; Mrot[2][3] = 0.0f;
    Mrot[3][0] = 0.0f; Mrot[3][1] = 0.0f; Mrot[3][2] = 0.0f; Mrot[3][3] = 1.0f;

    //Create Mtrans by moving the camera to the origin;
    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), -cameraPos);

    //Combine Mrot and Mtrans to get Mview;
    this->view = glm::mat4(1.);
    this->view = Mrot * Mtrans;
    return;
}

// TODO
void Rasterizer::SetProjection()
{
    //Get camera parameters from Loader;
    const Camera& camera = this->loader.GetCamera();
    float nearClip = -camera.nearClip; //positive
    float farClip = -camera.farClip; //positive     
    float camera_width = camera.width;
    float camera_height = camera.height;

    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    //Create Mpersp to squish the objects to conform to perspective
    glm::mat4 Mpersp = glm::mat4(0.0f);
    Mpersp[0][0] = nearClip;
    Mpersp[1][1] = nearClip;
    Mpersp[2][2] = nearClip + farClip; 
    Mpersp[2][3] = 1.0f;
    Mpersp[3][2] = - nearClip * farClip;

    //Create Mortho to rescale and translate the objects to the canonical space
    glm::mat4 Mortho_scale = glm::mat4(1.0f);
    Mortho_scale[0][0] = 2.0f / camera_width;
    Mortho_scale[1][1] = 2.0f / camera_height;
    Mortho_scale[2][2] = 2.0f / (nearClip - farClip);

    glm::mat4 Mortho_trans = glm::mat4(1.0f);
    Mortho_trans[3][2] = - (nearClip + farClip) / 2.0f;

    this->projection = Mortho_scale * Mortho_trans * Mpersp;

    return;
}

// TODO
void Rasterizer::SetScreenSpace()
{   
    //Get camera parameters from loader
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    //Create Mss based on the width and height of the canvas, and preserve the z value;
    glm::mat4 Mss = glm::mat4(1.0f);

    Mss[0][0] = width / 2.0f;
    Mss[1][1] = height / 2.0f;
    Mss[2][2] = 1.0f;
    Mss[3][0] = width / 2.0f; Mss[3][1] = height / 2.0f; Mss[3][2] = 0.0f;

    this->screenspace = Mss;
    
    return;
}

// TODO
glm::vec3 Rasterizer::BarycentricCoordinate(glm::vec2 pos, Triangle trig)
{
    glm::vec3 v1 = glm::vec3(trig.pos[0].x, trig.pos[0].y, 0);
    glm::vec3 v2 = glm::vec3(trig.pos[1].x, trig.pos[1].y, 0);
    glm::vec3 v3 = glm::vec3(trig.pos[2].x, trig.pos[2].y, 0);
    
    float alpha = ((v2.y - v3.y) * (pos.x - v3.x) + (v3.x - v2.x) * (pos.y - v3.y));
    float beta = ((v3.y - v1.y) * (pos.x - v3.x) + (v1.x - v3.x) * (pos.y - v3.y));

    float area = ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    float alpha_nor = alpha / area;
    float beta_nor = beta / area;

    return glm::vec3(alpha_nor, beta_nor, 1.0f - alpha_nor - beta_nor);
}

// TODO
float Rasterizer::zBufferDefault = -1.0f;

// TODO
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{
    glm::vec2 pos(x+0.5f, y+0.5f);
    glm::vec3 v0(transformed.pos[0].x, transformed.pos[0].y, 0.0f);
    glm::vec3 v1(transformed.pos[1].x, transformed.pos[1].y, 0.0f);
    glm::vec3 v2(transformed.pos[2].x, transformed.pos[2].y, 0.0f);
    glm::vec3 pos3(pos, 0.0f);

    if (IsPointInsideTriangle(pos3, v0, v1, v2)){
        glm::vec3 bary = BarycentricCoordinate(pos, transformed);

        //Compute the depth of the pixel using barycentric coordinates of triangle
        float depth = glm::dot(bary, glm::vec3(original.pos[0].z, original.pos[1].z, original.pos[2].z));

        if (depth > ZBuffer.Get(x, y))
        {
            ZBuffer.Set(x, y, depth);
        }
    }

    return;
}

// TODO
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{
    glm::vec2 pos(x+0.5f, y+0.5f);
    glm::vec3 v0(transformed.pos[0].x, transformed.pos[0].y, 0.0f);
    glm::vec3 v1(transformed.pos[1].x, transformed.pos[1].y, 0.0f);
    glm::vec3 v2(transformed.pos[2].x, transformed.pos[2].y, 0.0f);
    glm::vec3 pos3(pos, 0.0f);

    if (IsPointInsideTriangle(pos3, v0, v1, v2))
    {
        glm::vec3 bary = BarycentricCoordinate(pos, transformed);

        //Compute the depth of the pixel using barycentric coordinates of triangle
        float depth = glm::dot(bary, glm::vec3(original.pos[0].z, original.pos[1].z, original.pos[2].z));

        if (depth == this->ZBuffer.Get(x, y))
        {
            glm::vec3 original_coords = bary.x * glm::vec3(original.pos[0]) + bary.y * glm::vec3(original.pos[1]) + bary.z * glm::vec3(original.pos[2]);
            //Retrieve its normal using barycentric coordinates
            glm::vec3 normal = bary.x * glm::vec3(original.normal[0]) + bary.y * glm::vec3(original.normal[1]) + bary.z * glm::vec3(original.normal[2]);
            normal = glm::normalize(normal);

            std::vector<Light> lights = this->loader.GetLights();
            Color ambientColor = this->loader.GetAmbientColor();
            float specularExponent = this->loader.GetSpecularExponent();
            glm::vec3 Camera = this->loader.GetCamera().pos;

            Color result = ambientColor;
            for (auto& light : lights)
            {
                glm::vec3 l = glm::normalize(light.pos - original_coords);
                glm::vec3 h = glm::normalize(l + glm::normalize(Camera - original_coords));
                float r = glm::length(light.pos - original_coords);
                
                Color L_d = light.color * (light.intensity / (std::pow(r, 2)) * std::max(glm::dot(l, normal), 0.0f));
                Color L_s = light.color * (light.intensity / (r * r)) * glm::pow(glm::max(glm::dot(normal, h), 0.0f), specularExponent);

                result = result + L_d + L_s;
            }

            image.Set(x, y, result);
        }
    }
    return;
}
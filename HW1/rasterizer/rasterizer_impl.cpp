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
    glm::mat4 Mrot = rotation;
    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), transform.translation);
    glm::mat4 Mscale = glm::scale(glm::mat4(1.0f), transform.scale);

    glm::mat4 Mmodel = Mrot * Mtrans * Mscale;

    this->model.push_back(Mmodel);

    return;
}

// TODO
void Rasterizer::SetView()
{
    const Camera& camera = this->loader.GetCamera();
    glm::vec3 cameraPos = camera.pos;
    glm::vec3 cameraLookAt = camera.lookAt;
    glm::vec3 cameraUp = camera.up;

    
    // Compute the forward (g), right (f), and up (h) vectors
    glm::vec3 g = glm::normalize(cameraLookAt - cameraPos);    // Forward vector
    glm::vec3 f = glm::normalize(glm::cross(g, cameraUp));     // Right vector
    glm::vec3 h = glm::cross(f, g);                            // Up vector (recomputed for orthogonality)

    // Construct the rotation matrix (Mrot)
    glm::mat4 Mrot(1.0f);
    Mrot[0][0] = f.x; Mrot[0][1] = h.x; Mrot[0][2] = -g.x; Mrot[0][3] = 0.0f;
    Mrot[1][0] = f.y; Mrot[1][1] = h.y; Mrot[1][2] = -g.y; Mrot[1][3] = 0.0f;
    Mrot[2][0] = f.z; Mrot[2][1] = h.z; Mrot[2][2] = -g.z; Mrot[2][3] = 0.0f;
    Mrot[3][0] = 0.0f; Mrot[3][1] = 0.0f; Mrot[3][2] = 0.0f; Mrot[3][3] = 1.0f;

    // Construct the translation matrix (Mtrans)
    glm::mat4 Mtrans = glm::translate(glm::mat4(1.0f), -cameraPos);

    // Combine rotation and translation to get the view matrix
    // TODO change this line to the correct view matrix
    this->view = glm::mat4(1.);
    this->view = Mrot * Mtrans;
    return;
}

// TODO
void Rasterizer::SetProjection()
{
    const Camera& camera = this->loader.GetCamera();

    float nearClip = -camera.nearClip;                   // near clipping distance, strictly positive
    float farClip = -camera.farClip;      
    float camera_width = camera.width;
    float camera_height = camera.height;               // far clipping distance, strictly positive
    
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    // Step 15: Create Mpersp to squish the objects to conform to perspective
    glm::mat4 Mpersp = glm::mat4(0.0f);

    Mpersp[0][0] = nearClip;
    Mpersp[1][1] = nearClip;
    Mpersp[2][2] = nearClip + farClip;
    Mpersp[2][3] = 1.0f;
    Mpersp[3][2] = - nearClip * farClip;

    // Create the scaling matrix for orthographic projection
    glm::mat4 M_ortho_scale = glm::mat4(1.0f);
    M_ortho_scale[0][0] = 2.0f / camera_width;
    M_ortho_scale[1][1] = 2.0f / camera_height;
    M_ortho_scale[2][2] = 2.0f / (nearClip - farClip); // Negative to account for the z-axis direction

    // Create the translation matrix for orthographic projection
    glm::mat4 M_ortho_trans = glm::mat4(1.0f);
    M_ortho_trans[3][2] = - (nearClip + farClip) / 2.0f;                        // Centers the z-axis

    // Combine the scaling and translation to get the orthographic matrix
    glm::mat4 M_ortho = M_ortho_scale * M_ortho_trans;

    // Step 17: Combine Mpersp and Mortho to get Mprojection
    this->projection = M_ortho * Mpersp;

    return;
}

// TODO
void Rasterizer::SetScreenSpace()
{
    float width = this->loader.GetWidth();
    float height = this->loader.GetHeight();

    // TODO change this line to the correct screenspace matrix
    // Initialize the screen space transformation matrix to identity
    glm::mat4 screenspace = glm::mat4(1.0f);

    // Scale x and y by width/2 and height/2 respectively
    screenspace[0][0] = width / 2.0f;   // Scale x
    screenspace[1][1] = height / 2.0f; // Scale y and flip y-axis (if needed)
    screenspace[2][2] = 1.0f;           // Preserve z

    // Translate x and y by width/2 and height/2 respectively
    screenspace[3][0] = width / 2.0f;   // Translate x
    screenspace[3][1] = height / 2.0f;  // Translate y
    screenspace[3][2] = 0.0f;           // No translation in z

    this->screenspace = screenspace;
    
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
float Rasterizer::zBufferDefault = -1.1f;

// TODO
void Rasterizer::UpdateDepthAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, ImageGrey& ZBuffer)
{
    glm::vec2 pos(x+0.5f, y+0.5f);

    // Extract triangle vertices in screen space
    glm::vec3 v0(transformed.pos[0].x, transformed.pos[0].y, 0.0f);
    glm::vec3 v1(transformed.pos[1].x, transformed.pos[1].y, 0.0f);
    glm::vec3 v2(transformed.pos[2].x, transformed.pos[2].y, 0.0f);
    glm::vec3 pos3(pos, 0.0f);

    // Check if the point is inside the triangle using IsPointInsideTriangle
    if (IsPointInsideTriangle(pos3, v0, v1, v2)){
        // Compute barycentric coordinates at this pixel
        glm::vec3 bary = BarycentricCoordinate(pos, transformed);
        // Get depth values at the triangle vertices (after projection and perspective divide)
        float result = glm::dot(bary, glm::vec3(original.pos[0].z, original.pos[1].z, original.pos[2].z));
        // float currentDepth = ZBuffer.Get(x, y).value_or(Rasterizer::zBufferDefault);
        std::optional<float> currentDepth = ZBuffer.Get(x, y);
        if (result > currentDepth)
        {
            ZBuffer.Set(x, y, result);
        }
    }

    return;
}

// TODO
// void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
// {
//     glm::vec2 pos(x+0.5f, y+0.5f);

//     // Extract triangle vertices in screen space
//     glm::vec3 v0(transformed.pos[0].x, transformed.pos[0].y, 0.0f);
//     glm::vec3 v1(transformed.pos[1].x, transformed.pos[1].y, 0.0f);
//     glm::vec3 v2(transformed.pos[2].x, transformed.pos[2].y, 0.0f);
//     glm::vec3 pos3(pos, 0.0f);

//     // Check if the point is inside the triangle using IsPointInsideTriangle
//     if (!IsPointInsideTriangle(pos3, v0, v1, v2)){
//         return;
//     }
//     glm::vec3 bary = BarycentricCoordinate(pos, transformed);

//     float depth = glm::dot(bary, glm::vec3(original.pos[0].z, original.pos[1].z, original.pos[2].z));
//     std::optional<float> currentdepth = this->ZBuffer.Get(x, y);
//     if (depth == currentdepth){
        
//         glm::vec3 worldPos0 = glm::vec3(original.pos[0]); // Discard the w component
//         glm::vec3 worldPos1 = glm::vec3(original.pos[1]);
//         glm::vec3 worldPos2 = glm::vec3(original.pos[2]);
    
//         // Interpolate world-space position
//         glm::vec3 worldPos = bary.x * worldPos0 + bary.y * worldPos1 + bary.z * worldPos2;
    
//         glm::vec3 normal0 = original.normal[0];
//         glm::vec3 normal1 = original.normal[1];
//         glm::vec3 normal2 = original.normal[2];
    
//         glm::vec3 normal = bary.x * normal0 + bary.y * normal1 + bary.z * normal2;
//         normal = glm::normalize(normal);
    
//         std::vector<Light> lights = this->loader.GetLights();
//         Color ambient = this->loader.GetAmbientColor();
//         float specularExponent = this->loader.GetSpecularExponent();
//         glm::vec3 cameraPos = this->loader.GetCamera().pos;
    
//         glm::vec3 color = glm::vec3(ambient.r, ambient.g, ambient.b);
    
//         for (const Light& light : lights)
//         {
//             glm::vec3 lightPos = light.pos; // Assuming 'pos' is the position in 'Light'
//             glm::vec3 lightColor = glm::vec3(light.color.r, light.color.g, light.color.b);
//             float lightIntensity = light.intensity;
    
//             // Compute vectors for shading
//             glm::vec3 lightDir = glm::normalize(lightPos - worldPos);
//             glm::vec3 viewDir = glm::normalize(cameraPos - worldPos);
//             glm::vec3 halfVector = glm::normalize(lightDir + viewDir);
    
//             // Distance attenuation
//             float distanceSquared = glm::dot(lightPos - worldPos, lightPos - worldPos);
//             float attenuation = 1.0f / distanceSquared;
    
//             // Compute NdotL and NdotH as floats
//             float NdotL = glm::max(glm::dot(normal, lightDir), 0.0f);
//             float NdotH = glm::max(glm::dot(normal, halfVector), 0.0f);
    
//             float specularExponent = static_cast<float>(this->loader.GetSpecularExponent());
//              // Now compute diffuse and specular components
//             glm::vec3 diffuse = lightColor * lightIntensity * attenuation * NdotL;
//             float specularFactor = powf(NdotH, specularExponent);
//             glm::vec3 specular = lightColor * lightIntensity * attenuation * specularFactor;
//             // Accumulate color contributions
//             color += diffuse + specular;
//         }
//         // Clamp the final color to [0, 1]
//         color = glm::clamp(color, 0.0f, 1.0f);
    
//         // Step 8: Write the Color to the Image Buffer
//         Color result(color);
//         image.Set(x, y, result);

//     }

//     return;
// }
glm::vec3 CalculateCoordsWithBarycentric(glm::vec3 barycentric, std::array<glm::vec4, 3> original)
{
    glm::vec3 coords = barycentric.x * glm::vec3(original[0]) + barycentric.y * glm::vec3(original[1]) + barycentric.z * glm::vec3(original[2]);
    return coords;
}


glm::vec3 CalculateNormal(glm::vec3 barycentric, Triangle original)
{
    glm::vec3 normal = CalculateCoordsWithBarycentric(barycentric, original.normal);
    return glm::normalize(normal);
}

Color CalculateColor_BlinnPhong(glm::vec3 pos, glm::vec3 normal, glm::vec3 view_pos, std::vector<Light> lights, Color ambient, float specularExponent)
{
    Color result = Color(0.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 lightDir;
    glm::vec3 half;
    float diffuse;
    float specular;
    Color ambientColor = ambient;
    Color diffuseColor;
    Color specularColor;
    glm::vec3 view = glm::normalize(view_pos - pos);

    float diffuse_decay;
    float specular_decay;
    result = ambientColor;

    specularExponent = specularExponent;
    for (auto& light : lights)
    {
        // Calculate the light direction
        lightDir = glm::normalize(light.pos - pos);

        // Calculate the half vector
        half = glm::normalize(lightDir + view);

        // Calculate the diffuse term
        diffuse = glm::max(glm::dot(normal, lightDir), 0.0f);

        // Calculate the specular term
        specular = glm::pow(glm::max(glm::dot(normal, half), 0.0f), specularExponent);

        // Calculate the light decay
        diffuse_decay = light.intensity / (glm::length(light.pos - pos) * glm::length(light.pos - pos));

        // Calculate the diffuse term
        diffuseColor = diffuse_decay * light.color * diffuse ;

        // Calculate the specular decay
        specular_decay = light.intensity / (glm::length(light.pos - pos) * glm::length(light.pos - pos));

        // Calculate the specular term
        specularColor = specular_decay* light.color * specular;

        // Calculate the final color
        result = diffuseColor + specularColor + result;
    }
    return result;
}

// TODO
void Rasterizer::ShadeAtPixel(uint32_t x, uint32_t y, Triangle original, Triangle transformed, Image& image)
{

    // Calculate the barycentric coordinates of the pixel
    Color result;
    float depth;
    
    std::vector<Light> lights = this->loader.GetLights();
    Color ambient = this->loader.GetAmbientColor();
    float specularExponent = this->loader.GetSpecularExponent();
    glm::vec3 cam_pos = this->loader.GetCamera().pos;

    glm::vec2 pos(x+0.5f, y+0.5f);

    // Extract triangle vertices in screen space
    glm::vec3 v0(transformed.pos[0].x, transformed.pos[0].y, 0.0f);
    glm::vec3 v1(transformed.pos[1].x, transformed.pos[1].y, 0.0f);
    glm::vec3 v2(transformed.pos[2].x, transformed.pos[2].y, 0.0f);
    glm::vec3 pos3(pos, 0.0f);

    if (IsPointInsideTriangle(pos3, v0, v1, v2))
    {
        glm::vec3 barycentric = BarycentricCoordinate(glm::vec2(x + 0.5, y + 0.5), transformed);           // Bug Fix: x + 0.5, y + 0.5, or the pixel will be at the top-left corner of the triangle

        // Calculate the original depth of the pixel
        depth = glm::dot(barycentric, glm::vec3(original.pos[0].z, original.pos[1].z, original.pos[2].z));

        if (depth == this->ZBuffer.Get(x, y))
        {
            // Calculate the normal of the pixel
            glm::vec3 normal = CalculateNormal(barycentric, original);

            glm::vec3 original_coords = CalculateCoordsWithBarycentric(barycentric, original.pos);

            // glm::vec3 view_pos = glm::normalize(cam_pos);

            result = CalculateColor_BlinnPhong(original_coords, normal, cam_pos, lights, ambient, specularExponent);

            image.Set(x, y, result);
        }
    }
    return;
}
#include "Scene.h"
#include "Config.h"
#include <iostream>
#include <filesystem>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// Task 5
/*
Vec3 Scene::trace(const Ray &ray, int bouncesLeft, bool discardEmission) {
    if constexpr(DEBUG) {
        assert (ray.isNormalized());
    }
    if (bouncesLeft < 0) return {};
    Intersection inter = getIntersection(ray);
    if (!inter.happened) return {};
    
    return inter.getDiffuseColor();
}
*/

//Task 6
/*
Vec3 Scene::trace(const Ray &ray, int bouncesLeft, bool discardEmission) {
    if constexpr(DEBUG) {
        assert (ray.isNormalized());
    }
    if (bouncesLeft < 0) return {};
    Intersection inter = getIntersection(ray);

    if (!inter.happened) return {};
    
    Vec3 Lo(0.0f, 0.0f, 0.0f);
    if (!discardEmission) {
        Lo += inter.getEmission();
    }

    Vec3 randomDir = Random::randomHemisphereDirection(inter.getNormal());
    Ray randomRay = {inter.pos, randomDir};
    Intersection randominter = getIntersection(randomRay); 

    Vec3 Li = Vec3(0.0f, 0.0f, 0.0f);
    if (randominter.happened) {
        float pdf = 1.0f / (2.0f * PI);
        Vec3 brdf = inter.calcBRDF(-randomDir, -ray.dir);
        float cosineTerm = randomDir.dot(inter.getNormal());
        Li += 1 / pdf * randominter.getEmission() * brdf * cosineTerm;
    }

    return Lo + Li;
}
*/

//Task 7
/*
Vec3 Scene::trace(const Ray &ray, int bouncesLeft, bool discardEmission) {
    if constexpr(DEBUG) {
        assert (ray.isNormalized());
    }
    if (bouncesLeft < 0) return {};
    Intersection inter = getIntersection(ray);

    if (!inter.happened) return {};

    Vec3 randomDir = Random::randomHemisphereDirection(inter.getNormal());
    Ray nextRay(inter.pos, randomDir);
    Intersection randominter = getIntersection(nextRay); 

    Vec3 Lo(0.0f, 0.0f, 0.0f);
    if (!discardEmission) {
        Lo += inter.getEmission();
    }
    
    if (randominter.happened) {
        float pdf = 1.0f / (2.0f * PI);
        Vec3 brdf = inter.calcBRDF(-randomDir, -ray.dir);
        float cosineTerm = randomDir.dot(inter.getNormal());
        
        Vec3 Li = trace(nextRay, bouncesLeft - 1, false);
        Lo += 1 / pdf * Li * brdf * cosineTerm;
    }

    return Lo;
}
*/

//Task 8
Vec3 Scene::trace(const Ray &ray, int bouncesLeft, bool discardEmission) {
    if constexpr(DEBUG) {
        assert (ray.isNormalized());
    }
    if (bouncesLeft < 0) return {};
    Intersection inter = getIntersection(ray);

    if (!inter.happened) return {};

    Vec3 Lo(0.0f, 0.0f, 0.0f);
    if (!discardEmission) {
        Lo += inter.getEmission();
    }

    Vec3 indirectRadiance(0.0f, 0.0f, 0.0f);

    Vec3 randomDir = Random::cosWeightedHemisphere(inter.getNormal());  
    Ray randomRay(inter.pos, randomDir);
    Intersection bounceInter = getIntersection(randomRay);

    if (bounceInter.happened) {
        Vec3 Li = trace(randomRay, bouncesLeft - 1, true);
        Vec3 brdf = inter.calcBRDF(-randomDir, -ray.dir);
        float cosineTerm = randomDir.dot(inter.getNormal());
        float pdf = cosineTerm / PI;
        indirectRadiance = (Li * brdf * cosineTerm) / pdf;
    }

    Vec3 directRadiance(0.0f, 0.0f, 0.0f);

    Intersection lightSample = sampleLight();
    Vec3 lightDir = lightSample.pos - inter.pos;
    float distanceToLight = lightDir.getLength();
    lightDir.normalize();

    Ray rayToLight = {inter.pos, lightDir};
    Intersection lightInter = getIntersection(rayToLight);

    if (lightInter.happened && (lightInter.object == lightSample.object)) {

        Vec3 brdf = inter.calcBRDF(-lightDir, -ray.dir);
        float cosineTerm = lightDir.dot(inter.getNormal());
        float lightCosineTerm = -lightDir.dot(lightSample.getNormal());

        float pdfLightSample = 1.0f / lightArea;
        float attenuation = 1.0f / (distanceToLight * distanceToLight);

        directRadiance = (lightSample.getEmission() * brdf * cosineTerm * lightCosineTerm * attenuation) / pdfLightSample;
    }

    Lo += indirectRadiance + directRadiance;

    return Lo;
}

tinyobj::ObjReader Scene::reader {};

void Scene::addObjects(std::string_view modelPath, std::string_view searchPath) {
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = searchPath;
    if (!reader.ParseFromFile(std::string(modelPath), config)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
            std::filesystem::path relative(modelPath);
            std::cerr << "Reading file " << std::filesystem::absolute(relative) << " error. File may be malformed or not exist.\n";
        }
        exit(1);
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();
    auto& materials = reader.GetMaterials();

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        auto* object = new Object();
        object->name = shapes[s].name;
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
            std::vector<Vec3> positions;
            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
                tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
                tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

                positions.push_back({vx, vy, vz});
            } // per-vertex
            index_offset += fv;
            Mesh mesh {positions[0], positions[1], positions[2]};
            object->area += mesh.area;
            object->meshes.push_back(std::move(mesh));
        } // per-face
        object->constructBoundingBox();
        // we assume each object uses only a single material for all meshes
        auto materialId = shapes[s].mesh.material_ids[0];
        auto& material = materials[materialId];
        object->kd = Vec3 {
            material.diffuse[0],
            material.diffuse[1],
            material.diffuse[2],
        };
        if (material.emission[0] + 
            material.emission[1] + 
            material.emission[2] > 0) { // is light
            object->ke = Vec3 {
                material.emission[0], 
                material.emission[1],
                material.emission[2]
            };
            object->hasEmission = true;
            lights.push_back(object);
            lightArea += object->area;
        }
        objects.push_back(object);
    } // per-shape
}

void Scene::constructBVH() {
    assert (!objects.empty());
    bvh.root = BVH::build(objects);
}

Intersection Scene::getIntersection(const Ray &ray) {
    assert (bvh.root);
    return bvh.root->intersect(ray);
}

Intersection Scene::sampleLight() const {
    assert (lights.size() == 1 && "Currently only support a single light object");
    assert (lightArea > 0.0f);
    Intersection inter;
    return lights[0]->sample();
}

Scene::~Scene() {
    for (auto obj : objects) {
        delete obj;
    }
}

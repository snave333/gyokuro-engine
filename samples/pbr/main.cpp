#include <iostream>
#include <string>

#include <gyo/gyo.h>

using namespace gyo;

void loadScene(SceneController& sc);

int main(int argc, const char * argv[]) {
    std::cout << "Starting..." << std::endl;

    gyo::Engine* engine = new gyo::Engine(1280, 720);

    if(!engine->IsRunning()) {
        std::cerr << "Engine failed to start." << std::endl;
        return 1;
    }

    loadScene(engine->sc());

    while(engine->IsRunning()) {
        engine->Frame();
    }

    std::cout << "Shutting down..." << std::endl;
    
    delete engine;

    return 0;
}

void loadScene(SceneController& sc) {
    // sc.SetEnvironment("brown_photostudio_2k.hdr");
    // sc.SetEnvironment("dikhololo_night_2k.hdr");
    sc.SetEnvironment("sunny_rose_garden_2k.hdr");

    // first our lights

    float dist = 3;
    float lz = -3;
    glm::vec3 lColor = glm::vec3(1, 1, 1) * 4.0f;

    LightNode* p1 = new LightNode(new PointLight(lColor));
    ModelNode* p1M = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(lColor, 1.0f)))));
    p1->Translate(dist, dist, lz);
    p1M->Translate(dist, dist, lz);
    sc.AddNode(p1);
    sc.AddNode(p1M);

    LightNode* p2 = new LightNode(new PointLight(lColor));
    ModelNode* p2M = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(lColor, 1.0f)))));
    p2->Translate(-dist, dist, lz);
    p2M->Translate(-dist, dist, lz);
    sc.AddNode(p2);
    sc.AddNode(p2M);

    LightNode* p3 = new LightNode(new PointLight(lColor));
    ModelNode* p3M = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(lColor, 1.0f)))));
    p3->Translate(dist, -dist, lz);
    p3M->Translate(dist, -dist, lz);
    sc.AddNode(p3);
    sc.AddNode(p3M);

    LightNode* p4 = new LightNode(new PointLight(lColor));
    ModelNode* p4M = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(lColor, 1.0f)))));
    p4->Translate(-dist, -dist, lz);
    p4M->Translate(-dist, -dist, lz);
    sc.AddNode(p4);
    sc.AddNode(p4M);

    // spawn our grid of spheres
#if 0

    float z = -0.5f;
    float spacing = 0.6f;

    int numRows = 5;
    int numColumns = 5;    
    float metalness = 0;
    float roughness = 0;

    for(int i = 0; i < numRows; i++) {
        float y = -floorf(numRows * 0.5f) + i;
        metalness = (float)i / (float)(numRows - 1);

        for(int j = 0; j < numColumns; j++) {
            float x = -floorf(numColumns * 0.5f) + j;
            roughness = glm::clamp(1.0f - (float)j / (float)(numColumns - 1), 0.05f, 1.0f);

            // std::cout << std::to_string(metalness) << ", " << std::to_string(roughness) << std::endl;

            ModelNode* model = new ModelNode(new Model(new Mesh(new Sphere(0.5f), new PBRMaterial(
                true,
                { 0, 0.8f, 1 },
                metalness,
                roughness
            ))));
            model->SetPosition(x * spacing, y * spacing, z);
            model->SetScale(0.5f);
            sc.AddNode(model);
        }
    }
#else
    Material* brickMat = new PBRMaterial(
        true,
        glm::vec3(1),
        1, 1, 1,
        glm::vec3(0),
        Resources::GetTexture("alley-brick-wall_albedo.png", true),
        Resources::GetTexture("alley-brick-wall_normal-ogl.png", false),
        Resources::GetTexture("alley-brick-wall_metallic.png", false),
        Resources::GetTexture("alley-brick-wall_roughness.png", false),
        nullptr,
        Resources::GetTexture("alley-brick-wall_ao.png", false)
    );
    ModelNode* sphere1 = new ModelNode(new Model(new Mesh(new Sphere(0.5f), brickMat)));
    sc.AddNode(sphere1);
#endif
}

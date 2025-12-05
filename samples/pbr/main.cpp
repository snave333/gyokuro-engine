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
    sc.SetEnvironment("dikhololo_night_2k.hdr");
    // sc.SetEnvironment("sunny_rose_garden_2k.hdr");

#if 0
    // first our lights

    float dist = 3;
    float lz = -3;
    glm::vec3 lColor = glm::vec3(1, 1, 1) * 10.0f;

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
    glm::vec3 lColor = glm::vec3(1, 1, 1) * 10.0f;
    LightNode* p2 = new LightNode(new PointLight(lColor));
    ModelNode* p2M = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(lColor, 1.0f)))));
    p2->Translate(-1.2, 2, 0);
    p2M->Translate(-1.2, 2, 0);
    sc.AddNode(p2);
    sc.AddNode(p2M);

    Material* stoneMat = new PBRMaterial(
        true,
        glm::vec3(1),
        1, 1, 1,
        glm::vec3(0),
        Resources::GetTexture("old-middle-eastern-wall_albedo.png", true),
        Resources::GetTexture("old-middle-eastern-wall_normal-ogl.png", false),
        nullptr,
        nullptr,
        Resources::GetTexture("old-middle-eastern-wall_metallic-roughness.png", false),
        Resources::GetTexture("old-middle-eastern-wall_ao.png", false),
        nullptr,
        glm::vec2(2.0f)
    );
    Material* goldMat = new PBRMaterial(
        true,
        glm::vec3(1),
        1, 1, 1,
        glm::vec3(0),
        Resources::GetTexture("gold-scuffed_basecolor.png", true),
        Resources::GetTexture("gold-scuffed_normal.png", false),
        Resources::GetTexture("gold-scuffed_metallic.png", false),
        Resources::GetTexture("gold-scuffed_roughness.png", false)
    );
    Material* rustedIronMat = new PBRMaterial(
        true,
        glm::vec3(1),
        1, 1, 1,
        glm::vec3(0),
        Resources::GetTexture("rustediron2_basecolor.png", true),
        Resources::GetTexture("rustediron2_normal.png", false),
        Resources::GetTexture("rustediron2_metallic.png", false),
        Resources::GetTexture("rustediron2_roughness.png", false)
    );
    Material* plasticMat = new PBRMaterial(
        true,
        glm::vec3(0.0, 0.5, 1.0),
        1, 1, 1,
        glm::vec3(0),
        Resources::GetTexture("scuffed-plastic-alb.png", true),
        Resources::GetTexture("scuffed-plastic-normal.png", false),
        Resources::GetTexture("scuffed-plastic-metal.png", false),
        Resources::GetTexture("scuffed-plastic-rough.png", false),
        nullptr,
        Resources::GetTexture("scuffed-plastic-ao.png", false)
    );

    ModelNode* sphere1 = new ModelNode(new Model(new Mesh(new Sphere(0.5f), stoneMat)));
    sphere1->SetPosition(1.8, 0, 0);
    sc.AddNode(sphere1);

    ModelNode* sphere2 = new ModelNode(new Model(new Mesh(new Sphere(0.5f), goldMat)));
    sphere2->SetPosition(0.6, 0, 0);
    sc.AddNode(sphere2);

    ModelNode* sphere3 = new ModelNode(new Model(new Mesh(new Sphere(0.5f), rustedIronMat)));
    sphere3->SetPosition(-0.6, 0, 0);
    sc.AddNode(sphere3);

    ModelNode* sphere4 = new ModelNode(new Model(new Mesh(new Sphere(0.5f), plasticMat)));
    sphere4->SetPosition(-1.8, 0, 0);
    sc.AddNode(sphere4);
#endif
}

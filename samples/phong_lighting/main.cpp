#include <iostream>

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
    // add the lights first

    glm::vec3 pointLight1Color = glm::vec3(0.2f, 0.6f, 1);
    LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color));
    ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
    pointLight1->Translate(3, -1, 4);
    pointLight1Model->Translate(3, -1, 4);
    sc.AddNode(pointLight1);
    sc.AddNode(pointLight1Model);

    glm::vec3 pointLight2Color = glm::vec3(1, 0.6f, 0.2f);
    LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color * 10.0f));
    ModelNode* pointLight2Model = new ModelNode(new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(glm::vec4(pointLight2Color, 1.0f)))));
    pointLight2->Translate(-3, -1, 4);
    pointLight2Model->Translate(-3, -1, 4);
    sc.AddNode(pointLight2);
    sc.AddNode(pointLight2Model);

    glm::vec3 spotLight1Color = glm::vec3(0.6f, 0.2f, 1.0f);
    LightNode* spotLight1 = new LightNode(new SpotLight(spotLight1Color * 6.0f, 20.0f));
    ModelNode* spotLight1Model = new ModelNode(new Model(new Mesh(new Pyramid(0.1f, 0.2f), new UnlitMaterial(glm::vec4(spotLight1Color, 1.0f)))));
    spotLight1->Translate(4, 1, -5);
    spotLight1Model->Translate(4, 1, -5);
    spotLight1->Rotate(45, 0, 0);
    spotLight1Model->Rotate(-45, 0, 0);
    sc.AddNode(spotLight1);
    sc.AddNode(spotLight1Model);

    glm::vec3 spotLight2Color = glm::vec3(1.0f, 0.2f, 0.2f);
    LightNode* spotLight2 = new LightNode(new SpotLight(spotLight2Color * 6.0f, 40.0f));
    ModelNode* spotLight2Model = new ModelNode(new Model(new Mesh(new Pyramid(0.15f, 0.2f), new UnlitMaterial(glm::vec4(spotLight2Color, 1.0f)))));
    spotLight2->Translate(2, 1, -5);
    spotLight2Model->Translate(2, 1, -5);
    spotLight2->Rotate(45, -15, 0);
    spotLight2Model->Rotate(-45, -15, 0);
    sc.AddNode(spotLight2);
    sc.AddNode(spotLight2Model);

    // next add the models

    ModelNode* floor = new ModelNode(new Model(new Mesh(new Quad(), new PhongMaterial(
        { 1, 1, 1, 1 }, { 1, 1, 1 }, 64,
        Resources::GetTexture("brick_DIFF.jpg", true),
        nullptr,
        Resources::GetTexture("brick_NRM.jpg", false),
        glm::vec2(4)))));
    floor->Translate(0, -2, 0);
    floor->Rotate(-90, 0, 0);
    floor->Scale(10);
    sc.AddNode(floor);

    ModelNode* crate = new ModelNode(new Model(new Mesh(new Cube(),
        new PhongMaterial(
            { 1, 1, 1, 1 },
            { 1, 1, 1 },
            128,
            Resources::GetTexture("crate_DIFF.jpg", true),
            Resources::GetTexture("crate_SPEC.jpg", false)))));
    crate->Translate(0, -1, 4);
    sc.AddNode(crate);

    // our update function

    sc.AddUpdateFunction([crate](float dt) {
        crate->Rotate(dt * -60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    });
}

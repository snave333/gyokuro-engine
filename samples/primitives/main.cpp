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
    LightNode* dirLight = new LightNode(new DirectionalLight(glm::vec3(1, 1, 0.8f)));
    dirLight->Rotate(45, 60, 0);
    sc.AddNode(dirLight);

    // the models

    ModelNode* floor = new ModelNode(new Model(new Mesh(new Quad(), new PhongMaterial())));
    floor->Translate(0, -2, 0);
    floor->Rotate(-90, 0, 0);
    floor->Scale(10);
    sc.AddNode(floor);

    ModelNode* cube = new ModelNode(new Model(new Mesh(new Cube(), new PhongMaterial())));
    cube->Translate(-3, 0, 4);
    sc.AddNode(cube);

    ModelNode* pyramid = new ModelNode(new Model(new Mesh(new Pyramid(), new PhongMaterial())));
    pyramid->Translate(-1, -0.5, 4);
    sc.AddNode(pyramid);

    ModelNode* sphere = new ModelNode(new Model(new Mesh(new Sphere(), new PhongMaterial())));
    sphere->Translate(1, 0, 4);
    sc.AddNode(sphere);

    ModelNode* torus = new ModelNode(new Model(new Mesh(new Torus(), new PhongMaterial())));
    torus->Translate(3, 0, 4);
    sc.AddNode(torus);

    // our update function

    sc.AddUpdateFunction([cube, pyramid, sphere, torus](float dt) {
        cube->Rotate(dt * -60, glm::normalize(glm::vec3(1.0, 0.5, 0.0)));
        pyramid->Rotate(dt * 60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
        sphere->Rotate(dt * -60, glm::normalize(glm::vec3(0.0, 1.0, 0.5)));
        torus->Rotate(dt * 60, glm::normalize(glm::vec3(0.0, 0.5, 1.0)));
    });
}

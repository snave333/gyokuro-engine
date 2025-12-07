#include <iostream>

#include <gyo/gyo.h>

using namespace gyo;

void loadScene(SceneController& sc);

int main(int argc, const char * argv[]) {
    std::cout << "Starting..." << std::endl;

    gyo::Engine engine(1280, 720);

    if(!engine.IsRunning()) {
        std::cerr << "Engine failed to start." << std::endl;
        return 1;
    }

    loadScene(engine.sc());

    while(engine.IsRunning()) {
        engine.Frame();
    }

    std::cout << "Shutting down..." << std::endl;

    return 0;
}

void loadScene(SceneController& sc) {
    // light

    glm::vec3 pointLight1Color = glm::vec3(1, 1, 1) * 4.0f;
    LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color));
    ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
    pointLight1->Translate(-3, 1, -2);
    pointLight1Model->Translate(-3, 1, -2);
    sc.AddNode(pointLight1);
    sc.AddNode(pointLight1Model);

    // models

    ModelNode* m1 = new ModelNode(new Model(new Mesh(new Torus(), new GoochMaterial())));
    m1->Scale(2);
    sc.AddNode(m1);
}

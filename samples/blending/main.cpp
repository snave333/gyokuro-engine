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
    std::vector<const char*> faces {
        "skybox_px.jpg",
        "skybox_nx.jpg",
        "skybox_py.jpg",
        "skybox_ny.jpg",
        "skybox_nz.jpg",
        "skybox_pz.jpg"
    };
    Skybox* skybox = new Skybox(Resources::GetTextureCube(faces, true));
    sc.SetSkybox(skybox);

    // transparent objects

    ModelNode* m1 = new ModelNode(new Model(new Mesh(new Cube(0.5f), new UnlitMaterial(glm::vec4(1), false, Resources::GetTexture("window.png", true)))));
    // m1->Rotate(180, 0, 0);
    sc.AddNode(m1);

    ModelNode* m2 = new ModelNode(new Model(new Mesh(new Quad(0.5f), new UnlitMaterial(glm::vec4(1), false, Resources::GetTexture("window.png", true)))));
    m2->Translate(0.75f, 0.25f, 2);
    m2->Rotate(180, 0, 0);
    sc.AddNode(m2);

    ModelNode* m3 = new ModelNode(new Model(new Mesh(new Quad(0.5f), new UnlitMaterial(glm::vec4(0, 0.5f, 0.8f, 0.2f)))));
    m3->Translate(0, -0.25f, 3);
    m3->Rotate(180, 0, 0);
    sc.AddNode(m3);

    // additive

    ModelNode* mAdd = new ModelNode(new Model({
        new Mesh(new Cube(0.5f), new UnlitMaterial({ 1, 0.5, 0, 1 }, true)),
        new Mesh(new Sphere(0.65f), new UnlitMaterial({ 0, 1, 0.5, 1 }, true)),
    }));
    mAdd->Translate(-2, 0, 0);
    sc.AddNode(mAdd);
}

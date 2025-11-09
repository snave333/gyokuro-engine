#include <iostream>

#include <gyo/gyo.h>

using namespace gyo;

void loadScene(SceneController& sc);

int main(int argc, const char * argv[]) {
    std::cout << "Starting..." << std::endl;

    gyo::Engine* engine = new gyo::Engine();

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

    ModelNode* torus = new ModelNode(new Model(new Mesh(new Torus(), new UnlitMaterial({1, 0.5, 0, 1}))));
    torus->Translate(0, 0, 4);
    sc.AddNode(torus);
}

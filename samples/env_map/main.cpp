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
    Skybox* skybox = new Skybox(Resources::GetHDRTexture("brown_photostudio_2k.hdr"));
    sc.SetSkybox(skybox);

    ModelNode* torus = new ModelNode(new Model(new Mesh(new Torus(), new PBRMaterial())));
    torus->Translate(0, 0, 3);
    sc.AddNode(torus);
}

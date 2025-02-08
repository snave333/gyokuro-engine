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
    int w = 10;
    int h = 5;
    int d = 10;
    
    float spacing = 2;
    float halfW = w / 2.0f;
    float halfH = h / 2.0f;
    float halfD = d / 2.0f;

    // add a bunch of cubes in a 3d grid to test VFC

    for(int x = -floorf(halfW); x < ceilf(halfW); x++) {
        for(int y = -floorf(halfH); y < ceilf(halfH); y++) {
            for(int z = -floorf(halfD); z < ceilf(halfD); z++) {
                ModelNode* model = new ModelNode(new Model(new Mesh(new Cube(), new UnlitMaterial())));
                model->SetPosition(x * spacing, y * spacing, z * spacing);
                model->SetScale(0.5f);
                sc.AddNode(model);
            }
        }
    }
}

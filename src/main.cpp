//
//  main.cpp
//  Gyokuro
//
//  Created by Spencer Evans on 12/16/24.
//

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <core/Engine.h>
#include <scene_loader.h>

int main(int argc, const char * argv[]) {
    std::cout << "Starting..." << std::endl;

    Engine* engine = new Engine(1280, 720);

    if(!engine->IsRunning()) {
        std::cout << "Engine failed to start." << std::endl;
        return 1;
    }

    // SceneLoader::LoadVFCScene(engine->sc());
    // SceneLoader::LoadLightingScene(engine->sc());
    // SceneLoader::LoadTransparencyScene(engine->sc());
    // SceneLoader::LoadDrawablesScene(engine->sc());
    // SceneLoader::LoadGoochScene(engine->sc());
    SceneLoader::LoadModelScene(engine->sc());

    while(engine->IsRunning()) {
        engine->Frame();
    }
    
    delete engine;

    return 0;
}

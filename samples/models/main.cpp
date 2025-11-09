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
    // some lighting

    glm::vec3 pointLight1Color = glm::vec3(0.8f, 0.8f, 1);
    LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color * 2.0f));
    ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
    glm::vec3 p1Position = { 3, -2, -2 };
    pointLight1->Translate(p1Position);
    pointLight1Model->Translate(p1Position);

    glm::vec3 pointLight2Color = glm::vec3(1, 1, 0.8f);
    LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color * 40.0f));
    ModelNode* pointLight2Model = new ModelNode(new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(glm::vec4(pointLight2Color, 1.0f)))));
    glm::vec3 p2Position = { -3, 2, -2 };
    pointLight2->Translate(p2Position);
    pointLight2Model->Translate(p2Position);

    sc.AddNode(pointLight1);
    sc.AddNode(pointLight2);

    sc.AddNode(pointLight1Model);
    sc.AddNode(pointLight2Model);

    // the models we're loading

    ModelNode* helmet = new ModelNode(Resources::GetModel("DamagedHelmet.glb", true));
    helmet->Translate(-1, 0, 0);
    sc.AddNode(helmet);

    Model* diceModel = Resources::GetModel("Dice.fbx", false);
    diceModel->GetMeshes()[0]->SetMaterial(new PhongMaterial(
        glm::vec4(1), glm::vec4(1), 128,
        Resources::GetTexture("Dice_Diffuse.png", true),
        Resources::GetTexture("Dice_SpecularGlossiness.png", false),
        Resources::GetTexture("Dice_Normal.png", false)
    ));

    ModelNode* dice = new ModelNode(diceModel);
    dice->Translate(1, 0, 0);
    dice->Scale(0.5f);
    sc.AddNode(dice);

    sc.AddUpdateFunction([helmet, dice](float dt) {
        helmet->Rotate(0, dt * -15, 0);
        dice->Rotate(0, dt * -15, 0);
    });
}

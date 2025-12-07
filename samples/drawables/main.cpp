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
    // meshes objects

    ModelNode* m1 = new ModelNode(new Model({
        new Mesh(new Cube(0.5f), new UnlitMaterial({ 1, 0.5, 0, 1 })),
        new Mesh(new Sphere(0.65f), new UnlitMaterial({ 0, 1, 0.5, 1 }, true)),
    }));
    m1->Translate(1, 0, 0);
    sc.AddNode(m1);

    Geometry* geo = new Sphere(0.5f);
    ModelNode* m2 = new ModelNode(new Model(new Mesh(geo, new UnlitMaterial({ 0, 0.5, 1, 1 }))));
    m2->Translate(-1, 0, 0);
    sc.AddNode(m2);

    // utility objects

    AABBWireframe* aabb = new AABBWireframe(m1->GetBounds());
    sc.AddDrawable(aabb);

    TangentsRenderer* tangents = new TangentsRenderer(*geo, 0.05f);
    sc.AddDrawable(tangents);

    // our update function

    sc.AddUpdateFunction([m1, m2, aabb, tangents](float dt) {
        m1->Rotate(dt * 45, glm::normalize(glm::vec3(0.5f, 1.0, 0.0)));
        m2->Rotate(dt * 15, glm::normalize(glm::vec3(0, 1.0, 0.0)));

        aabb->Update(m1->GetBounds());
        tangents->Update(m2->GetTransform(), m2->GetNormalMatrix());
    });
}

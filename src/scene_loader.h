#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <glm/glm.hpp>

#include <scene/SceneController.h>

#include <mesh/Model.h>
#include <mesh/Mesh.h>
#include <mesh/Cube.h>
#include <mesh/Quad.h>
#include <mesh/Sphere.h>
#include <mesh/Torus.h>
#include <mesh/Pyramid.h>
#include <shading/UnlitMaterial.h>
#include <shading/PhongMaterial.h>
#include <lighting/LightNode.h>
#include <lighting/DirectionalLight.h>
#include <lighting/PointLight.h>
#include <lighting/SpotLight.h>

struct SceneLoader {
    static void LoadScene1(SceneController* sc) {
        int w = 10;
        int h = 5;
        int d = 10;
        float spacing = 2;
        float halfW = w / 2.0f;
        float halfH = h / 2.0f;
        float halfD = d / 2.0f;
        for(int x = -floorf(halfW); x < ceilf(halfW); x++) {
            for(int y = -floorf(halfH); y < ceilf(halfH); y++) {
                for(int z = -floorf(halfD); z < ceilf(halfD); z++) {
                    Model* model = new Model(new Mesh(new Cube(), new UnlitMaterial()));
                    model->SetPosition(x * spacing, y * spacing, z * spacing);
                    model->SetScale(0.5f);

                    sc->AddNode(model);
                }
            }
        }
    }

    static void LoadScene2(SceneController* sc) {
        // add the lighting first

        LightNode* dirLight = new LightNode(new DirectionalLight(glm::vec3(1, 1, 0.8f) * 0.0f)); // disabled for now
        dirLight->Rotate(45, 60, 0);

        glm::vec3 pointLight1Color = glm::vec3(0.2f, 0.6f, 1);
        LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color, 10));
        Model* pointLight1Model = new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(pointLight1Color)));
        pointLight1->Translate(3, -1, 4);
        pointLight1Model->Translate(3, -1, 4);

        glm::vec3 pointLight2Color = glm::vec3(1, 0.6f, 0.2f);
        LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color * 10.0f, 10));
        Model* pointLight2Model = new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(pointLight2Color)));
        pointLight2->Translate(-3, -1, 4);
        pointLight2Model->Translate(-3, -1, 4);

        glm::vec3 spotLight1Color = glm::vec3(0.6f, 0.2f, 1.0f);
        LightNode* spotLight1 = new LightNode(new SpotLight(spotLight1Color * 6.0f, 20.0f, 10));
        Model* spotLight1Model = new Model(new Mesh(new Pyramid(0.1f, 0.2f), new UnlitMaterial(spotLight1Color)));
        spotLight1->Translate(4, 1, -5);
        spotLight1Model->Translate(4, 1, -5);
        spotLight1->Rotate(45, 0, 0);
        spotLight1Model->Rotate(-45, 0, 0);

        glm::vec3 spotLight2Color = glm::vec3(1.0f, 0.2f, 0.2f);
        LightNode* spotLight2 = new LightNode(new SpotLight(spotLight2Color * 6.0f, 40.0f, 10));
        Model* spotLight2Model = new Model(new Mesh(new Pyramid(0.15f, 0.2f), new UnlitMaterial(spotLight2Color)));
        spotLight2->Translate(2, 1, -5);
        spotLight2Model->Translate(2, 1, -5);
        spotLight2->Rotate(45, -15, 0);
        spotLight2Model->Rotate(-45, -15, 0);

        sc->AddNode(dirLight);
        sc->AddNode(pointLight1);
        sc->AddNode(pointLight2);
        sc->AddNode(spotLight1);
        sc->AddNode(spotLight2);

        sc->AddNode(pointLight1Model);
        sc->AddNode(pointLight2Model);
        sc->AddNode(spotLight1Model);
        sc->AddNode(spotLight2Model);

        // next add the models

        Model* floor = new Model(new Mesh(new Quad(), new PhongMaterial(
            { 1, 1, 1 }, { 1, 1, 1 }, 64,
            Resources::GetTexture("brick_DIFF.jpg", true),
            nullptr,
            Resources::GetTexture("brick_NRM.jpg", false),
            glm::vec2(4))));
        floor->Translate(0, -2, 0);
        floor->Rotate(-90, 0, 0);
        floor->Scale(10);

        Model* m1 = new Model(new Mesh(new Cube(), new UnlitMaterial({ 1, 0.5, 0 })));
        m1->Translate(4, -1, 0);

        Model* m2 = new Model(new Mesh(new Sphere(), new UnlitMaterial(
            { 0, 0.5, 1 },
            Resources::GetTexture("awesomeface.png", true),
            glm::vec2(2, 1))));
        m2->Translate(2, -1, 0);

        Model* m3 = new Model(new Mesh(new Torus(), new PhongMaterial({ 0.5, 0, 1 }, { 0.5, 0, 1 })));
        m3->Translate(0, -1, 0);
        // m3->Rotate(90, 0, 0);

        Model* m4 = new Model(new Mesh(new Cube(),
            new PhongMaterial(
                { 1, 1, 1 },
                { 1, 1, 1 },
                128,
                Resources::GetTexture("crate_DIFF.png", true),
                Resources::GetTexture("crate_SPEC.png", false))));
        m4->Translate(0, -1, 4);

        sc->AddNode(floor);
        sc->AddNode(m1);
        sc->AddNode(m2);
        sc->AddNode(m3);
        sc->AddNode(m4);

        sc->AddUpdateFunction([m1, m2, m3, m4](float dt) {
            m1->Rotate(dt * 45, glm::normalize(glm::vec3(0.5f, 1.0, 0.0)));
            m2->Rotate(dt * 60, glm::normalize(glm::vec3(0, 1.0, 0.0)));
            m3->Rotate(0, dt * -15, 0);
            m4->Rotate(dt * -60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
        });
    }
};

#endif // SCENE_LOADER_H

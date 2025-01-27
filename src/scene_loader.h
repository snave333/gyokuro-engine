#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <scene/SceneController.h>

#include <mesh/ModelNode.h>
#include <mesh/Model.h>
#include <mesh/Mesh.h>
#include <mesh/Cube.h>
#include <mesh/Quad.h>
#include <mesh/Sphere.h>
#include <mesh/Torus.h>
#include <mesh/Pyramid.h>
#include <drawable/AABBWireframe.h>
#include <drawable/TangentsRenderer.h>
#include <mesh/Skybox.h>
#include <shading/UnlitMaterial.h>
#include <shading/PhongMaterial.h>
#include <shading/GoochMaterial.h>
#include <lighting/LightNode.h>
#include <lighting/DirectionalLight.h>
#include <lighting/PointLight.h>
#include <lighting/SpotLight.h>

struct SceneLoader {
    static void LoadVFCScene(SceneController* sc) {
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
                    ModelNode* model = new ModelNode(new Model(new Mesh(new Cube(), new UnlitMaterial())));
                    model->SetPosition(x * spacing, y * spacing, z * spacing);
                    model->SetScale(0.5f);

                    sc->AddNode(model);
                }
            }
        }
    }

    static void LoadLightingScene(SceneController* sc) {
        // add the lighting first

        LightNode* dirLight = new LightNode(new DirectionalLight(glm::vec3(1, 1, 0.8f) * 0.0f)); // disabled for now
        dirLight->Rotate(45, 60, 0);

        glm::vec3 pointLight1Color = glm::vec3(0.2f, 0.6f, 1);
        LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color, 10));
        ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
        pointLight1->Translate(3, -1, 4);
        pointLight1Model->Translate(3, -1, 4);

        glm::vec3 pointLight2Color = glm::vec3(1, 0.6f, 0.2f);
        LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color * 10.0f, 10));
        ModelNode* pointLight2Model = new ModelNode(new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(glm::vec4(pointLight2Color, 1.0f)))));
        pointLight2->Translate(-3, -1, 4);
        pointLight2Model->Translate(-3, -1, 4);

        glm::vec3 spotLight1Color = glm::vec3(0.6f, 0.2f, 1.0f);
        LightNode* spotLight1 = new LightNode(new SpotLight(spotLight1Color * 6.0f, 20.0f, 10));
        ModelNode* spotLight1Model = new ModelNode(new Model(new Mesh(new Pyramid(0.1f, 0.2f), new UnlitMaterial(glm::vec4(spotLight1Color, 1.0f)))));
        spotLight1->Translate(4, 1, -5);
        spotLight1Model->Translate(4, 1, -5);
        spotLight1->Rotate(45, 0, 0);
        spotLight1Model->Rotate(-45, 0, 0);

        glm::vec3 spotLight2Color = glm::vec3(1.0f, 0.2f, 0.2f);
        LightNode* spotLight2 = new LightNode(new SpotLight(spotLight2Color * 6.0f, 40.0f, 10));
        ModelNode* spotLight2Model = new ModelNode(new Model(new Mesh(new Pyramid(0.15f, 0.2f), new UnlitMaterial(glm::vec4(spotLight2Color, 1.0f)))));
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

        // skybox

        std::vector<const char*> faces {
            "skybox_px.jpg",
            "skybox_nx.jpg",
            "skybox_py.jpg",
            "skybox_ny.jpg",
            "skybox_nz.jpg",
            "skybox_pz.jpg"
        };
        Skybox* skybox = new Skybox(Resources::GetTextureCube(faces, true));
        sc->SetSkybox(skybox);

        // next add the models

        ModelNode* floor = new ModelNode(new Model(new Mesh(new Quad(), new PhongMaterial(
            { 1, 1, 1, 1 }, { 1, 1, 1 }, 64,
            Resources::GetTexture("brick_DIFF.jpg", true),
            nullptr,
            Resources::GetTexture("brick_NRM.jpg", false),
            glm::vec2(4)))));
        floor->Translate(0, -2, 0);
        floor->Rotate(-90, 0, 0);
        floor->Scale(10);

        ModelNode* m1 = new ModelNode(new Model(new Mesh(new Cube(), new UnlitMaterial({ 1, 0.5, 0, 1 }, true))));
        m1->Translate(4, -1, 0);

        ModelNode* m2 = new ModelNode(new Model({
            new Mesh(new Sphere(0.6f), new UnlitMaterial(
                { 0, 0.5, 1, 1 },
                false,
                Resources::GetTexture("awesomeface.png", true),
                glm::vec2(2, 1))),
            new Mesh(new Cube(0.2f), new UnlitMaterial({ 1, 0, 1, 1 }))
        }));
        m2->Translate(2, -1, 0);

        ModelNode* m3 = new ModelNode(new Model({
            new Mesh(new Torus(), new PhongMaterial({ 0.5, 0, 1, 0.8f }, { 0.5, 0, 1 })),
            new Mesh(new Sphere(0.3f), new UnlitMaterial({ 0, 1, 1, 1 })),
        }));
        m3->Translate(0, -1, 0);
        // m3->Rotate(90, 0, 0);

        ModelNode* m4 = new ModelNode(new Model(new Mesh(new Cube(),
            new PhongMaterial(
                { 1, 1, 1, 1 },
                { 1, 1, 1 },
                128,
                Resources::GetTexture("crate_DIFF.jpg", true),
                Resources::GetTexture("crate_SPEC.jpg", false)))));
        m4->Translate(0, -1, 4);

        sc->AddNode(floor);
        sc->AddNode(m1);
        sc->AddNode(m2);
        sc->AddNode(m3);
        sc->AddNode(m4);

        // our update function

        sc->AddUpdateFunction([m1, m2, m3, m4](float dt) {
            m1->Rotate(dt * 45, glm::normalize(glm::vec3(0.5f, 1.0, 0.0)));
            m2->Rotate(dt * 60, glm::normalize(glm::vec3(0, 1.0, 0.0)));
            m3->Rotate(0, dt * -15, 0);
            m4->Rotate(dt * -60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
        });
    }

    static void LoadTransparencyScene(SceneController* sc) {
        // skybox

        std::vector<const char*> faces {
            "skybox_px.jpg",
            "skybox_nx.jpg",
            "skybox_py.jpg",
            "skybox_ny.jpg",
            "skybox_nz.jpg",
            "skybox_pz.jpg"
        };
        Skybox* skybox = new Skybox(Resources::GetTextureCube(faces, true));
        sc->SetSkybox(skybox);

        // transparent objects

        ModelNode* m1 = new ModelNode(new Model(new Mesh(new Quad(), new UnlitMaterial(glm::vec4(1), Resources::GetTexture("window.png", true)))));
        m1->Rotate(180, 0, 0);

        ModelNode* m2 = new ModelNode(new Model(new Mesh(new Quad(), new UnlitMaterial(glm::vec4(1), Resources::GetTexture("window.png", true)))));
        m2->Translate(0.75f, 0.25f, 2);
        m2->Rotate(180, 0, 0);

        ModelNode* m3 = new ModelNode(new Model(new Mesh(new Quad(), new UnlitMaterial(glm::vec4(0, 0.5f, 0.8f, 0.2f)))));
        m3->Translate(0, -0.25f, 3);
        m3->Rotate(180, 0, 0);

        sc->AddNode(m1);
        sc->AddNode(m2);
        sc->AddNode(m3);
    }

    static void LoadDrawablesScene(SceneController* sc) {
        // meshes objects

        ModelNode* m1 = new ModelNode(new Model({
            new Mesh(new Cube(), new UnlitMaterial({ 1, 0.5, 0, 1 })),
            new Mesh(new Sphere(0.65f), new UnlitMaterial({ 0, 1, 0.5, 1 }, true)),
        }));
        m1->Translate(1, 0, 0);

        Geometry* geo = new Sphere();
        ModelNode* m2 = new ModelNode(new Model(new Mesh(geo, new UnlitMaterial({ 0, 0.5, 1, 1 }))));
        m2->Translate(-1, 0, 0);

        sc->AddNode(m1);
        sc->AddNode(m2);

        // utility objects

        AABBWireframe* aabb = new AABBWireframe(m1->GetBounds());

        TangentsRenderer* tangents = new TangentsRenderer(*geo, 0.05f);

        sc->AddDrawable(aabb);
        sc->AddDrawable(tangents);

        // our update function

        sc->AddUpdateFunction([m1, m2, aabb, tangents](float dt) {
            m1->Rotate(dt * 45, glm::normalize(glm::vec3(0.5f, 1.0, 0.0)));
            m2->Rotate(dt * 15, glm::normalize(glm::vec3(0, 1.0, 0.0)));

            aabb->Update(m1->GetBounds());
            tangents->Update(m2->GetTransform(), m2->GetNormalMatrix());
        });
    }

    static void LoadGoochScene(SceneController* sc) {
        // light

        glm::vec3 pointLight1Color = glm::vec3(1, 1, 1) * 4.0f;
        LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color, 10));
        ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
        pointLight1->Translate(-3, 1, -2);
        pointLight1Model->Translate(-3, 1, -2);

        sc->AddNode(pointLight1);

        sc->AddNode(pointLight1Model);

        // models

        ModelNode* m1 = new ModelNode(new Model(new Mesh(new Sphere(), new GoochMaterial())));
        m1->Scale(2);

        sc->AddNode(m1);
    }

    static void LoadModelScene(SceneController* sc) {
        // some lighting

        glm::vec3 pointLight1Color = glm::vec3(0.2f, 0.6f, 1);
        LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color, 10));
        ModelNode* pointLight1Model = new ModelNode(new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(glm::vec4(pointLight1Color, 1.0f)))));
        glm::vec3 p1Position = { 3, -2, -2 };
        pointLight1->Translate(p1Position);
        pointLight1Model->Translate(p1Position);

        glm::vec3 pointLight2Color = glm::vec3(1, 0.6f, 0.2f);
        LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color * 10.0f, 10));
        ModelNode* pointLight2Model = new ModelNode(new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(glm::vec4(pointLight2Color, 1.0f)))));
        glm::vec3 p2Position = { -3, 2, -2 };
        pointLight2->Translate(p2Position);
        pointLight2Model->Translate(p2Position);

        sc->AddNode(pointLight1);
        sc->AddNode(pointLight2);

        sc->AddNode(pointLight1Model);
        sc->AddNode(pointLight2Model);

        // the model we're loading

        ModelNode* helmet = new ModelNode(Resources::GetModel("DamagedHelmet.glb"));
        helmet->Translate(-1, 0, 0);

        ModelNode* dice =   new ModelNode(Resources::GetModel("Dice.fbx"));
        dice->Translate(1, 0, 0);
        dice->Scale(0.5f);

        sc->AddNode(helmet);
        sc->AddNode(dice);

        sc->AddUpdateFunction([helmet, dice](float dt) {
            helmet->Rotate(0, dt * -15, 0);
            dice->Rotate(0, dt * -15, 0);
        });
    }
};

#endif // SCENE_LOADER_H


#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <shading/UnlitMaterial.h>
#include <shading/PhongMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <lighting/DirectionalLight.h>
#include <mesh/Quad.h>
#include <mesh/Sphere.h>
#include <mesh/Cube.h>
#include <mesh/Torus.h>
#include <mesh/Model.h>
#include <camera/FlyCamera.h>
#include <lighting/LightNode.h>
#include <utilities/Clock.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SceneController::SceneController(Renderer* r, const int& width, const int& height) {
    renderer = r;
    size = glm::vec2(width, height);

    // setup our default camera
    camera = new FlyCamera(Camera::PerspectiveCamera(60, width / height));
    // camera = new FlyCamera(Camera::OrthographicCamera(3, width / height));
    camera->Translate(0, 0, 3);

    // spawn an array of models for frustum culling testing
    /*
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
                Model* model = new Model(new Mesh(new Torus(), new UnlitMaterial()));
                model->SetPosition(x * spacing, y * spacing, z * spacing);
                model->SetScale(0.5f);
                models.push_back(model);
            }
        }
    }
    */
    
   // our test models

    Model* floor = new Model(new Mesh(new Quad(), new PhongMaterial()));
    floor->Translate(0, -2, 0);
    floor->Rotate(-90, 0, 0);
    floor->Scale(10);

    Model* m1 = new Model(new Mesh(new Cube(), new UnlitMaterial(glm::vec3(1, 0.5, 0))));
    m1->Translate(-4, -1, 0);

    Model* m2 = new Model(new Mesh(new Sphere(), new UnlitMaterial(glm::vec3(0, 0.5, 1), "wall.jpg")));
    m2->Translate(-2, -1, 0);

    Model* m3 = new Model(new Mesh(new Torus(), new PhongMaterial(glm::vec3(0.5, 0, 1), glm::vec3(0.5, 0, 1))));
    m3->Translate(0, -1, 0);
    m3->Rotate(90, 0, 0);

    models.push_back(floor);
    models.push_back(m1);
    models.push_back(m2);
    models.push_back(m3);

    // our test direct lighting

    dirLight = new LightNode(new DirectionalLight(glm::vec3(1)));
    dirLight->Rotate(45, -60, 0);

    // set the uniform block binding points
    for(const auto& m : models) {
        const Material& material = m->GetMaterial();
        const Shader& shader = material.GetShader();

        shader.Use();
        shader.SetUniformBlockBinding("Camera", 0);

        if(material.usesDirectLighting) {
            // shader.SetUniformBlockBinding("Lights", 1);

            shader.SetVec3("dirLight.direction", dirLight->GetForward());
            shader.SetVec3("dirLight.color", dirLight->GetLight().color);
        }
    }
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    for(const auto& model : models) {
        delete model;
    }
    models.clear();

    delete dirLight;
    dirLight = nullptr;
}

void SceneController::Update(float dt) {
    // model->Translate(dt / 2, 0, 0);
    // model->Translate(glm::vec3(dt / 2, 0, 0));
    // model->Rotate(dt * 30, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    // model->Rotate(0, dt * 60, 0);
    // model->SetScale(glm::sin(glfwGetTime()) + 1);

    // model->SetPosition(glm::sin(glfwGetTime()), 0, 0);
    // model->SetRotation(glm::sin(glfwGetTime()) * 30, 0, 0);
    // model->SetScale(glm::sin(glfwGetTime()) / 2 + 1);
}

void SceneController::Render() {
    if(renderer == nullptr) {
        return;
    }
    
    renderer->BeginFrame(); // set frame buffer, clear

    RenderScene();
    
    renderer->EndFrame(); // swap buffers
}

void SceneController::RenderScene() {
    /**
     * vector<Mesh*> visible
     * vector<Mesh*> opaque
     * vector<Mesh*> transparent
     * 
     * - update view matrix uniform block
     * - frustum culling
     * - separate opaque/transparent
     * - render opaque :: renderer->RenderOpaque(opaque)
     * - skybox pass
     * - sort transparent back to front
     * - render transparent :: renderer->RenderOpaque(opaque)
     */

    std::vector<Model*> visibleModels = {};
    {
        // CLOCK(frustum_culling);

        const Frustum& cameraFrustum = camera->GetFrustum();
        std::array<std::pair<int, int>, 6> frustumLUT = cameraFrustum.ComputeAABBTestLUT();

        for(const auto& model : models) {
            const AABB& bounds = model->GetBounds();
            const std::array<glm::vec3, 8>& boundsLUT = model->GetLUT();

            // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);
            FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT);

            if(result != Outside) {
                visibleModels.push_back(model);
            }
        }
    }

    // early exit if nothing is visible
    if(visibleModels.empty()) {
        return;
    }

    // update the camera view matrix for our shaders
    camera->UpdateViewMatrixUniform();

    // FIXME this should be done as models are added to the scene
    std::vector<Model*> opaqueModels = {};
    std::vector<Model*> transparentModels = {};
    for(const auto& model : visibleModels) {
        if(model->GetRenderType() == Opaque) {
            opaqueModels.push_back(model);
        }
        else {
            transparentModels.push_back(model);
        }
    }

    {
        // CLOCK(render_opaque);

        for(const auto& model : opaqueModels) {
            model->Queue();
            const Material& material = model->GetMaterial();
            const Shader& shader = material.GetShader();

            // set any shader uniforms
            shader.SetMat4("model", model->GetTransform());
            shader.SetMat4("normalMatrix", model->GetNormalMatrix());

            model->Draw();
        }
    }

    // int totalCount = models.size();
    // int drawCount = visibleModels.size();
    // std::cout << "Culled " << (totalCount - drawCount) << " meshes" << std::endl;
}

void SceneController::OnKeyPressed(int key, float dt) {
    float cameraSpeed = 4.0f;
    glm::vec3 velocity(0);

    switch(key) {
        case GLFW_KEY_W:
            velocity += camera->GetForward();
            break;
        case GLFW_KEY_S:
            velocity -= camera->GetForward();
            break;
        case GLFW_KEY_A:
            velocity -= camera->GetRight();
            break;
        case GLFW_KEY_D:
            velocity += camera->GetRight();
            break;
    }

    if(velocity != glm::vec3(0)) {
        camera->Translate(velocity * dt * cameraSpeed);
    }
}

void SceneController::OnMouseMove(float x, float y) {
    // the first mouse move; initialize the position values
    if(lastMouseX == 0 && lastMouseY == 0) {
        lastMouseX = x;
        lastMouseY = y;
    }

    // calculate our mouse delta

    float xOffset = x - lastMouseX;
    float yOffset = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    if(camera != nullptr) {
        camera->OnLook(xOffset, yOffset);
    }
}

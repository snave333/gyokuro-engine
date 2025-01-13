
#include <iostream>
#include <string>
#include <sstream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>
#include <resources/Resources.h>
#include <shading/UnlitMaterial.h>
#include <shading/PhongMaterial.h>
#include <shading/Shader.h>
#include <shading/Texture2D.h>
#include <lighting/DirectionalLight.h>
#include <lighting/PointLight.h>
#include <lighting/SpotLight.h>
#include <mesh/Quad.h>
#include <mesh/Sphere.h>
#include <mesh/Cube.h>
#include <mesh/Torus.h>
#include <mesh/Pyramid.h>
#include <mesh/Model.h>
#include <mesh/AABBWireframe.h>
#include <mesh/TangentsRenderer.h>
#include <camera/FlyCamera.h>
#include <lighting/LightNode.h>
#include <ui/Text.h>
#include <utilities/Clock.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

SceneController::SceneController(Renderer* r, const int& width, const int& height) {
    renderer = r;
    size = glm::ivec2(width, height);

    // setup our default camera
    camera = new FlyCamera(Camera::PerspectiveCamera(60, (float)width / height));
    // camera = new FlyCamera(Camera::OrthographicCamera(5, (float)width / height));
    camera->Translate(0, 0, -3);

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
    {
        Model* floor = new Model(new Mesh(new Quad(), new PhongMaterial(
            { 1, 1, 1 }, { 1, 1, 1 }, 64,
            Resources::GetTexture("brick_DIFF.jpg", true),
            nullptr,
            Resources::GetTexture("brick_NRM.jpg", false))));
        floor->Translate(0, -2, 0);
        floor->Rotate(-90, 0, 0);
        floor->Scale(10);

        Model* m1 = new Model(new Mesh(new Cube(), new UnlitMaterial({ 1, 0.5, 0 })));
        m1->Translate(4, -1, 0);

        Model* m2 = new Model(new Mesh(new Sphere(), new UnlitMaterial({ 0, 0.5, 1 }, Resources::GetTexture("awesomeface.png", true))));
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

        models.push_back(floor);
        models.push_back(m1);
        models.push_back(m2);
        models.push_back(m3);
        models.push_back(m4);

        boundsRenderer = new AABBWireframe(m1->GetBounds());
 
        Geometry geo = Cube();
        tangentsRenderer = new TangentsRenderer(geo, 0.1f);
    }

    // our test direct lighting
    {
        LightNode* dirLight = new LightNode(new DirectionalLight(glm::vec3(1, 1, 0.8f) * 0.0f)); // disabled for now
        dirLight->Rotate(45, 60, 0);

        glm::vec3 pointLight1Color = glm::vec3(0.6f, 0.8f, 1);
        LightNode* pointLight1 = new LightNode(new PointLight(pointLight1Color));
        Model* pointLight1Model = new Model(new Mesh(new Sphere(0.1f), new UnlitMaterial(pointLight1Color)));
        pointLight1->Translate(3, -1, 4);
        pointLight1Model->Translate(3, -1, 4);

        glm::vec3 pointLight2Color = glm::vec3(1, 0.8f, 0.6f) * 10.0f;
        LightNode* pointLight2 = new LightNode(new PointLight(pointLight2Color));
        Model* pointLight2Model = new Model(new Mesh(new Sphere(0.18f), new UnlitMaterial(pointLight2Color)));
        pointLight2->Translate(-3, -1, 4);
        pointLight2Model->Translate(-3, -1, 4);

        glm::vec3 spotLight1Color = glm::vec3(0.8f, 0.4f, 1.0f) * 6.0f;
        LightNode* spotLight1 = new LightNode(new SpotLight(spotLight1Color, 20.0f));
        Model* spotLight1Model = new Model(new Mesh(new Pyramid(0.1f, 0.2f), new UnlitMaterial(spotLight1Color)));
        spotLight1->Translate(4, 1, -5);
        spotLight1Model->Translate(4, 1, -5);
        spotLight1->Rotate(45, 0, 0);
        spotLight1Model->Rotate(-45, 0, 0);

        glm::vec3 spotLight2Color = glm::vec3(0.8f, 0.4f, 1.0f) * 6.0f;
        LightNode* spotLight2 = new LightNode(new SpotLight(spotLight2Color, 40.0f));
        Model* spotLight2Model = new Model(new Mesh(new Pyramid(0.15f, 0.2f), new UnlitMaterial(spotLight2Color)));
        spotLight2->Translate(2, 1, -5);
        spotLight2Model->Translate(2, 1, -5);
        spotLight2->Rotate(45, 0, 0);
        spotLight2Model->Rotate(-45, 0, 0);

        lights.push_back(dirLight);
        lights.push_back(pointLight1);
        lights.push_back(pointLight2);
        lights.push_back(spotLight1);
        lights.push_back(spotLight2);

        models.push_back(pointLight1Model);
        models.push_back(pointLight2Model);
        models.push_back(spotLight1Model);
        models.push_back(spotLight2Model);
    }
    
    // set the uniform block binding points
    for(const auto& m : models) {
        const Material& material = m->GetMaterial();
        const Shader& shader = material.GetShader();

        shader.Use();
        shader.SetUniformBlockBinding("Camera", 0);

        if(material.usesDirectLighting) {
            // shader.SetUniformBlockBinding("Lights", 1);

            shader.SetVec3("globalAmbient", ambientLight);

            int numPointLights = 0;
            int numSpotLights = 0;
            for (size_t i = 0; i < lights.size(); ++i) {
                const DirectionalLight* directionalLight = dynamic_cast<const DirectionalLight*>(lights[i]->GetLight());
                const PointLight* pointLight = dynamic_cast<const PointLight*>(lights[i]->GetLight());
                const SpotLight* spotLight = dynamic_cast<const SpotLight*>(lights[i]->GetLight());

                if (pointLight) {
                    std::string baseName = "pointLights[" + std::to_string(numPointLights) + "]";
                    numPointLights++;

                    shader.SetVec3((baseName + ".position").c_str(), lights[i]->GetPosition());
                    shader.SetVec3((baseName + ".color").c_str(), pointLight->color);
                    shader.SetFloat((baseName + ".constant").c_str(), pointLight->constant);
                    shader.SetFloat((baseName + ".linear").c_str(), pointLight->linear);
                    shader.SetFloat((baseName + ".quadratic").c_str(), pointLight->quadratic);
                    
                }
                else if (spotLight) {
                    std::string baseName = "spotLights[" + std::to_string(numSpotLights) + "]";
                    numSpotLights++;

                    shader.SetVec3((baseName + ".position").c_str(), lights[i]->GetPosition());
                    shader.SetVec3((baseName + ".direction").c_str(), lights[i]->GetForward());
                    shader.SetVec3((baseName + ".color").c_str(), spotLight->color);
                    shader.SetFloat((baseName + ".cosAngle").c_str(), spotLight->cosAngle);
                    shader.SetFloat((baseName + ".constant").c_str(), spotLight->constant);
                    shader.SetFloat((baseName + ".linear").c_str(), spotLight->linear);
                    shader.SetFloat((baseName + ".quadratic").c_str(), spotLight->quadratic);
                }
                else {
                    shader.SetVec3("dirLight.direction", lights[i]->GetForward());
                    shader.SetVec3("dirLight.color", directionalLight->color);
                }
            }

            // shader.SetFloat("numPointLights", numPointLights);
            // shader.SetFloat("numSpotLights", numSpotLights);
        }
    }

    // our ui layer
    
    textRenderer = new Text("SourceCodePro-Regular.ttf", size, 14);
}

SceneController::~SceneController() {
    renderer = nullptr;

    delete camera;
    camera = nullptr;

    for(const auto& model : models) {
        delete model;
    }
    models.clear();

    delete boundsRenderer;
    boundsRenderer = nullptr;
    delete tangentsRenderer;
    tangentsRenderer = nullptr;

    for(const auto& light : lights) {
        delete light;
    }
    lights.clear();

    delete textRenderer;
    textRenderer = nullptr;
}

void SceneController::Update(float dt) {
    // for(const auto& m : models) {
    //     m->Rotate(dt * 30, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    // }

    // model->Translate(dt / 2, 0, 0);
    // model->Translate(glm::vec3(dt / 2, 0, 0));
    models[1]->Rotate(dt * 45, glm::normalize(glm::vec3(0.5f, 1.0, 0.0)));
    models[2]->Rotate(dt * 60, glm::normalize(glm::vec3(0, 1.0, 0.0)));
    models[3]->Rotate(0, dt * 15, 0);
    models[4]->Rotate(dt * -60, glm::normalize(glm::vec3(0.5, 1.0, 0.0)));
    // model->SetScale(glm::sin(glfwGetTime()) + 1);

    // model->SetPosition(glm::sin(glfwGetTime()), 0, 0);
    // model->SetRotation(glm::sin(glfwGetTime()) * 30, 0, 0);
    // model->SetScale(glm::sin(glfwGetTime()) / 2 + 1);

    boundsRenderer->Update(models[1]->GetBounds());
    tangentsRenderer->Update(models[1]->GetTransform(), models[1]->GetNormalMatrix());
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
    FrameStats stats;
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

    // frustum culling
    {
        CLOCKT(frustum_culling, &stats.vfcMs);

        const Frustum& cameraFrustum = camera->GetFrustum();
        std::array<std::pair<int, int>, 6> frustumLUT = cameraFrustum.ComputeAABBTestLUT();

        for(const auto& model : models) {
            const AABB& bounds = model->GetBounds();
            const std::array<glm::vec3, 8>& boundsLUT = model->GetLUT();

            // FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds);
            FrustumTestResult result = cameraFrustum.TestAABBIntersection(bounds, boundsLUT, frustumLUT);

            if(result != OUTSIDE) {
                visibleModels.push_back(model);
            }
        }
    }

    // update the camera view matrix for our shaders
    camera->UpdateViewMatrixUniform();

    // FIXME this should be done as models are added to the scene
    std::vector<Model*> opaqueModels = {};
    std::vector<Model*> transparentModels = {};
    for(const auto& model : visibleModels) {
        if(model->GetRenderType() == OPAQUE) {
            opaqueModels.push_back(model);
        }
        else {
            transparentModels.push_back(model);
        }
    }

    // opaque pass
    {
        CLOCKT(render_opaque, &stats.opaqueMs);

        for(const auto& model : opaqueModels) {
            model->Queue();
            const Material& material = model->GetMaterial();
            const Shader& shader = material.GetShader();

            // set any shader uniforms
            shader.SetMat4("model", model->GetTransform());
            shader.SetMat4("normalMatrix", model->GetNormalMatrix());

            model->Draw();
            ++stats.drawCalls;
        }

        // NOTE this should be optimized to only draw if the associated model is visible
        if(!opaqueModels.empty()) {
            boundsRenderer->Draw();
            tangentsRenderer->Draw();
        }
    }

    {
        // TODO this should come AFTER the transparency pass and any image effects
        // CLOCK(render_ui);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // number of draw calls

        textRenderer->RenderText(std::string("draw calls: ") + std::to_string(stats.drawCalls), 10, 50, 1.0f, glm::vec3(1));

        // view frustum culling

        std::ostringstream stream;
        stream.precision(2);
        stream << std::fixed << stats.vfcMs;

        std::string vfcMs = stream.str();
        textRenderer->RenderText(std::string("vfc: ") + vfcMs + std::string(" ms"), 10, 30, 1.0f, glm::vec3(1));

        // opaque pass

        stream.str("");
        stream.clear();
        stream << std::fixed << stats.opaqueMs;

        std::string opaqueMs = stream.str();
        textRenderer->RenderText(std::string("opaque pass: ") + opaqueMs + std::string(" ms"), 10, 10, 1.0f, glm::vec3(1));

        glDisable(GL_BLEND);
    }
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
            velocity += camera->GetRight();
            break;
        case GLFW_KEY_D:
            velocity -= camera->GetRight();
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

    float xOffset = lastMouseX - x;
    float yOffset = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;

    if(camera != nullptr) {
        camera->OnLook(xOffset, yOffset);
    }
}

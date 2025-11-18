
/**
 * Lights uniform buffer object, found in lights.glsl, has the following signature:
 * layout (std140) uniform Lights {
 *      vec4 globalAmbient;                         // .rgb: ambient light color, .a: 0 [unused]
 *      DirectionalLight dirLight;                  // 32 bytes
 *      PointLight pointLights[MAX_POINT_LIGHTS];   // 128 bytes (32 * 4)
 *      SpotLight spotLights[MAX_SPOT_LIGHTS];      // 256 bytes (64 * 4)
 *      int numPointLights;                         // 4 bytes
 *      int numSpotLights;                          // 4 bytes
 *      // 8 bytes padding
 * }; // total size with std140 layout: 448 bytes
 */

#include <gyo/lighting/LightsUBO.h>
#include <gyo/lighting/LightNode.h>
#include <gyo/lighting/DirectionalLight.h>
#include <gyo/lighting/PointLight.h>
#include <gyo/lighting/SpotLight.h>
#include <gyo/scene/SceneController.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <gyo/utilities/GetError.h>

namespace gyo {

LightsUBO::LightsUBO() {
    // create our lights uniform buffer object, and bind for initialization
    glGenBuffers(1, &uboLights);
    glCheckError();
    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glCheckError();

    // allocate enough memory for all of the light uniform values
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, NULL, GL_STATIC_DRAW);
    glCheckError();

    // link the range of the entire buffer to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, bufferSize);
    glCheckError();

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCheckError();
}

LightsUBO::~LightsUBO() {
    glDeleteBuffers(1, &uboLights);
    glCheckError();
}

void LightsUBO::UpdateValues(glm::vec3 ambient, std::vector<LightNode*> lights) {
    // separate all of our lights into their respective types

    const DirectionalLight* directionalLight = nullptr;
    glm::vec3 directionalLightDirection = glm::vec3(0, 0, 1);

    std::vector<const PointLight*> pointLights;
    std::vector<glm::vec3> pointLightPositions;
    
    std::vector<const SpotLight*> spotLights;
    std::vector<glm::vec3> spotLightPositions;
    std::vector<glm::vec3> spotLightDirections;

    for (size_t i = 0; i < lights.size(); ++i) {
        const DirectionalLight* dLight = dynamic_cast<const DirectionalLight*>(lights[i]->GetLight());
        const PointLight* pLight = dynamic_cast<const PointLight*>(lights[i]->GetLight());
        const SpotLight* sLight = dynamic_cast<const SpotLight*>(lights[i]->GetLight());

        if(pLight && pointLights.size() < SceneController::MAX_POINT_LIGHTS) {
            pointLights.push_back(pLight);
            pointLightPositions.push_back(lights[i]->GetPosition());
        }
        else if(sLight && spotLights.size() < SceneController::MAX_SPOT_LIGHTS) {
            spotLights.push_back(sLight);
            spotLightPositions.push_back(lights[i]->GetPosition());
            spotLightDirections.push_back(lights[i]->GetForward());
        }
        // note this only takes the first directional light
        else if(dLight && !directionalLight) {
            directionalLight = dLight;
            directionalLightDirection = lights[i]->GetForward();
        }
    }

    int numPointLights = pointLights.size();
    int numSpotLights = spotLights.size();

    // create our cpu-side buffer for mapping directly to gpu memory

    std::vector<uint8_t> buffer(bufferSize);
    size_t offset = 0L;

    auto write = [&](const void* data, size_t size) {
        memcpy(buffer.data() + offset, data, size);
        offset += size;
    };
    auto setOffset = [&](size_t size) {
        offset = size;
    };

    write(glm::value_ptr(glm::vec4(ambient, 0)), sizeof(glm::vec4));

    /**
        struct DirectionalLight {
            vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
            vec4 color;                                 // .rgb: light color, .a: 0 [unused]
        }; // total size with std140 layout: 32 bytes

     */
    write(glm::value_ptr(glm::vec4(directionalLightDirection, 0)), sizeof(glm::vec4));
    write(
        glm::value_ptr(directionalLight ? glm::vec4(directionalLight->color, 0) : glm::vec4(0)),
        sizeof(glm::vec4)
    );

    /**
        struct PointLight {
            vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
            vec4 color;                                 // .rgb: light color, .a: 0 [unused]
        }; // total size with std140 layout: 32 bytes
     */
    for(int i = 0; i < numPointLights; i++) {
        const PointLight* light = pointLights[i];
        glm::vec3 position = pointLightPositions[i];

        write(glm::value_ptr(glm::vec4(position, 0)), sizeof(glm::vec4));
        write(glm::value_ptr(glm::vec4(light->color, 0)), sizeof(glm::vec4));
    }
    setOffset(176UL); // 16 + 32 + 128

    /**
        struct SpotLight {
            vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
            vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
            vec4 color;                                 // .rgb: light color, .a: 0 [unused]
            float cosAngle;                             // 4 bytes
            // 12 bytes padding
        }; // total size with std140 layout: 64 bytes
     */
    for(int i = 0; i < numSpotLights; i++) {
        const SpotLight* light = spotLights[i];
        glm::vec3 position = spotLightPositions[i];
        glm::vec3 direction = spotLightDirections[i];

        write(glm::value_ptr(glm::vec4(position, 0)), sizeof(glm::vec4));
        write(glm::value_ptr(glm::vec4(direction, 0)), sizeof(glm::vec4));
        write(glm::value_ptr(glm::vec4(light->color, 0)), sizeof(glm::vec4));
        write(&light->cosAngle, sizeof(float) + 12); // 4, plus 12 bytes padding
    }
    setOffset(432UL); // 16 + 32 + 128 + 256

    // counters (ints are 8 byte aligned)

    write(&numPointLights, sizeof(int));
    write(&numSpotLights, sizeof(int));

    // copy the data to the gpu

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glCheckError();

    void* gpuPtr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, buffer.size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    glCheckError();
    if (gpuPtr) {
        memcpy(gpuPtr, buffer.data(), buffer.size());
        glUnmapBuffer(GL_UNIFORM_BUFFER);
        glCheckError();
    }

    std::cout << "Finished update Lights UBO with offset " << offset << std::endl;

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCheckError();
}

} // namespace gyo

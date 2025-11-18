
/**
 * Lights uniform buffer object, found in lights.glsl, has the following signature:
 * layout (std140) uniform Lights {
 *      vec4 globalAmbient;                         // .rgb: ambient light color, .a: 0 [unused]
 *      DirectionalLight dirLight;                  // 32 bytes
 *      PointLight pointLights[MAX_POINT_LIGHTS];   // 128 bytes (32 * 4)
 *      SpotLight spotLights[MAX_SPOT_LIGHTS];      // 256 bytes (64 * 4)
 *      int numPointLights;                         // 4 bytes
 *      // 12 bytes padding
 *      int numSpotLights;                          // 4 bytes
 *      // 12 bytes padding
 * }; // total size with std140 layout: 464 bytes
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

    // the size of our ubo
    unsigned long size = 448;
    
    // allocate enough memory for all of the light uniform values
    glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
    glCheckError();

    // link the range of the entire buffer to binding point 0
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, size);
    glCheckError();

    // store the first part of the uniform buffer with the projection matrix
    // glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));

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
    glm::vec3 directionalLightDirection;

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

    // use sub data to set all of the values

    glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
    glCheckError();
    unsigned int offset = 0;

    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(ambient, 0)));
    glCheckError();
    offset += 16;

    /**
        struct DirectionalLight {
            vec4 direction;                             // .xyz: normalized light direction, .w: 0 [unused]
            vec4 color;                                 // .rgb: light color, .a: 0 [unused]
        }; // total size with std140 layout: 32 bytes

     */
    if(directionalLight != nullptr) {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(directionalLightDirection, 0)));
        glCheckError();
        offset += 16;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(directionalLight->color, 0)));
        glCheckError();
        offset += 16;
    }
    else {
        offset += 32;
    }

    /**
        struct PointLight {
            vec4 position;                              // .xyz: light world-space position, .w: 0 [unused]
            vec4 color;                                 // .rgb: light color, .a: 0 [unused]
        }; // total size with std140 layout: 32 bytes
     */
    for(int i = 0; i < numPointLights; i++) {
        const PointLight* light = pointLights[i];
        glm::vec3 position = pointLightPositions[i];

        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(position, 0)));
        glCheckError();
        offset += 16;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(light->color, 0)));
        glCheckError();
        offset += 16;
    }
    offset = 176; // 16 + 32 + 128

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

        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(position, 0)));
        glCheckError();
        offset += 16;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(direction, 0)));
        glCheckError();
        offset += 16;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(glm::vec4(light->color, 0)));
        glCheckError();
        offset += 16;
        glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &light->cosAngle);
        glCheckError();
        offset += 4 + 12; // 4, plus 12 bytes padding
    }
    offset = 432; // 16 + 32 + 128 + 256

    // counters (ints are 8 byte aligned)

    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &numPointLights);
    glCheckError();
    offset += 4;
    glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(int), &numSpotLights);
    glCheckError();
    offset += 4;

    std::cout << "Finished update Lights UBO with offset " << offset << std::endl;

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glCheckError();
}

} // namespace gyo

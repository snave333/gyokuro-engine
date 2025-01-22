#ifndef LIGHTS_UBO_H
#define LIGHTS_UBO_H

#include <vector>
#include <glm/glm.hpp>

class LightNode;

class LightsUBO {
public:
    LightsUBO();
    ~LightsUBO();

    void UpdateValues(glm::vec3 ambient, std::vector<LightNode*> lights);
private:
    unsigned int uboLights;
};

#endif // LIGHTS_UBO_H

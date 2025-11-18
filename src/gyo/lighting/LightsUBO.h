#ifndef LIGHTS_UBO_H
#define LIGHTS_UBO_H

#include <vector>
#include <glm/glm.hpp>

namespace gyo {

class LightNode;

class LightsUBO {
public:
    LightsUBO();
    ~LightsUBO();

    void UpdateValues(glm::vec3 ambient, std::vector<LightNode*> lights);
private:
    // the byte size of our ubo
    const signed long int bufferSize = 448L;

    unsigned int uboLights;
};

} // namespace gyo

#endif // LIGHTS_UBO_H


#include <lighting/LightNode.h>

namespace gyo {

LightNode::LightNode(Light* light) : light(light) {
}

LightNode::~LightNode() {
    delete light;
    light = nullptr;
}

} // namespace gyo

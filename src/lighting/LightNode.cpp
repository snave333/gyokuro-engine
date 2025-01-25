
#include <lighting/LightNode.h>

LightNode::LightNode(Light* light) : light(light) {
}

LightNode::~LightNode() {
    delete light;
    light = nullptr;
}

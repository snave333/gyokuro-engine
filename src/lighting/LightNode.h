#ifndef LIGHT_NODE_H
#define LIGHT_NODE_H

#include <scene/SceneNode.h>
#include <lighting/Light.h>

class LightNode : public SceneNode {
public:
    LightNode(Light* light);
    ~LightNode();

    const Light* GetLight() { return light; }

private:
    Light* light;
};

#endif // LIGHT_NODE_H

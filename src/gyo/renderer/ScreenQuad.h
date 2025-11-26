#ifndef SCREEN_QUAD_H
#define SCREEN_QUAD_H

namespace gyo {

// class Shader;
class Mesh;

class ScreenQuad {
public:
    // constructor
    ScreenQuad();
    ~ScreenQuad();

    void Draw(unsigned int textureColorbuffer);

private:
    Mesh* mesh;
};

} // namespace gyo

#endif // SCREEN_QUAD_H

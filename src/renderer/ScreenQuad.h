#ifndef SCREEN_QUAD_H
#define SCREEN_QUAD_H

class Shader;

class ScreenQuad {
public:
    // constructor
    ScreenQuad();
    ~ScreenQuad();

    void Draw(unsigned int textureColorbuffer);

private:
    // render data
    unsigned int VAO;
    unsigned int VBO;

    Shader* shader = nullptr;
};

#endif // SCREEN_QUAD_H
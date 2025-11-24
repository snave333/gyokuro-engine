#ifndef SHADER_SEMANTICS_H
#define SHADER_SEMANTICS_H

namespace gyo {

#define SEMANTIC_POSITION 0x001
#define SEMANTIC_NORMAL 0x002
#define SEMANTIC_TEXCOORD0 0x003
#define SEMANTIC_TANGENT 0x004
#define SEMANTIC_COLOR 0x005

static const std::unordered_map<unsigned int, GLenum> SEMANTIC_TO_GLTYPE = {
    { SEMANTIC_POSITION,  GL_FLOAT_VEC3 },
    { SEMANTIC_NORMAL,    GL_FLOAT_VEC3 },
    { SEMANTIC_TEXCOORD0, GL_FLOAT_VEC2 },
    { SEMANTIC_TANGENT,   GL_FLOAT_VEC3 },
    { SEMANTIC_COLOR,     GL_FLOAT_VEC4 }
};
  
static const std::unordered_map<unsigned int, std::pair<GLint, unsigned long>> SEMANTIC_TO_GLSIZE = {
    { SEMANTIC_POSITION,  { 3, sizeof(float) }},
    { SEMANTIC_NORMAL,    { 3, sizeof(float) }},
    { SEMANTIC_TEXCOORD0, { 2, sizeof(float) }},
    { SEMANTIC_TANGENT,   { 3, sizeof(float) }},
    { SEMANTIC_COLOR,     { 4, sizeof(float) }}
};
  
} // namespace gyo

#endif // SHADER_SEMANTICS_H

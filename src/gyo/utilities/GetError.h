#ifndef GET_ERROR_H
#define GET_ERROR_H

#include <glad/glad.h>

namespace gyo {

#define glCheckError() glCheckError_(__FILE__, __LINE__)

GLenum glCheckError_(const char *file, int line);

} // namespace gyo

#endif // GET_ERROR_H

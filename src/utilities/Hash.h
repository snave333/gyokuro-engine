#ifndef HASH_H
#define HASH_H

/**
 * Hashing a string into an unsigned integer for quicker comparisons than a
 * string. Borrowed from Jason Gregory's book "Game Engine Architecture", and
 * Joey DeVries from learnopengl.com
 */

#include <string>

namespace gyo {

#define HASH(string) simple_hash(string)

// djb2 hashing algorithm
inline unsigned int simple_hash(const std::string& str) {
    unsigned long hash = 5381;
    for(char c : str) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

// supports c string literals
inline unsigned int simple_hash(const char* cStr) {
    std::string str(cStr);
    return simple_hash(str);
}

} // namespace gyo

#endif // HASH_H

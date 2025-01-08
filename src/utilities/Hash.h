#ifndef HASH_H
#define HASH_H

/**
 * Hashing a string into an unsigned integer for quicker comparisons than a
 * string. Borrowed from Jason Gregory's book "Game Engine Architecture", and
 * Joey DeVries from learnopengl.com
 */

#include <string>

#define HASH(string) simple_hash(string)

inline unsigned int simple_hash(const std::string& str) {
    unsigned int hash = 0;

	for (auto& it : str) {
		// be sure to use prime numbers
		hash = 37 * hash + 17 * static_cast<char>(it);
	}

    // hash value will automatically wrap around 
    return hash;
}

// supports c string literals
inline unsigned int simple_hash(const char* cStr) {
    std::string str(cStr);
    return simple_hash(str);
}

#endif // HASH_H

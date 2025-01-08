
#include <resources/TextureLoader.h>
#include <shading/Texture2D.h>

#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

Texture2D TextureLoader::LoadTexture(std::string imageFilePath) {
    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // flip vertically
    stbi_set_flip_vertically_on_load(true);
    
    // load and generate the texture
    int width, height, numChannels;
    unsigned char *data = stbi_load(imageFilePath.c_str(), &width, &height, &numChannels, 0);
    if (data) {
        unsigned int format;
        if(numChannels == 1) {
            format = GL_RED;
        }
        else if (numChannels == 3) {
            format = GL_RGB;
        }
        else if (numChannels == 4) {
            format = GL_RGBA;
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free the image memory
    stbi_image_free(data);

    return Texture2D(id);
}

// Texture2D TextureLoader::GenerateTexture() {}

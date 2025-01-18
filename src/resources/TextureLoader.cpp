
#include <resources/TextureLoader.h>
#include <shading/Texture2D.h>
#include <shading/TextureCube.h>

#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

Texture2D TextureLoader::LoadTexture(std::string imageFilePath, bool srgb) {
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
        unsigned int internalFormat;
        GetTextureFormat(srgb, numChannels, &format, &internalFormat);
        
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free the image memory
    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture2D(id);
}

TextureCube TextureLoader::LoadTextureCube(std::vector<std::string> faceFilePaths, bool srgb) {
    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    int width, height, numChannels;
    for(unsigned int i = 0; i < faceFilePaths.size(); i++) {
        unsigned char* data = stbi_load(faceFilePaths[i].c_str(), &width, &height, &numChannels, 0);
        if(data) {
            unsigned int format;
            unsigned int internalFormat;
            GetTextureFormat(srgb, numChannels, &format, &internalFormat);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else {
            std::cout << "Failed to load cubemap at path " << faceFilePaths[i] << std::endl;
        }

        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return TextureCube(id);
}

Texture2D TextureLoader::GenerateTexture2D(int width, int height, unsigned int format, const unsigned char* pixels) {
    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    return Texture2D(id);
}

void TextureLoader::GetTextureFormat(const bool& srgb, const int& numChannels, unsigned int* format, unsigned int* internalFormat) {
    if(numChannels == 1) {
        *format = GL_RED;
        *internalFormat = GL_RED;
    }
    else if (numChannels == 3) {
        *format = GL_RGB;
        *internalFormat = srgb ? GL_SRGB : GL_RGB;
    }
    else if (numChannels == 4) {
        *format = GL_RGBA;
        *internalFormat = srgb ? GL_SRGB_ALPHA : GL_RGBA;
    }
}

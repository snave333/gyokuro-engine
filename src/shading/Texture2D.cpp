
#include <shading/Texture2D.h>
#include <resources/Resources.h>

#include <glad/glad.h>
#include <stb_image.h>

#include <iostream>

Texture2D::Texture2D(const char* imageFileName, bool transparency) {
    // get our full path to the texture file
    std::string fullImagePath = Resources::GetTexturePath(imageFileName);

    // create and bind the texture object
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // flip vertically
    stbi_set_flip_vertically_on_load(true);
    
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(fullImagePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        unsigned int format = transparency ? GL_RGBA : GL_RGB;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }

    // free the image memory
    stbi_image_free(data);
}

Texture2D::~Texture2D() {
    glDeleteTextures(1, &ID);
}

void Texture2D::Bind(unsigned int textureUnit) {
    glActiveTexture(GL_TEXTURE0 + textureUnit); // activate texture unit 0 before binding
    glBindTexture(GL_TEXTURE_2D, ID);
}

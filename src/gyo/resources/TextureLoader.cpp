
#include <gyo/resources/TextureLoader.h>
#include <gyo/shading/Texture2D.h>
#include <gyo/shading/TextureCube.h>
#include <gyo/utilities/FileSystem.h>
#include <gyo/utilities/GetError.h>
#include <gyo/utilities/Log.h>

#include <algorithm>

#include <stb/stb_image.h>
#include <assimp/texture.h>
#include <jpeglib.h>

namespace gyo {

std::string TextureLoader::ResourceDir = "";

Texture2D TextureLoader::LoadTexture(const char* imageFileName, bool srgb, int wrapMode, bool useMipmaps) {
    // get the full file path
    std::string imageFilePath = FileSystem::CombinePath(ResourceDir, imageFileName);

    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();
    
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
        glCheckError();
        if (useMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
            glCheckError();
        }
    }
    else {
        throw std::runtime_error("Failed to load texture");
    }

    // free the image memory
    stbi_image_free(data);

    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, useMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    return Texture2D(id, width, height, numChannels == 4);
}

Texture2D* TextureLoader::LoadEmbeddedTexture(const aiTexture* texture, bool srgb) {
    int width, height, numChannels;
    unsigned char* imageData = nullptr;

    // check format of the embedded texture
    std::string formatHint(texture->achFormatHint);

    // compressed data
    // https://assimp-docs.readthedocs.io/en/latest/usage/use_the_lib.html#textures
    if(texture->mHeight == 0) {
        if(formatHint == "jpg") {
            // texture->mWidth is the size of the raw data
            TextureLoader::DecompressJpegData(
                texture->pcData, texture->mWidth, &width, &height, &numChannels, &imageData);
        }
        // else if(formatHint == "png") {} // TODO look into using libpng
        else {
            LOGE("Unsupported embedded texture compression format %s", formatHint.c_str());
            return nullptr;
        }
    }
    // uncompressed image data
    else {
        // TODO parse format from hint - e.g. rgba8888, argb8888, rgba5650, etc.
        LOGE("Unsupported embedded texture format %s", formatHint.c_str());
        return nullptr;
    }

    if(!imageData) {
        LOGE("Failed to extract image data from aiTexture");
        return nullptr;
    }

    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();
    
    unsigned int format;
    unsigned int internalFormat;
    GetTextureFormat(srgb, numChannels, &format, &internalFormat);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, imageData);
    glCheckError();
    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckError();

    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    // free the image data after uploading it to the GPU
    free(imageData);

    return new Texture2D(id, width, height, numChannels == 4);
}

Texture2D TextureLoader::LoadHDRTexture(const char* imageFileName) {
    // get the full file path
    std::string imageFilePath = FileSystem::CombinePath(ResourceDir, imageFileName);

    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();
    
    // flip vertically
    stbi_set_flip_vertically_on_load(true);
    
    // load and generate the texture
    int width, height, numChannels;
    float *data = stbi_loadf(imageFilePath.c_str(), &width, &height, &numChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);
        glCheckError();
    }
    else {
        throw std::runtime_error("Failed to load texture");
    }

    // free the image memory
    stbi_image_free(data);

    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    return Texture2D(id, width, height, false);
}

TextureCube TextureLoader::LoadTextureCube(std::vector<const char*> faceFileNames, bool srgb) {
    // get the full file paths
    std::vector<std::string> faceFilePaths(6);
    std::transform(faceFileNames.begin(), faceFileNames.end(), faceFilePaths.begin(), [](const char* fileName) {
        return FileSystem::CombinePath(ResourceDir, fileName);
    });

    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glCheckError();

    // flip vertically
    stbi_set_flip_vertically_on_load(false);
    
    // load and generate the textures
    int width, height, numChannels;
    for(unsigned int i = 0; i < faceFilePaths.size(); i++) {
        unsigned char* data = stbi_load(faceFilePaths[i].c_str(), &width, &height, &numChannels, 0);
        if(data) {
            unsigned int format;
            unsigned int internalFormat;
            GetTextureFormat(srgb, numChannels, &format, &internalFormat);

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glCheckError();
        }
        else {
            LOGE("Failed to load cubemap at path '%s'", faceFilePaths[i].c_str());
        }

        stbi_image_free(data);
    }

    // set the texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glCheckError();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glCheckError();

    return TextureCube(id, width, height);
}

Texture2D TextureLoader::GenerateTexture2D(int width, int height, unsigned int format, const unsigned char* pixels) {
    // create and bind the texture object
    unsigned int id;
    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();

    // generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
    glCheckError();

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glCheckError();

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

    bool hasAlpha = format == GL_RGBA || format == GL_SRGB_ALPHA;

    return Texture2D(id, width, height, hasAlpha);
}

void TextureLoader::DecompressJpegData(
    const aiTexel* pcData, const unsigned int& pcDataSize,
    int* width, int* height, int* numChannels,
    unsigned char** imageData
) {
    // use libjpeg to decode the JPEG data
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, (unsigned char*)pcData, pcDataSize); 

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height = cinfo.output_height;
    *numChannels = cinfo.output_components; // typically 3 for RGB

    LOGD("Decompressing %dx%d JPEG image with %d channels", *width, *height, *numChannels);

    *imageData = (unsigned char*)malloc(*width * *height * *numChannels);
    if (!*imageData) {
        jpeg_destroy_decompress(&cinfo);
        LOGE("Failed to allocate memory for JPEG image");
        return;
    }

    // decompress the JPEG data into the imageData buffer
    while (cinfo.output_scanline < cinfo.output_height) {
        unsigned char* rowPtr[1];
        rowPtr[0] = *imageData + cinfo.output_scanline * *width * *numChannels;
        jpeg_read_scanlines(&cinfo, rowPtr, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
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

} // namespace gyo

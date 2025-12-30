# include <textures.h>
# include <stdio.h>

# define STB_IMAGE_IMPLEMENTATION
# include <stb_image.h>

unsigned char *load_image(const char *imagePath, int *width, int *height, int *channels){
    void *ptr = stbi_load(imagePath, width, height, channels, 0);
    unsigned char *data;
    if(!ptr){
        fprintf(stderr, "stbi_load() failed!\n");
        return NULL;
    }

    data = ptr;
    return data;
}

GLuint create_2Dtexture(const unsigned char *data, int width, int height, GLenum format, GLenum textureSlot){
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(textureSlot);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

TextureData load_texture(const char *imagePath, GLenum format, GLenum textureSlot){
    TextureData tex;
    int w, h, c;
    unsigned char *data = load_image(imagePath, &w, &h, &c);
    if(!data){
        tex.data=NULL; tex.texture=-1; tex.width=0; tex.height=0; tex.channels=0;
        return tex;
    }

    GLuint texture = create_2Dtexture(data, w, h, format, textureSlot);

    tex.width = w; tex.height = h; tex.channels = c; tex.texture = texture;
    tex.data = data;
    return tex;
}

void destroy_textures(TextureData texture){
    stbi_image_free(texture.data);
    glDeleteTextures(1, &texture.texture);
}

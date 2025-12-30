# ifndef TEXTURES_H
# define TEXTURES_H

# include <glad/glad.h>

typedef struct TextureData{
    unsigned char *data;
    GLuint texture;
    int width; int height; int channels;
} TextureData;

unsigned char *load_image(const char*, int*, int*, int*);
GLuint create_2Dtexture(const unsigned char*, int, int, GLenum, GLenum);
void destroy_textures(TextureData);
TextureData load_texture(const char*, GLenum, GLenum);

# endif

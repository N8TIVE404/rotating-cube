# ifndef SHADERS_H
# define SHADERS_H

# include <glad/glad.h>
# include <stdlib.h>

typedef struct ShaderText{
    void *data;
    size_t size;
} ShaderText;

ShaderText load_shader(const char*);
GLuint shader_program(const char*, const char*);

# endif

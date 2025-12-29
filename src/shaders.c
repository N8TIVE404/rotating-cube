# include <shaders.h>
# include <string.h>
# include <stdio.h>

ShaderText load_shader(const char *filePath){
    ShaderText text  = {.data = NULL, .size = 0};
    
    // Open the file and get the size of the file
    FILE *file = fopen(filePath, "rb");
    if(!file){
        fprintf(stderr, "fopen() failed in load_shader().\n");
        return text;
    }

    if(fseek(file, 0, SEEK_END)){
        fprintf(stderr, "fseek() failed in load_shader().\n");
        fclose(file);
        return text;
    }

    long size = ftell(file);

    if(fseek(file, 0, SEEK_SET)){
        fprintf(stderr, "fseek() failed in load_shader().\n");
        fclose(file);
        return text;
    }

    // Allocate memory and read the contents of the file
    unsigned char *ptr = malloc((size_t)size + 1);
    if(!ptr){
        fprintf(stderr, "malloc() failed in load_shader().\n");
        fclose(file);
        return text;
    }

    size_t read = fread(ptr, sizeof(char), size, file);
    if(!read || read != (size_t)size){
        fprintf(stderr, "fread() failed in load_shader().\n");
        free(ptr);
        fclose(file);
        return text;
    }
    ptr[size] = '\0';

    text.data = ptr;
    text.size = size;
    fclose(file);

    return text;
}

GLuint shader_program(const char *vertexPath, const char *fragmentPath){
    ShaderText vertexSource, fragmentSource;
    vertexSource = load_shader(vertexPath);
    if(!vertexSource.data){
        fprintf(stderr, "load_shader() failed.\n");
        free(vertexSource.data);
        exit(EXIT_FAILURE);
    }

    fragmentSource = load_shader(fragmentPath);
    if(!fragmentSource.data){
        fprintf(stderr, "load_shader() failed for fragment shader.\n");
        free(vertexSource.data);
        free(fragmentSource.data);
        exit(EXIT_FAILURE);
    }

    int success = 0;
    char infoLog[512] = {0};

    GLuint v, f;
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(v, 1, (const GLchar *const *)&vertexSource.data, NULL);
    glShaderSource(f, 1, (const GLchar *const *)&fragmentSource.data, NULL);

    glCompileShader(v);
    glGetShaderiv(v, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(v, 512, NULL, infoLog);
        fprintf(stderr, "Vertex shader compilation failed: (%s)\n", infoLog);
        exit(EXIT_FAILURE);
    }

    glCompileShader(f);
    success = 0; memset(infoLog, 0, sizeof(infoLog));
    glGetShaderiv(f, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(f, 512, NULL, infoLog);
        fprintf(stderr, "Fragment shader compilation failed: (%s)\n", infoLog);
        exit(EXIT_FAILURE);
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, v);
    glAttachShader(shaderProgram, f);
    glLinkProgram(shaderProgram);

    success = 0; memset(infoLog, 0, sizeof(infoLog));
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Program linking failed: (%s)\n", infoLog);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(v);
    glDeleteShader(f);

    free(vertexSource.data);
    free(fragmentSource.data);

    return shaderProgram;
}

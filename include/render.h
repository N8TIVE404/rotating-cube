# ifndef RENDER_H
# define RENDER_H

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>

struct Camera;
struct TextureData;

typedef struct Position{
    vec3 scale, axis, location;
    mat4 model, view, projection;
    float angle;
} Position;

typedef struct verticesData{
    float *data;
    size_t size;
    int count;
} verticesData;

typedef struct indicesData{
    unsigned int *data;
    GLenum type;
    size_t size;
    int count;
} indicesData;

typedef struct Mesh{
    size_t firstVertex;
    size_t maxVertex;
    GLsizei stride;
    size_t firstIndex;
    size_t maxIndex;
    int vertexElements;
    int textureElements;
} Mesh;

typedef struct Material{
    GLuint shaderProgram;
    GLuint texture;
} Material;

typedef struct Renderer{
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
} Renderer;

// Display the scene
void render(GLFWwindow*, struct Camera*, Position*, float*, GLuint);

// Handle the input
void process_input(GLFWwindow*, struct Camera*);

void cleanUp(Renderer*, struct TextureData*, Material*);

Renderer init_renderer(verticesData, indicesData, Mesh);

void draw_mesh(struct Camera *cam, Position *pos, Renderer*, Material*, Mesh, mat4, vec3, GLuint, int, int);

void calculate_model(Position*);

void calculate_view(struct Camera*, Position*);

void calculate_projection(struct Camera*, Position*, int, int);

float *calculate_mvp(struct Camera*, Position*, mat4, int, int);

# endif

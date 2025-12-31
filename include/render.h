# ifndef RENDER_H
# define RENDER_H

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>

struct Camera;
struct TextureData;

typedef enum LightType{
    POINT, DIRECTIONAL, SPOT, TYPE_COUNT
} LightType;

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
    int normalElements;
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

typedef struct Light{
    LightType type;
    vec3 position;
    vec3 color;
    vec3 direction;
    float intensity;
} Light;

Light init_light(vec3*, vec3*, float, LightType);

void render_light(struct Camera*, Position*, Renderer*, Light, mat4, Mesh, GLuint, GLuint, GLuint, int, int);

// Display the scene
void render(GLFWwindow*, struct Camera*, Position*, float*, GLuint);

// Handle the input
void process_input(GLFWwindow*, struct Camera*);

void cleanUp(Renderer*, struct TextureData*, Material*);

Renderer init_renderer(verticesData, indicesData, Mesh);

void draw_mesh(struct Camera *, Position *, Renderer*, Material*, Mesh, mat4, vec3, GLuint, int, int);

void calculate_model(Position*);

void calculate_view(struct Camera*, Position*);

void calculate_projection(struct Camera*, Position*, int, int);

float *calculate_mvp(struct Camera*, Position*, mat4, int, int);

# endif

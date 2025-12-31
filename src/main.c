# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <shaders.h>
# include <textures.h>
# include <cglm/cglm.h>
# include <camera.h>
# include <render.h>

// Variables for the viewport width and height
int width, height;

// Cube co-ordinates
float cubeVertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, // 0
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, // 1
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, // 2
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, // 3

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, // 4
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, // 5
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, // 6
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, // 7

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 8
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 9
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 10
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 11

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, // 12
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, // 13
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, // 14
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, // 15

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, // 16
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, // 17
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, // 18
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, // 19

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, // 20
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, // 21
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, // 22
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f  // 23
};

unsigned int cubeIndices[] = {
    0, 1, 2,     2, 3, 0,    // Back Face
    4, 5, 6,     6, 7, 4,    // Front Face
    8, 9, 10,   10, 11, 8,   // Left Face
    12, 13, 14, 14, 15, 12,  // Right Face
    16, 17, 18, 18, 19, 16,  // Bottom Face
    20, 21, 22, 22, 23, 20   // Top Face
};

float cubeLocation[][3] = {
    {1.0f,  -1.0f,  2.0f},
    {0.0f,  -2.0f, 2.0f},
};

Camera cam;     // Scene camera
// Variable that controls the camera movement
float deltaTime = 0.0f;

void framebuffer_resize(GLFWwindow *window, int w, int h){
    if(!window) return;
    // Update the width variable and height variable
    width = w;
    height = h;

    // Update the size of the viewport
    glViewport(0, 0, width, height);
}

// variables to keep track of the previous position of the cursor
float lastX, lastY;
// Track if the current loop is for the first repositioning of the cursor
bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xPos, double yPos){
    if(!window) return;

    // Initialize lastX and lastY if it is the first record of the cursor position
    if(firstMouse){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    // Get the offset for the X and Y axes
    float offsetX = xPos - lastX;
    float offsetY = lastY - yPos;

    // Update the lastX and lastY variables
    lastX = xPos; lastY = yPos;

    // Multiply the offsets by the sensitivity of the cursor
    const float sensitivity = 0.1f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    // Update the yaw and pitch with the offset values
    cam.yaw += offsetX;
    cam.pitch += offsetY;

    // Clip the pitch values
    if(cam.pitch > 89.0f) cam.pitch = 89.0f;
    if(cam.pitch < -89.0f) cam.pitch = -89.0f;

    // Get the orientation of the camera based on the yaw and the pitch
    vec3 direction;
    direction[0] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
    direction[1] = sin(glm_rad(cam.pitch));
    direction[2] = sin(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
    glm_normalize(direction);

    // Set the front of the camera to the orientation of the camera
    cam.front[0] = direction[0]; cam.front[1] = direction[1]; cam.front[2] = direction[2];
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset){
    if(!window) return;
    (void)xOffset;

    // Apply the offset to the field of view
    cam.fieldOfView -= (float)yOffset;
    // Clip the field of view between 1 and 45
    if(cam.fieldOfView < 1.0f) cam.fieldOfView = 1.0f;
    if(cam.fieldOfView > 60.0f) cam.fieldOfView = 60.0f;
}

int
main(void){
    // Initilaize GLFW
    glfwInit();

    // Setup OpenGL profile and version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window and check for success
    GLFWwindow *window = glfwCreateWindow(3840, 2160, "N8TIVE WORLD", NULL, NULL);
    if(!window){
        fprintf(stderr, "glfwCreateWindow() failed!\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Attach the window to the current context
    glfwMakeContextCurrent(window);

    // Load GLAD and check for errors. Cleanup and exit in case of any errors
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fprintf(stderr, "gladLoadGLLoader() failed!\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwGetFramebufferSize(window, &width, &height);

    // Define the position, up and front vectors and the field of view needed to create the camera
    vec3 cameraPosition = {0.0f, 0.0f, 3.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    vec3 cameraFront = {0.0f, 0.0f, -1.0f};

    // Create the camera and assign the navigation, resizing and scrolling functions for scene and window manipulation
    cam = initialize_camera(cameraPosition, cameraFront, cameraUp, 60.0f);
    cam.nav = mouse_callback;
    cam.resize = framebuffer_resize;
    cam.scroll = scroll_callback;

    glfwSetScrollCallback(window, cam.scroll);
    glfwSetFramebufferSizeCallback(window, cam.resize);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cam.nav);

    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Position pos;
    vec3 scale = {1.0f, 1.0f, 1.0f};
    glm_vec3_copy(scale, pos.scale);
    vec3 axis = {1.0f, 1.0f, 0.0f};
    glm_vec3_copy(axis, pos.axis);
    pos.angle = 0.0f;

    Mesh cube = {.firstVertex = 0, .maxVertex = 24, .vertexElements = 3, .normalElements = 3, .textureElements = 2, .stride = 8 * sizeof(float), .maxIndex = 36, .firstIndex = 0};

    Material wooden;
    wooden.shaderProgram = shader_program("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    GLuint lightShader = shader_program("assets/shaders/light.vert", "assets/shaders/light.frag");

    TextureData tex = load_texture("assets/textures/textureimage.jpg", GL_RGB, GL_TEXTURE0);
    wooden.texture = tex.texture;

    verticesData cubeData = {.size = sizeof(cubeVertices), .data = cubeVertices, .count = 12};
    indicesData cubeIndicesData = {.size = sizeof(cubeIndices), .data = cubeIndices, .count = 36};

    Renderer cubeRenderer = init_renderer(cubeData, cubeIndicesData, cube);

    vec3 lightColor = {1.0f, 1.0f, 1.0f};
    Position lightPos;
    lightPos.angle = 20.0f;
    vec3 lightScale = {0.3, 0.3f, 0.3f};
    glm_vec3_copy(lightScale, lightPos.scale);
    vec3 lightAxis = {1.0f, 1.0f, 0.0f};
    glm_vec3_copy(lightAxis, lightPos.axis);

    Light light = init_light(&cubeLocation[0], &lightColor, 1.0f, POINT);

    glUseProgram(wooden.shaderProgram);
    GLuint mvpLoc = glGetUniformLocation(wooden.shaderProgram, "glMvp");
    GLuint glObjectColor = glGetUniformLocation(wooden.shaderProgram, "glObjectColor");
    GLuint glLightColor = glGetUniformLocation(wooden.shaderProgram, "glLightColor");
    GLuint glLightSource = glGetUniformLocation(wooden.shaderProgram, "glLightSource");
    GLuint glModel = glGetUniformLocation(wooden.shaderProgram, "glModel");
    GLuint glViewerPosition = glGetUniformLocation(wooden.shaderProgram, "glViewerPosition");

    vec3 objectColor = {1.0f, 0.5f, 0.31f};

    glUseProgram(lightShader);
    GLuint lightMvpLoc = glGetUniformLocation(lightShader, "glMvp");
    GLuint colorLocLight = glGetUniformLocation(lightShader, "glColor");

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        cam.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        mat4 mvp;
        mat4 lightMvp;
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_input(window, &cam);
        render_light(&cam, &lightPos, &cubeRenderer, light, lightMvp, cube, lightShader, lightMvpLoc, colorLocLight, width, height);
        
        glUseProgram(wooden.shaderProgram);
        glUniform3fv(glLightSource, 1, light.position);
        glUniform3fv(glObjectColor, 1, objectColor);
        glUniform3fv(glLightColor, 1, lightColor);
        glUniformMatrix4fv(glModel, 1, GL_FALSE, (float*)(pos.model));
        glUniform3fv(glViewerPosition, 1, cam.position);
        draw_mesh(&cam, &pos, &cubeRenderer, &wooden, cube, mvp, cubeLocation[1], mvpLoc, width, height);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUp(&cubeRenderer, &tex, &wooden);

    // Terminate GLFW session
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

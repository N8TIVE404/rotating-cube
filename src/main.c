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
float vertices[] = {
    // Front face (Z = +0.5)
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Back face (Z = -0.5)
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,

    // Left face (X = -0.5)
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

    // Right face (X = +0.5)
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,

     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Bottom face (Y = -0.5)
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,

     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    // Top face (Y = +0.5)
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f
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
    if(cam.fieldOfView > 45.0f) cam.fieldOfView = 45.0f;
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
    vec3 cameraPosition = {0.0f, 3.0f, 2.0f};
    vec3 cameraUp = {0.0f, 1.0f, 0.0f};
    vec3 cameraFront = {0.0f, 0.0f, -1.0f};

    // Create the camera and assign the navigation, resizing and scrolling functions for scene and window manipulation
    cam = initialize_camera(cameraPosition, cameraFront, cameraUp, 45.0f);
    cam.nav = mouse_callback;
    cam.resize = framebuffer_resize;
    cam.scroll = scroll_callback;

    glfwSetScrollCallback(window, cam.scroll);
    glfwSetFramebufferSizeCallback(window, cam.resize);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cam.nav);

    float lastFrame = 0.0f;

    glEnable(GL_DEPTH_TEST);

    Position pos;
    vec3 scale = {0.7f, 0.7f, 0.7f};
    glm_vec3_copy(scale, pos.scale);
    vec3 axis = {1.0f, 1.0f, 0.0f};
    glm_vec3_copy(axis, pos.axis);
    pos.angle = 0.0f;

    Mesh cube = {.first_vertex = 0, .max_vertex = 36, .vertexElements = 3, .textureElements = 2, .stride = 5 * sizeof(float)};
    Material wooden;
    wooden.shaderProgram = shader_program("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    TextureData tex = load_texture("assets/textures/textureimage.jpg", GL_RGB, GL_TEXTURE0);
    wooden.texture = tex.texture;

    verticesData vd = {.size = sizeof(vertices), .data = vertices, .count = 36};
    Renderer cubeRenderer = init_renderer(vd, GL_ARRAY_BUFFER, cube);

    GLuint mvpLoc = glGetUniformLocation(wooden.shaderProgram, "glMvp");
    GLuint tex0ID = glGetUniformLocation(wooden.shaderProgram, "tex0");
    glUniform1i(tex0ID, 0);

    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        cam.deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        mat4 mvp;
        glClearColor(0.6f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_input(window, &cam);

        draw_mesh(&cam, &pos, &cubeRenderer, &wooden, cube, mvp, mvpLoc, width, height);
        pos.angle += 1.0f;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    cleanUp(&cubeRenderer, &tex, &wooden);

    // Terminate GLFW session
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

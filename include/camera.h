# ifndef CAMERA_H
# define CAMERA_H

# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <cglm/cglm.h>

// Camera struct
typedef struct Camera{
    float position[3];
    float up[3];
    float front[3];
    float fieldOfView, speed, yaw, pitch, deltaTime;
    void (*nav)(GLFWwindow*, double, double);   // navigation
    void (*resize)(GLFWwindow*, int, int);  ;   // framebuffer resize
    void (*scroll)(GLFWwindow*, double, double);    // scroll call back
} Camera;

Camera initialize_camera(vec3, vec3, vec3, float);
void render(GLFWwindow*,int*, int*, float*, size_t, Camera*);
void process_input(GLFWwindow*, Camera*);

# endif

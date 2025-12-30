# include <camera.h>
# include <textures.h>
# include <shaders.h>

// Create and populate the camera members
Camera initialize_camera(vec3 position, vec3 front, vec3 up, float fieldOfView){
    Camera cam;
    glm_vec3_copy(position, cam.position);
    glm_vec3_copy(front, cam.front);
    glm_vec3_copy(up, cam.up);
    cam.fieldOfView = fieldOfView;
    cam.deltaTime = 0.0f;
    cam.speed = 2.5f;
    cam.yaw = -90.0f;
    cam.pitch = 0.0f;

    return cam;
}


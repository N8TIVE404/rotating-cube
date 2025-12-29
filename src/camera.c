# include <camera.h>
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


void render(GLFWwindow *window, int *width, int *height, float *vertices, size_t size, Camera *cam){
    float lastFrame = 0.0f;

    // Set the size of the viewport with the width and height obtained from querying GLFW
    glfwGetFramebufferSize(window, width, height);
    glViewport(0, 0, *width, *height);

    // Create a shader program and activate it
    GLuint shaderProgram = shader_program("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    glUseProgram(shaderProgram);

    // Create, bind, populate and enable the vao and vbo
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Unbind the vao to maintain the state
    glBindVertexArray(0);

    // Enable depth test to ensure only the closest point is rendered
    glEnable(GL_DEPTH_TEST);

    mat4 projection, view, model;
    vec3 axis = {1.0f, 1.0f, 0.0f};
    glm_normalize(axis);
    float angle = 0.0f;
    vec3 scale = {0.7f, 0.7f, 0.7f};

    GLuint mvpLoc = glGetUniformLocation(shaderProgram, "glMvp");

    // Rendering loop
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        cam -> deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.6f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_input(window, cam);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);

        // Scale the cube and get the model matrix of the scaled cube
        glm_mat4_identity(model);
        glm_scale(model, scale);

        // Get the view matrix relative to the position of the camera in the world
        vec3 ans;
        glm_vec3_add(cam -> position, cam -> front, ans);
        glm_lookat(cam -> position, ans, cam -> up, view);
        
        // Rotate the object on the axis given and get the projection matrix
        glm_mat4_identity(projection);
        glm_rotate(model, glm_rad(angle), axis);
        glm_perspective(glm_rad(cam -> fieldOfView), (float)(*width)/(*height), 0.1f, 100.0f, projection);

        // Get the final matrix to send to the shader
        // Final = projection * view * model
        mat4 final;
        mat4 temp;
        glm_mat4_mul(projection, view, temp);
        glm_mat4_mul(temp, model, final);
        glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, (float*)final);

        // Update  the angle of rotation
        if(angle > 360) angle = 0.0f;
        angle += 1.0f;

        // Draw the triangles
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup after the loop
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(shaderProgram);
}

void process_input(GLFWwindow *window, Camera *cam){
    // Close window on ESC
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    // Get the speed of camera movement that is platform independent
    float velocity = cam -> speed * cam -> deltaTime;

    // Movement forward, towards the user
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        glm_vec3_muladds(cam -> front, velocity, cam -> position);
    }

    // Movement backwards, into the screen
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        glm_vec3_mulsubs(cam -> front, velocity, cam -> position);
    }

    // Movement to the left along -x axis
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        vec3 temp;
        glm_cross(cam -> front, cam -> up, temp);
        glm_normalize(temp);
        glm_vec3_muladds(temp, velocity, cam -> position);
    }

    // Movement along the right towards +x axis
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        vec3 temp;
        glm_cross(cam -> up, cam -> front, temp);
        glm_normalize(temp);
        glm_vec3_muladds(temp, velocity, cam -> position);
    }

    /*
     * The functions above appear similar but yield different results because the 'front' member of the camera
     * stores the orientation of the camera. 'front' will have different orientations hence the cross product will yield
     * opposing vectors
     *
    */
}


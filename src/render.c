# include <render.h>
# include <camera.h>
# include <shaders.h>
# include <textures.h>

// Helper function to make the model matrix
// The function modifies the model matrix in the position structure used for all matrices calculations
// We apply uniform scaling so, the order or scaling vs rotation is not significant
void calculate_model(Position *pos){
    glm_mat4_identity(pos -> model);
    glm_translate(pos -> model, pos -> location);
    glm_scale(pos -> model, pos -> scale);
    glm_rotate(pos -> model, glm_rad(pos -> angle), pos -> axis);
}

// Helper function for making the view matrix
// The matrix is calculated relative to the camera
void calculate_view(Camera *cam, Position *pos){
    glm_mat4_identity(pos -> view);
    vec3 ans;
    glm_vec3_add(cam -> position, cam -> front, ans);
    glm_lookat(cam -> position, ans, cam -> up, pos -> view);
}

// Helper function for calculating the projection matrix
void calculate_projection(Camera *cam, Position *pos, int width, int height){
    glm_mat4_identity(pos -> projection);
    glm_perspective(glm_rad(cam -> fieldOfView), (float)width / height, 0.1f, 100.0f, pos -> projection);
}

// This function calculates the final matrix using the functions defined above
float *calculate_mvp(Camera *cam, Position *pos, mat4 mvp, int width, int height){
    mat4 temp;
    calculate_model(pos);
    calculate_view(cam, pos);
    calculate_projection(cam, pos, width, height);
    glm_mat4_mul(pos -> view, pos -> model, temp);
    glm_mat4_mul(pos -> projection, temp, mvp);
    return (float*)mvp;
}

// A renderer is defined for different meshes
// It is to be initialized once per mesh is the interface meshes use to interact with OpenGL functionality
Renderer init_renderer(verticesData vd, indicesData id, Mesh mesh){
    Renderer r;

    glGenVertexArrays(1, &r.vao);
    glBindVertexArray(r.vao);

    glGenBuffers(1, &r.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, r.vbo);
    glBufferData(GL_ARRAY_BUFFER, vd.size, vd.data, GL_STATIC_DRAW);

    glGenBuffers(1, &r.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, id.size, id.data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, mesh.vertexElements, GL_FLOAT, GL_FALSE, mesh.stride, (void*)0);
    glVertexAttribPointer(1, mesh.normalElements, GL_FLOAT, GL_FALSE, mesh.stride, (void*)(mesh.vertexElements * sizeof(float)));

    glBindVertexArray(0);

    return r;
}

Light init_light(vec3 *position, vec3 *color, float intensity, LightType type){
    Light l;
    glm_vec3_copy(*position, l.position);
    glm_vec3_copy(*color, l.color);
    l.intensity = intensity;
    l.type = type;
    return l;
}

void render_light(Camera *cam, Position *pos, Renderer *r, Light light, mat4 lightMvp, Mesh mesh, GLuint shaderProgram, GLuint lightMvpLoc, GLuint colorLoc, int width, int height){
    GLuint lightVao;
    glGenVertexArrays(1, &lightVao);
    glBindVertexArray(lightVao);
    glBindBuffer(GL_ARRAY_BUFFER, r -> vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r -> ebo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, mesh.vertexElements, GL_FLOAT, GL_FALSE, mesh.stride, (void*)0);

    glUseProgram(shaderProgram);
    glm_vec3_copy(light.position, pos -> location);
    glUniform3fv(colorLoc, 1, light.color);
    glUniformMatrix4fv(lightMvpLoc, 1, GL_FALSE, calculate_mvp(cam, pos, lightMvp, width, height));
    glDrawElements(GL_TRIANGLES, mesh.maxIndex, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

// Draw the triangles
void draw_mesh(Camera *cam, Position *pos, Renderer *r, Material *mat, Mesh mesh, mat4 mvp, vec3 location, GLuint mvpLoc, int width, int height){
    glUseProgram(mat -> shaderProgram);
    glBindVertexArray(r -> vao);
//    glBindTexture(GL_TEXTURE_2D, mat -> texture);
    glm_vec3_copy(location, pos -> location);
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, calculate_mvp(cam, pos, mvp, width, height));
    glDrawElements(GL_TRIANGLES, mesh.maxIndex, GL_UNSIGNED_INT, (void *)0);
}


void cleanUp(Renderer *r, TextureData *tex, Material *mat){
    glDeleteBuffers(1, &(r -> vbo));
    glDeleteBuffers(1, &(r -> ebo));
    glDeleteVertexArrays(1, &(r -> vao));
    destroy_textures(*tex);
    glDeleteProgram(mat -> shaderProgram);
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


# version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexture;

uniform mat4 glMvp;
uniform mat4 glModel;
out vec3 glNormal;
out vec3 glFragPos;

void main(){
    gl_Position = glMvp * vec4(aPosition, 1.0);
    glFragPos = vec3(glModel * vec4(aPosition, 1.0));
    glNormal = mat3(transpose(inverse(glModel))) * aNormal;
}

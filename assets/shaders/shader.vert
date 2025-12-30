# version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexture;

out vec2 tex;
uniform mat4 glMvp;

void main(){
    gl_Position = glMvp * vec4(aPosition, 1.0);
    tex = aTexture;
}

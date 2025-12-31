# version 330 core

layout (location = 0)
in  vec3 aPosition;

uniform mat4 glMvp;

void main(){
    gl_Position = glMvp * vec4(aPosition, 1.0);
}

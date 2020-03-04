#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertex_color;
out vec3 fragment_color;

void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1.0);
    fragment_color = vertex_color;
}

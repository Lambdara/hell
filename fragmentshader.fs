#version 330

in vec3 fragment_color;
out vec3 pixel_color;

void main() {
    pixel_color = fragment_color;
}
#version 330

in vec3 fragment_color;
in vec3 fragment_position;
out vec3 pixel_color;

void main() {
    vec3 normal = vec3(0.0, 0.0, 1.0);

    vec3 light1_position = vec3(-1.0, -1.0, 0.3);
    vec3 light1_direction = normalize(light1_position - fragment_position);
    float light1 = max(dot(normal, light1_direction), 0.0);

    vec3 light2_position = vec3(1.0, 1.0, 0.3);
    vec3 light2_direction = normalize(light2_position - fragment_position);
    float light2 = max(dot(normal, light2_direction), 0.0);

    float light = (light1 + light2 + 0.6)/3.0;

    pixel_color = light * fragment_color;
}

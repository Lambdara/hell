#version 330

in vec3 fragment_color;
in vec3 fragment_position;
out vec3 pixel_color;

void main() {
    vec3 normal = vec3(0.0, 0.0, 1.0);

    vec3 light1_position = vec3(-1.0, -1.0, 0.5);
    vec3 light1_diff = light1_position - fragment_position;
    float light1 = max(dot(normal, normalize(light1_diff)), 0.0)/length(light1_diff);

    vec3 light2_position = vec3(1.0, 1.0, 0.5);
    vec3 light2_diff = light2_position - fragment_position;
    float light2 = max(dot(normal, normalize(light2_diff)), 0.0) / length(light2_diff);

    float light = (light1 + light2 + 0.6)/3.0;

    pixel_color = light * fragment_color;
}

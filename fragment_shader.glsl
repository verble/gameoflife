
#version 330 core

in vec3 color_from_vertex_shader;

out vec4 color;

void main() {
    color = vec4(color_from_vertex_shader, 1.0);
}

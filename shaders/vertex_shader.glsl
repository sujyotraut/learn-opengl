#version 450 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 texture_coordinates;

out vec3 interpolated_color;
out vec2 interpolated_texture_coordinates;

void main()
{
    gl_Position = vec4(vertex_position, 1.0);
    interpolated_texture_coordinates = texture_coordinates;
    interpolated_color = vertex_color;
}
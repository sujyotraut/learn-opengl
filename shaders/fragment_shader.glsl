#version 450 core
in vec3 interpolated_color;

out vec4 fragment_color;
uniform float x;

void main()
{
    fragment_color = vec4(x, interpolated_color.yz, 1.0f);
} 
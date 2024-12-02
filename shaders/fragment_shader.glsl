#version 450 core
in vec2 interpolated_texture_coordinates;

out vec4 fragment_color;
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec2 TexCoord = interpolated_texture_coordinates;
    fragment_color = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2f);
    // fragment_color = texture(texture1, interpolated_texture_coordinates);
    // fragment_color = vec4(interpolated_texture_coordinates, 1.0f, 1.0f);
    // fragment_color = vec4(interpolated_color, 1.0f);
} 
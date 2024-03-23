#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 pos;
in vec3 normal;
in vec2 texture;
in vec3 color;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec3 out_texture;

void main()
{
    // TODO(student): Write pixel out color
   // out_color = vec4(color, 1);
    out_color = vec4(normal, 1);
   // out_texture = vec3(texture, 1);
}

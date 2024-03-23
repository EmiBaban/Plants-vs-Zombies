#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Output
// TODO(student): Output values to fragment shader
out vec3 pos;
out vec3 normal;
out vec2 texture;
out vec3 color;

void main()
{
    // TODO(student): Send output to fragment shader
    pos = v_pos;
    normal = v_normal;
    texture = v_texture;
    color = v_color;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(pos, 1.0f);
}

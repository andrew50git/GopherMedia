#version 330

uniform mat4 world_transform;
uniform mat4 view_projection;
uniform vec4 color;
in vec3 inPosition;
out vec4 vertex_color;

void main()
{
    gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0) * world_transform * view_projection;
    vertex_color = color;
}

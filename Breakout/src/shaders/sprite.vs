#version 330 core

layout (location = 0) in vec4 vertex; // vec2 position and vec2 texCoords

out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    texCoord = vertex.zw; // Swizzle the z and w vars of this vector that hold the texture coordinates.
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0); // Swizzle the x and y vars of this vector that give the position and set z to 0.0.
}
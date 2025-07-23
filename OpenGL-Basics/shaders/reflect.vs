#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 n;

out vec3 norm;
out vec3 worldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    norm = mat3(transpose(inverse(model))) * n;
    worldPos = vec3(model * vec4(pos, 1.0));
    gl_Position = projection  * view * vec4(pos, 1.0);
}
#version 330 core
out vec4 FragColor;

in vec3 norm;
in vec3 worldPos;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{
    vec3 I = normalize(worldPos - camPos);
    vec3 R = reflect(I, normalize(norm));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
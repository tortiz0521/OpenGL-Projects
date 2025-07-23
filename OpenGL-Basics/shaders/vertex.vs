#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec3 aNorm;
layout (location = 1) in vec2 atexCoord;

out vec2 texCoord;

out vec3 worldPos;
out vec3 norm;
out vec3 lPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool scale; // Detrmines if we are scaling for outlining of a shader!

void main()
{
   if(!scale) {
      gl_Position = projection * view * model * vec4(aPos, 1.0);
      worldPos = vec3(model * vec4(aPos, 1.0));
      //norm = mat3(transpose(inverse(model))) * aNorm;
      texCoord = atexCoord;
   }
   else {
      gl_Position = projection * view * model * vec4(aPos + aNorm * 0.02, 1.0);
      worldPos = vec3(model * vec4(aPos, 1.0));
      norm = mat3(transpose(inverse(model))) * aNorm;
      texCoord = atexCoord;
   }
}
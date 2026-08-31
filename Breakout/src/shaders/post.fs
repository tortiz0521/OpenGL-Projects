#version 330 core

in vec2 coords;
out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edgeKernel[9];
uniform float blurKernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;


void main()
{
    color = vec4(0.0f);
    vec3 sample[9];

    // sample from texture offsets if using convolution matrix
    if (chaos || shake) {
        for (int i = 0; i < 9; i++) {
            sample[i] = vec3(texture(scene, coords.st + offsets[i]));
        }
    }

    if (chaos) {
        for (int i = 0; i < 9; i++) {
            color += vec4(sample[i] * edgeKernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    else if (confuse) {
        color = vec4(1.0 - texture(scene, coords).rgb, 1.0f);
    }
    else if (shake) {
        for (int i = 0; i < 9; i++) {
            color += vec4(sample[i] * blurKernel[i], 0.0f);
        }
        color.a = 1.0f;
    }
    else {
        color = texture(scene, coords);
    }
}
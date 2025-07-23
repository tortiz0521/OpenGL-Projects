#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D texture1;
uniform bool fb;

const float offset = 1.0 / 300.0;

void main()
{
    vec4 texColor;
    if(fb) {
        // texColor = vec4(vec3(1.0 - texture(texture1, texCoord).rgb), 1.0); // Inversion!

        /*
            // Greyscale!
            vec3 temp = vec3(texture(texture1, texCoord));
            float average = (temp.r + temp.g + temp.b) / 3;
            texColor = vec4(vec3(average), 1.0);
        */

        vec2 offsets[9] = vec2[](
            vec2(-offset,  offset), // top-left
            vec2( 0.0f,    offset), // top-center
            vec2( offset,  offset), // top-right
            vec2(-offset,  0.0f),   // center-left
            vec2( 0.0f,    0.0f),   // center-center
            vec2( offset,  0.0f),   // center-right
            vec2(-offset, -offset), // bottom-left
            vec2( 0.0f,   -offset), // bottom-center
            vec2( offset, -offset)  // bottom-right    
        );

        // SHARPEN kernel
        /*float kernel[9] = float[](
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1
        );*/

        // BLUR kernel | Each value is divided by 16.0 to ensure that the scene does not become bright. General rule of thumb is to ensure that the kernel values, when added up together, add up to 1.
        /*float kernel[9] = float[](
            2 / 25.0, 4 / 25.0, 2 / 25.0,
            4 / 25.0, 1 / 25.0, 4 / 25.0,
            2 / 25.0, 4 / 25.0, 2 / 25.0
        );*/

        // EDGE DETECTION kernel
        float kernel[9] = float[](
            1,  4, 1,
            4, -16, 4,
            1,  4, 1
        );

        vec3 sampleTex[9];
        for(int i = 0; i < 9; i++) {
            sampleTex[i] = vec3(texture(texture1, texCoord.st + offsets[i]));
        }

        vec3 color = vec3(0.0);
        for(int i = 0; i < 9; i++) {
            color += sampleTex[i] * kernel[i];
        }

        texColor = vec4(color, 1.0);
    }
    else {
        texColor = texture(texture1, texCoord);
    }

    /* if(texColor.a < 0.1)
        discard;
 */
    FragColor = texColor;
}
#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

uniform Material material;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

in vec2 texCoord;
in vec3 norm;
in vec3 worldPos;

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    vec3 n = normalize(norm);
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 result = CalcDirLight(dirLight, n, viewDir);

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse calculation.
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular calculation.
    vec3 specDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, specDir), 0.0), material.shininess);

    vec3 a = light.ambient * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 d = light.diffuse * diff * vec3(texture(material.texture_diffuse1, texCoord));
    vec3 s = light.specular * spec * vec3(texture(material.texture_specular1, texCoord));

    return (a + d + s);
}
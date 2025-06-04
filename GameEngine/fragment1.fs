#version 330 core
out vec4 FragColor;

// We create all of these to be vec3s so that we can control the specific colors of EACH lighting component.
struct Material {
    //vec3 ambient;
    //vec3 diffuse;
    sampler2D diffuse; // We use diffuse maps for ambient! A second map can be created, but the diffuse is sufficient for ambient.
    //vec3 specular;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

uniform Material material;

struct Light  {
    vec3 position;
    vec3 direction;

    vec4 lightVec;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Attenuation variables:
    float constant;
    float linear;
    float quadratic;
    float cutOff;

    // Smooth/Soft edges variables:
    float outerCutOff;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;

struct PointLight {
    vec3 position;

    // Attenuation
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Attenuation variables:
    float constant;
    float linear;
    float quadratic;
    float cutOff;

    // Smooth/Soft edges variables:
    float outerCutOff;
};

uniform SpotLight spotLight;

// uniform Light light;

//in vec3 color;
in vec2 texCoord;
in vec3 norm;
in vec3 worldPos;

//uniform vec4 ourColor;
//uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float mixer;

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir);

void main()
{
    //FragColor = mix(texture(texture1, texCoord), texture(texture2, vec2(1.0 - texCoord.x, texCoord.y)), mixer);

    vec3 n = normalize(norm);
    vec3 viewDir = normalize(viewPos - worldPos);

    vec3 result = CalcDirLight(dirLight, n, viewDir);

    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(pointLights[i], n, worldPos, viewDir);
    }

    result += CalcSpotLight(spotLight, n, worldPos, viewDir);

    FragColor = vec4(result, 1.0);

    // float distance = length(light.position - worldPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
    //vec3 lightDir = normalize(light.position - worldPos);
    //vec3 lightDir = normalize(-light.direction);
    //float theta = dot(lightDir, normalize(-light.direction));
    //float epsilon = light.cutOff - light.outerCutOff;
    //float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //if(theta > light.outerCutOff)
    //{
        // diffuse
    //    float d = max(dot(n, lightDir), 0.0);
    //    vec3 diffuse = d * vec3(texture(material.diffuse, texCoord)).rgb * light.diffuse;
    //    diffuse *= attenuation;
    //    diffuse *= intensity;

    //     // ambient
    //     //vec3 ambient = material.ambient * light.ambient;
    //     vec3 ambient = vec3(texture(material.diffuse, texCoord)).rgb * light.ambient;
    //     //ambient *= attenuation;

    //     // specular
    //     vec3 viewDir = normalize(viewPos - worldPos);
    //     vec3 reflectDir = reflect(-lightDir, norm);
    //     float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //     vec3 specular = vec3(texture(material.specular, texCoord)).rgb * spec * light.specular;
    //     specular *= attenuation;
    //     specular *= intensity;

    //     vec3 result = ambient + diffuse + specular; // + vec3(texture(material.emission, texCoord)).rgb;
    //     FragColor = vec4(result, 1.0);
    // }
    // else
    // {
    //     FragColor = vec4(light.ambient * vec3(texture(material.diffuse, texCoord)), 1.0);
    // }*/
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(normal, lightDir), 0.0); // Diffuse shading.
    vec3 reflectDir = reflect(-lightDir, normal); //Specular Shading.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
    vec3 a = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 d = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 s = light.specular * spec * vec3(texture(material.specular, texCoord));
    return (a + d + s);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 lightDir = normalize(light.position - worldPos);
    
    float diff = max(dot(normal, lightDir), 0.0); // Diffuse shading.
    vec3 reflectDir = reflect(-lightDir, normal); //Specular Shading.
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 a = light.ambient * vec3(texture(material.diffuse, texCoord));
    vec3 d = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
    vec3 s = light.specular * spec * vec3(texture(material.specular, texCoord));

    a *= attenuation;
    d *= attenuation;
    s *= attenuation;
    return (a + d + s);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 worldPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - worldPos);

    float distance = length(light.position - worldPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    if (theta > light.outerCutOff) {
        float diff = max(dot(normal, lightDir), 0.0); // Diffuse shading.
        vec3 reflectDir = reflect(-lightDir, normal); //Specular Shading.
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 a = light.ambient * vec3(texture(material.diffuse, texCoord));
        vec3 d = light.diffuse * diff * vec3(texture(material.diffuse, texCoord));
        vec3 s = light.specular * spec * vec3(texture(material.specular, texCoord));

        d *= attenuation;
        s *= attenuation;
        d *= intensity;
        s *= intensity;

        return (a + s + d);
    }
    else {
        return vec3(0.0);
    }
}
#version 330 core
out vec4 FragColor; //define output variable

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointlights[NR_POINT_LIGHTS];
uniform SpotLight spotlight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
  
    vec3 result = CalcDirLight(dirlight, norm, viewDir);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointlights[i], norm, FragPos, viewDir);
    }
    result += CalcSpotLight(spotlight, norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(Normal * 0.5 + 0.5, 1.0); // 输出法线数据
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords).rgb);
    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords).rgb);
    // diffuse
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords).rgb);
    //计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords).rgb);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords).rgb);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 FragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

     //attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords).rgb);
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords).rgb);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords).rgb);

    diffuse *= attenuation * intensity;
    ambient *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
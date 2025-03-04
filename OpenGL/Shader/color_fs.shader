#version 330 core
out vec4 FragColor; //define output variable

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

// Texture Sample
//uniform sampler2D texture1; 
//uniform sampler2D texture2; 
void main()
{
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    //计算光源对当前片段实际的漫反射影响。结果值再乘以光的颜色，得到漫反射分量

    //specular
    float specularStrenth = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrenth * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    //环境光分量和漫反射分量相加，然后把结果乘以物体的颜色，获得片段输出颜色
    FragColor = vec4(result, 1.0);
}
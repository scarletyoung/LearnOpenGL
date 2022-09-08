#type vertex
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 vNormal;
out vec3 vPos;
out vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 nModel;
void main()
{
 vPos = vec3(model * vec4(aPos, 1.0));
 gl_Position = projection * view * vec4(vPos, 1.0);
 vNormal = mat3(nModel) * aNormal;
 vTexCoords = aTexCoords;
}

#type fragment
#version 450 core
in vec3 vNormal;
in vec3 vPos;
in vec2 vTexCoords;

out vec4 FragColor;

uniform vec3 viewPos;

struct Material 
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};
uniform Material material;

struct DirectionalLight
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform DirectionalLight dLight;

struct PointLight
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quad;
};
#define NP_POINT_LIGHTS 4
uniform PointLight pLights[NP_POINT_LIGHTS];

struct SpotLight
{
  vec3 position;
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quad;

  float cutoff;
  float outerCutoff;
};
uniform SpotLight sLight;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
  vec3 ambient = light.ambient * texture(material.diffuse, vTexCoords).rgb;
  vec3 lightDir = normalize(-light.direction);
  vec3 diffuse = light.diffuse * texture(material.diffuse, vTexCoords).rgb * max(dot(normal, lightDir), 0.0);
  
  vec3 h = normalize(lightDir + viewDir);
  vec3 specular = light.specular * texture(material.specular, vTexCoords).rgb * pow(max(dot(normal, h), 0.0), material.shininess);
  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) 
{
  vec3 ambient = light.ambient * texture(material.diffuse, vTexCoords).rgb;
  
  vec3 lightDir = light.position - vPos;
  float distance = length(lightDir);
  lightDir = normalize(lightDir);

  float diff = max(dot(normal, lightDir), 0.0);
  vec3 diffuse = texture(material.diffuse, vTexCoords).rgb * diff * light.diffuse;
 
  vec3 r = reflect(-lightDir, normal);
  vec3 h = normalize(lightDir + viewDir);
  float spec = pow(max(dot(r, viewDir), 0.0), material.shininess);
  vec3 specular = texture(material.specular, vTexCoords).rgb * spec * light.specular;
  
  float attenuation = 1.0f / (light.constant + (light.linear + light.quad * distance) * distance);

  return (ambient + diffuse + specular);// * attenuation;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
  vec3 ambient = texture(material.diffuse, vTexCoords).rgb * light.ambient;

  vec3 lightDir = light.position - vPos;
  float distance = length(lightDir);
  lightDir = normalize(lightDir);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

  vec3 diffuse = texture(material.diffuse, vTexCoords).rgb * max(dot(normal, lightDir), 0.0) * light.diffuse;

  vec3 h = normalize(lightDir + viewDir);
  float spec = pow(max(dot(h, normal), 0.0), material.shininess);
  vec3 specular = texture(material.specular, vTexCoords).rgb * spec * light.specular;
  
  float attenuation = 1.0f / (light.constant + (light.linear + light.quad * distance) * distance);

  return ambient + (diffuse + specular) * attenuation * intensity;
}

void main()
{
  vec3 n = normalize(vNormal);
  vec3 viewDir = normalize(viewPos - vPos);
  vec3 result;
  result = CalcDirLight(dLight, n, viewDir);
  for (int i = 0; i < NP_POINT_LIGHTS; ++i) {
    result += CalcPointLight(pLights[i], n, viewDir);
  }
  result += CalcSpotLight(sLight, n, viewDir);
  FragColor = vec4(result, 1.0);
}
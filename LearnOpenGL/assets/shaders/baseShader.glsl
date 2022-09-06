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

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec3 viewPos;

struct Material 
{
  sampler2D diffuse;
  sampler2D specular;
  float shiness;
};
uniform Material material;

struct Light
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform Light light;
void main()
{
  vec3 ambient = texture(material.diffuse, vTexCoords).rgb * light.ambient;

  vec3 n = normalize(vNormal);
  vec3 lightDir = normalize(light.position - vPos);
  vec3 diffuse = texture(material.diffuse, vTexCoords).rgb * max(dot(n, lightDir), 0.0) * light.diffuse;

  vec3 viewDir = normalize(viewPos - vPos);
  vec3 h = normalize(lightDir + viewDir);
  float spec = pow(max(dot(h, n), 0.0), material.shiness);
  vec3 specular = texture(material.specular, vTexCoords).rgb * spec * light.specular;

  FragColor = vec4((ambient + diffuse + specular), 1.0f);
}
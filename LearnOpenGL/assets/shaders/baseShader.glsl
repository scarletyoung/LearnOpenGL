#type vertex
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
out vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
 gl_Position = projection * view * model * vec4(aPos, 1.0);
 vTexCoords = aTexCoords;
}

#type fragment
#version 450 core
in vec2 vTexCoords;
out vec4 FragColor;

uniform sampler2D vTexture;
uniform sampler2D vTexture2;

void main()
{
  FragColor = mix(texture(vTexture, vTexCoords), texture(vTexture2, vTexCoords), 0.2);
}
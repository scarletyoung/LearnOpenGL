#type vertex
#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;
out vec3 vColor;
out vec2 vTexCoords;
void main()
{
 gl_Position = vec4(aPos, 1.0);
 vColor = aColor;
 vTexCoords = aTexCoords;
}

#type fragment
#version 450 core
in vec3 vColor;
in vec2 vTexCoords;
out vec4 FragColor;

uniform sampler2D vTexture;

void main()
{
  FragColor = texture(vTexture, vTexCoords) * vec4(vColor, 1.0);
}
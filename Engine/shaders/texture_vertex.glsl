#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPosition;
out vec4 FragPositionLightSpace;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

uniform mat4 u_LightSpaceMatrix;

void main()
{
    TexCoords = a_TexCoords;
    Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

    vec4 worldPosition = u_Model * vec4(a_Pos, 1.0);
    FragPosition = vec3(worldPosition);
    FragPositionLightSpace = u_LightSpaceMatrix * worldPosition;

    gl_Position = u_Projection * u_View * worldPosition;

}
#version 410 core

layout (location = 0) in vec3 aVertexPosition;

out vec3 TextureCoordinates;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    TextureCoordinates = aVertexPosition;
    vec4 tempPos = u_Projection * u_View * vec4(aVertexPosition, 1.0);
    gl_Position = tempPos.xyww;
}

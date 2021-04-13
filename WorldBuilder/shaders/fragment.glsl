#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 NormCoords;

uniform sampler2D texture_diffuse1;

void main()
{    
     //FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    //FragColor = vec4(NormCoords.x, NormCoords.y, NormCoords.z, 1.0f);
    FragColor = vec4(texture(texture_diffuse1, TexCoords).rgb, 1.0f);
}
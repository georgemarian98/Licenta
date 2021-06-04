#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 NormCoords;
in vec3 FragPosition;

uniform sampler2D u_TextureDiffuse1;
uniform sampler2D u_TextureSpecular1;

uniform vec3 u_TintColor;

uniform vec3 u_LightPosition;
uniform vec3 u_LightColor;

uniform vec3 u_CameraPosition;  

vec3 PhongLightingComponent( )
{
    //Ambient component
    const float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_LightColor;

    //Diffuse component
    vec3 normal = normalize(NormCoords);
    vec3 lightDirection = normalize(-u_LightPosition);
    float diffuseCoeficient = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diffuseCoeficient * u_LightColor * vec3(texture(u_TextureDiffuse1, TexCoords));

    //Specular component
    float specularStrength = 0.5;
    vec3 viewDirection = normalize(u_CameraPosition - FragPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal); 

    float specularCoefficient = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * specularCoefficient * u_LightColor * vec3(texture(u_TextureSpecular1, TexCoords)); 

    return ambient + diffuse + specular;
}

void main()
{    
    vec3 light = PhongLightingComponent( );
    FragColor = vec4(u_TintColor * light, 1.0f);
}
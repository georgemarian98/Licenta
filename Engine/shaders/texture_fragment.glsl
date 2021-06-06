#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 NormCoords;
in vec3 FragPosition;

struct Light {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D u_TextureDiffuse1;
uniform sampler2D u_TextureSpecular1;

uniform vec3 u_TintColor;

uniform Light u_LightProperties;

uniform vec3 u_CameraPosition;  

vec3 BlinnPhongLightingComponent( )
{
    vec4 diffuseTexture = texture(u_TextureDiffuse1, TexCoords);
    // if(diffuseTexture.a < 0.5){
    //     discard;
    // }

    //Ambient component
    vec3 ambient = u_LightProperties.ambient * diffuseTexture.rgb;

    //Diffuse component
    vec3 normal = normalize(NormCoords);
    vec3 lightDirection = normalize(-u_LightProperties.position);
    float diffuseCoeficient = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = u_LightProperties.diffuse * diffuseCoeficient * diffuseTexture.rgb;

    //Specular component
    vec3 viewDirection = normalize(u_CameraPosition - FragPosition);
    vec3 halfDirection = normalize(lightDirection + viewDirection);

    float specularCoefficient = pow(max(dot(normal, halfDirection), 0.0), 16);
    vec3 specular =  u_LightProperties.specular * specularCoefficient * texture(u_TextureSpecular1, TexCoords).rgb; 

    return ambient + diffuse + specular;
}

void main()
{    
    vec3 light = BlinnPhongLightingComponent( );
    FragColor = vec4(u_LightProperties.color * u_TintColor * light, 1.0f);
}
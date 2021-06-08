#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPosition;
in vec4 FragPositionLightSpace;

struct Light {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D u_ShadowTexture;

uniform sampler2D u_TextureDiffuse1;
uniform sampler2D u_TextureSpecular1;

uniform Light u_LightProperties;

uniform vec3 u_TintColor;
uniform vec3 u_CameraPosition;  

float CalculateShadow()
{
    // perform perspective divide
    vec3 projCoords = FragPositionLightSpace.xyz / FragPositionLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(u_ShadowTexture, projCoords.xy).x;
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(Normal, u_LightProperties.position)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0; 

    // if(projCoords.z > 1.0)
    //     shadow = 0.0;

    return shadow;
}

vec3 BlinnPhongLightingComponent( )
{
    vec4 diffuseTexture = texture(u_TextureDiffuse1, TexCoords);

    //Ambient component
    vec3 ambient = u_LightProperties.ambient * diffuseTexture.rgb;

    //Diffuse component
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(-u_LightProperties.position);
    float diffuseCoeficient = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = u_LightProperties.diffuse * diffuseCoeficient * diffuseTexture.rgb;

    //Specular component
    vec3 viewDirection = normalize(u_CameraPosition - FragPosition);
    vec3 halfDirection = normalize(lightDirection + viewDirection);

    float specularCoefficient = pow(max(dot(normal, halfDirection), 0.0), 64.0);// 64?
    vec3 specular =  u_LightProperties.specular * specularCoefficient * texture(u_TextureSpecular1, TexCoords).rgb; 

    //Shadow component
    float shadow = CalculateShadow();
    return ambient + (1.0 - shadow) * (diffuse + specular); 
}

void main()
{    
    vec3 light = BlinnPhongLightingComponent( );
    FragColor = vec4(u_LightProperties.color * u_TintColor * light, 1.0f);
    // float depthValue = texture(u_ShadowTexture, TexCoords).r;
    // FragColor = vec4(vec3(depthValue), 1.0);
}
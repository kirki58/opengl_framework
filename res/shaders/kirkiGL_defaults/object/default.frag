#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse0;
    sampler2D diffuse1;
    sampler2D diffuse2;

    sampler2D specular0;
    sampler2D specular1;
    float shininess;
}; 

// 48 BYTES
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 60 BYTES
struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// 80 BYTES
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

// MAX NUMBER OF LIGHTS THAT CAN HAVE EFFECT ON AN OBJECT.
#define NR_POINT_LIGHTS 4
#define NR_DIR_LIGHTS 2
#define NR_SPOT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform uint DirLight_Count;
uniform uint PointLight_Count;
uniform uint SpotLight_Count;
uniform uint DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLights[NR_SPOT_LIGHTS];

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 result = vec3(0.0);
    // phase 1: directional lighting
    for(int i = 0; i < DirLight_Count; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);

    // phase 2: point lights
    for(int i = 0; i < PointLight_Count; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        
    // phase 3: spot light
    for(int i = 0; i < SpotLight_Count; i++)
        result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);
    
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords)) * vec3(texture(material.specular1, TexCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords)) * vec3(texture(material.specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse0, TexCoords)) * vec3(texture(material.diffuse1, TexCoords)) * vec3(texture(material.diffuse2, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular0, TexCoords)) * vec3(texture(material.specular1, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
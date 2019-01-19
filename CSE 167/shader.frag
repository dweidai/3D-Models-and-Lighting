#version 330 core

struct PointLight {
    vec3 position;
    vec3 color;
};
struct SpotLight {
    vec3 direction;
    vec3 position;
    vec3 color;
    float cutoff;
    float exponent;
};
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float Coloring;
};

uniform Material material;
uniform SpotLight spotLight;
uniform PointLight pointLight;
uniform vec3 specular;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 viewDir;
uniform vec3 Objcolor;
uniform float spec;
uniform float theta;
uniform float attenuation;
uniform float normal_coloring;
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.

in vec3 Normal;
in vec3 FragPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    color = vec4(0,0,0,0);
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    if(normal_coloring == 1.0f){
        vec3 normal = normalize(Normal);
        color = vec4(normal, 1.0f);
    }
    else{
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 norm = normalize(Normal);
        vec3 lightColor = pointLight.color;
        //vec3 lightPos;
        vec3 lightPos = pointLight.position;
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / ((0.1f + 0.1f * distance));
        // ambient
        vec3 ambient = lightColor * material.ambient;
        
        // diffuse
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = lightColor * (diff * material.diffuse);
        
        // specular
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lightColor * (spec * material.specular);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        vec3 result = ambient + diffuse + specular;
        color = vec4(result * Objcolor, 1.0);
        
        //now we consider the spotlight
        lightPos = spotLight.position;
        lightDir = normalize(lightPos - FragPos);
        float theta = dot(lightDir, normalize(-spotLight.direction));
        if(theta > spotLight.cutoff)
        {
            //color = vec4(0,0,1,1.0);
            // do lighting calculations
            vec3 lightColor = spotLight.color;
            //vec3 lightPos;
            float distance = length(lightPos - FragPos);
            float attenuation = 1.0 / ( 0.05f * distance);
            // ambient
            vec3 ambient = lightColor * material.ambient;
            
            // diffuse
            vec3 norm = normalize(Normal);
            lightDir = normalize(lightPos - FragPos);
            float diff = max(dot(norm, lightDir), 0.0);
            vec3 diffuse = lightColor * (diff * material.diffuse);
            
            // specular
            vec3 viewDir = normalize(viewPos - FragPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
            float spec = pow(theta,spotLight.exponent);
            vec3 specular = lightColor * (spec * material.specular);
          
            ambient  *= attenuation;
            diffuse  *= attenuation;
            specular *= attenuation;
            vec3 result = ambient + diffuse + specular;
            //color = vec4(0,0,1,1.0);
            color = color + vec4(result * Objcolor, 1.0);
        }
    }
}

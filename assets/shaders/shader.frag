# version 330 core

out vec4 fragColor;
in vec3 glNormal;
in vec3 glFragPos;
uniform vec3 glObjectColor;
uniform vec3 glLightColor;
uniform vec3 glLightSource;
uniform vec3 glViewerPosition;

void main(){
    vec3 norm = normalize(glNormal);
    vec3 lightDirection = normalize(glLightSource - glFragPos);

    float diff = max(dot(lightDirection, norm), 0.0f);
    vec3 diffuse = diff * glLightColor;

    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;

    vec3 viewDirection = normalize(glViewerPosition - glFragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
    vec3 specular = spec * specularStrength * glLightColor;

    vec3 ambient = ambientStrength * glLightColor;
    vec3 result = (ambient + diffuse + specular) * glObjectColor;

    fragColor = vec4(result, 1.0f);
}

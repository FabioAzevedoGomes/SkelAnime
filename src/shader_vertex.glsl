#version 450 core

layout(location=0)in vec3 vertexPosition;
layout(location=1)in vec3 vertexColor;
layout(location=2)in vec3 vertexNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform int object;

out vec4 interpolatedColor;

vec4 gouraud(vec3 diffuse, vec3 ambient) {
    vec4 lightPosition = inverse(view) * vec4(2.0,2.0,2.0,1.0);
    vec4 cameraPosition=inverse(view)[3];
    
    vec4 l = normalize(lightPosition - (model * vec4(vertexPosition, 1.0)));
    vec4 n = normalize(inverse(transpose(model)) * vec4(vertexNormal, 0.0));
    
    vec3 diffuseTerm = diffuse * 0.8 * max(0.2,dot(n,l));
    vec3 ambientTerm = ambient * 0.2;

    return vec4(diffuseTerm + ambientTerm, 1.0);
}

void main() {
    if (object == 0)
        interpolatedColor = gouraud(vertexColor, vec3(1.0, 1.0, 1.0));
    else
        interpolatedColor = vec4(1.0, 1.0, 1.0, 1.0);
    gl_Position = proj * view * model * vec4(vertexPosition, 1.0);
}

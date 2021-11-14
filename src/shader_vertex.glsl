#version 450 core

layout(location=0)in vec4 vertexPosition;
layout(location=1)in vec3 vertexColor;
layout(location=2)in vec3 vertexNormal;
layout(location=3)in int vertexBoneId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 cumulativeBoneTransforms[100];
uniform int object;

out vec4 interpolatedColor;

vec4 gouraud(vec3 diffuse, vec3 ambient, vec4 pos) {
    vec4 lightPosition = inverse(view) * vec4(2.0,2.0,2.0,1.0);
    vec4 cameraPosition = inverse(view)[3];
    
    vec4 l = normalize(lightPosition - (model * pos));
    vec4 n = normalize(inverse(transpose(model)) * vec4(vertexNormal, 0.0));
    
    vec3 diffuseTerm = diffuse * 0.8 * max(0.2,dot(n,l));
    vec3 ambientTerm = ambient * 0.2;

    return vec4(diffuseTerm + ambientTerm, 1.0);
}

vec4 deformVertexByBone() {
    mat4 transform = cumulativeBoneTransforms[vertexBoneId];
    return transform * vertexPosition;
}

void main() {

    vec4 posePos;

    if (object == 0) {
        posePos = deformVertexByBone();
        interpolatedColor = gouraud(vertexColor, vec3(1.0, 1.0, 1.0), posePos);
    }
    else {
        posePos = vertexPosition;
        interpolatedColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
    gl_Position = proj * view * model * posePos;
}

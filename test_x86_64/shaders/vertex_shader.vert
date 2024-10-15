#version 320 es

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 vTexCoords;

uniform int[2] enabledFlags;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

out float brightness;
out vec2 texCoords;
out flat int hasTexCoords;

void main() {
    if (enabledFlags[0] == 1) {
        vec3 V = (modelMatrix * vec4(vPosition.xyz, 1.0f)).xyz;
        vec3 N = (normalMatrix * vec4(normal, 0.0f)).xyz;
        float perp = 1.0f - abs(dot(N,V)/length(V));

        brightness = 0.0f + 1.0f*perp;
    } else brightness = 1.0;

    if (enabledFlags[1] == 1) {
        gl_Position = modelMatrix*vec4(vPosition.xy,0.0,1.0);
    } else {
        gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(vPosition.xyz,1.0);
    }
    hasTexCoords = enabledFlags[1];
    texCoords = vTexCoords;
}
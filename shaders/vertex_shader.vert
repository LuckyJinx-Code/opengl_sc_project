#version 320 es

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec3 normal;

uniform int setBrightness;
uniform int hasTexture;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out float brightness;
out vec2 TexCoords;
out flat int hasTexCoords;

void main() {
    if (setBrightness == 1) {
        vec3 vertexPos = (modelMatrix * vec4(vPosition.xyz, 1.0)).xyz;
        vec3 vertexCam = vPosition.xyz - vertexPos;
        float perp = sqrt(1.0-dot(normal,vertexCam)/(length(normal)*length(vertexCam)));
        brightness = perp;
    } else brightness = 1.0;

    if (hasTexture == 1) {
        gl_Position = modelMatrix*vec4(vPosition.xy,0.0,1.0);
    } else {
        gl_Position = projectionMatrix*viewMatrix*modelMatrix*vec4(vPosition.xyz,1.0);
    }
    hasTexCoords = hasTexture;
    TexCoords = vPosition.zw;
}
#version 320 es

precision mediump float;
out vec4 fragColor;
in vec2 TexCoords;
in float brightness;
in flat int hasTexCoords;

uniform sampler2D text;
uniform vec4 color;

void main() {
    if (hasTexCoords == 1) {
        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
        fragColor = color * sampled;
    } else {
        fragColor = vec4(color.x*brightness, 
                        color.y*brightness, 
                        color.z*brightness, 
                        color.a
        );
    }
} 
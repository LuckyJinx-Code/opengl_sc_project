#version 320 es

precision mediump float;
out vec4 fragColor;
in vec2 texCoords;
in float brightness;
in flat int hasTexCoords;

uniform sampler2D text;
uniform vec4 color;

void main() {
    if (hasTexCoords == 1) {
        fragColor = texture(text, texCoords) * vec4(color.x, 
                        color.y, 
                        color.z, 
                        color.a
        );
    } else {
        fragColor = vec4(color.x*brightness, 
                        color.y*brightness, 
                        color.z*brightness, 
                        color.a
        );
    }
} 
#include <stdio.h>

#include "waypoints.h"

void gen_waypoints() {
    float window_vertices[27];

    gen_octagon(
        WP_WINDOW_WIDTH,
        WP_WINDOW_HEIGHT,
        WP_WINDOW_CATET,
        (vec2){0.0f,0.0f},
        window_vertices
    );

    glGenBuffers(1, &wp_window_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, wp_window_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(window_vertices), window_vertices, GL_STATIC_DRAW);
}

void draw_waypoints() {

}
#include <stdio.h>

#include "compass.h"

void gen_compass() {
    
    float angle_display_vertices[27];

    gen_octagon(
        COMPASS_ANGLE_DISPLAY_WIDTH,
        COMPASS_ANGLE_DISPLAY_HEIGHT,
        COMPASS_ANGLE_DISPLAY_CATET,
        (vec2){0.0f,0.0f},
        angle_display_vertices
    );

    add_to_buffer(
        &compass_angle_display_VBO, angle_display_vertices, sizeof(angle_display_vertices));

    float angle_pointer_vertices[] = {
        -COMPASS_ANGLE_POINTER_SIZE/2.0f, 0.0f, 0.0f,
        0.0f, COMPASS_ANGLE_POINTER_SIZE/2.0f, 0.0f,
        COMPASS_ANGLE_POINTER_SIZE/2.0f, 0.0f, 0.0f,
        0.0f, -COMPASS_ANGLE_POINTER_SIZE/2.0f, 0.0f
    };

    add_to_buffer(
        &compass_angle_pointer_VBO, angle_pointer_vertices, sizeof(angle_pointer_vertices));

    float vertices[18*COMPASS_NUM_MARKERS];

    for (int i = 0; i < COMPASS_NUM_MARKERS; i++) {
        float rectangle[18];
        float marker_length = 
            i%2 == 0 ? COMPASS_MARKER_BIG_LEN : COMPASS_MARKER_SMALL_LEN;

        gen_rectangle(
            marker_length, 
            COMPASS_MARKER_THICKNESS,
            (vec3){0.0,0.0,0.0},
            COMPASS_RADIUS - marker_length/2,
            2*GLM_PI*i/COMPASS_NUM_MARKERS,
            rectangle
        );

        for (int j = 0; j < 18; j++)
            vertices[18*i + j] = rectangle[j];
    }

    add_to_buffer(
        &compass_VBO, vertices, sizeof(vertices));
}

void draw_compass(float vPosX, float vPosY) {
    //Uniforms

    reset_modelmatrix();
    set_position(vPosX,vPosY);
    rotate(rotY);
    set_color(WHITE);

    //Drawing lines

    draw(compass_VBO, COMPASS_NUM_VERTICES, GL_TRIANGLES);

    //Drawing units

    char compass_units[][3] = {
        "N\0\0","3\0\0","6\0\0","E\0\0","12\0","15\0","S\0\0","21\0","24\0","W\0\0","30\0","33\0"
    };

    for (int i = 0; i < 12; i++) {
        reset_modelmatrix();
        set_position(vPosX,vPosY);
        set_position(
            (float)COMPASS_NSWE_RADIUS*cos(glm_rad(90) - 2*GLM_PI*i/12 + rotY),
            (float)COMPASS_NSWE_RADIUS*sin(glm_rad(90) - 2*GLM_PI*i/12 + rotY)
        );
        render_text(compass_units[i],i%3 == 0 ? 0.5f : 0.32f,0);
    }

    reset_modelmatrix();
    set_position(
        vPosX, 
        vPosY +
            COMPASS_RADIUS + COMPASS_MARGIN + COMPASS_ANGLE_POINTER_SIZE/2.0f
    );
    set_color(BLACK);
    draw(compass_angle_pointer_VBO,4,GL_TRIANGLE_FAN);

    set_position(0.0f, COMPASS_ANGLE_DISPLAY_HEIGHT/2.0f);
    draw(compass_angle_display_VBO,9,GL_TRIANGLE_FAN);
    
    int deg_rotY = roundf(glm_deg(rotY));
    char rot_display[4];
    set_position(-27.0f*0.15f*COMPASS_ANGLE_FONT_SIZE,0.0f);
    sprintf(rot_display, "%03d", deg_rotY);
    set_color(WHITE);
    render_text(rot_display,COMPASS_ANGLE_FONT_SIZE,0);
    set_position(27.0f*1.75f*COMPASS_ANGLE_FONT_SIZE,27.0f*0.5f*COMPASS_ANGLE_FONT_SIZE);
    render_text("o",COMPASS_ANGLE_FONT_SIZE/1.85f,0);
}
#include <stdio.h>

#include "SSI.h"

void gen_SSI() {
    //Arc shape

    float arc_vertices[3 * SSI_ARC_NUM_VERTICES];

    gen_arc_section(
        SSI_RADIUS_1,SSI_RADIUS_2,(vec2){0.0,0.0},
        glm_rad(SSI_ANGLE), glm_rad(180 - SSI_ANGLE),
        SSI_ARC_NUM_SIDES,
        arc_vertices
    );

    glGenBuffers(1, &SSI_arc_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_arc_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arc_vertices), arc_vertices, GL_STATIC_DRAW);

    //Lines shape

    float lines_vertices[3 * SSI_LINES_NUM_VERTICES];

    float theta = (float)SSI_THICKNESS/SSI_RADIUS_2;
    float ang1 = glm_rad(SSI_ANGLE) + theta/2;
    float ang2 = glm_rad(180 - SSI_ANGLE) - theta/2;

    for (int i = 0; i < SSI_LINES_NUM_VERTICES; i++) {
        float rect_vertices[18];
        float angle = ang1 + (ang2 - ang1)*i/(SSI_NUM_LINES - 1);
        if (i%2 == 0)
            gen_rectangle(SSI_LINE_LEN_BIG,SSI_THICKNESS,(vec2){0.0,0.0},
            SSI_RADIUS_2 + (float)SSI_LINE_LEN_BIG/2,angle,rect_vertices);
        else
            gen_rectangle(SSI_LINE_LEN_SMALL,SSI_THICKNESS,(vec2){0.0,0.0},
            SSI_RADIUS_2 + (float)SSI_LINE_LEN_SMALL/2,angle,rect_vertices);
        for (int j = 0; j < 18; j++)
            lines_vertices[18*i + j] = rect_vertices[j];
    }

    glGenBuffers(1, &SSI_lines_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_lines_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines_vertices), lines_vertices, GL_STATIC_DRAW);

    //Center mark

    float mark_vertices[18];

    gen_rectangle(
        SSI_CENTER_MARK_SIZE,
        SSI_CENTER_MARK_SIZE,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        mark_vertices
    );

    glGenBuffers(1, &SSI_center_mark_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_center_mark_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mark_vertices), mark_vertices, GL_STATIC_DRAW);

    //Pitch level

    float rect_vertices1[18];

    gen_rectangle(
        SSI_PITCH_WIDTH_BIG,
         SSI_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices1
    );
    glGenBuffers(1, &SSI_pitch_line_1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_pitch_line_1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices1), rect_vertices1, GL_STATIC_DRAW);

    float rect_vertices2[18];

    gen_rectangle(
        SSI_PITCH_WIDTH_SMALL,
         SSI_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices2
    );
    glGenBuffers(1, &SSI_pitch_line_2_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_pitch_line_2_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices2), rect_vertices2, GL_STATIC_DRAW);

    float tr_height = sqrt(pow(SSI_TRIANGLE_SIDE/2.0f,2.0f) + pow(SSI_TRIANGLE_SIDE,2.0f));

    float tri_vertices[9] = {
        -SSI_TRIANGLE_SIDE/2.0f, tr_height/2.0, 0.0,
        SSI_TRIANGLE_SIDE/2.0f, tr_height/2.0, 0.0,
        0.0, -tr_height/2.0, 0.0
    };
    glGenBuffers(1, &SSI_triangle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_triangle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tri_vertices), tri_vertices, GL_STATIC_DRAW);

    float horizon_vertices[18];
    gen_rectangle(
        SSI_HORIZON_LINES_WIDTH,
        SSI_HORIZON_LINES_HEIGHT,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        horizon_vertices
    );
    glGenBuffers(1, &SSI_horizon_line_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, SSI_horizon_line_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(horizon_vertices), horizon_vertices, GL_STATIC_DRAW);

}

void draw_SSI(float vPosX, float vPosY) {
    //Draw triangle

    float tr_height = sqrt(pow(SSI_TRIANGLE_SIDE/2.0f,2.0f) + pow(SSI_TRIANGLE_SIDE,2.0f));

    reset_modelmatrix();
    set_position(vPosX,vPosY + SSI_RADIUS_1 + tr_height/2.0f);
    set_color(WHITE);
    draw(SSI_triangle_VBO, 3, GL_TRIANGLES);

    //Drawing arc and lines

    reset_modelmatrix();
    set_position(vPosX,vPosY);
    rotate(rotZ);
    draw(SSI_arc_VBO, SSI_ARC_NUM_VERTICES, GL_TRIANGLE_STRIP);
    draw(SSI_lines_VBO, SSI_LINES_NUM_VERTICES, GL_TRIANGLES);

    //Drawing angle indicator
    
    float angle = 2*GLM_PI - rotX;
    if (angle > GLM_PI) angle = -2*GLM_PI + angle; //180,360 range turns to -180,0

    float SSI_height = SSI_PITCH_NUM_LINES*SSI_PITCH_SPACE_BETWEEN;

    float current_pos = - SSI_height/2  + (angle/GLM_PI) * SSI_height/2;

    float SSI_bottom = - (float)SSI_PITCH_HEIGHT/2;
    float SSI_top = (float)SSI_PITCH_HEIGHT/2;

    float transparency_point1 = (float)SSI_PITCH_VANISH_HEIGHT;
    float transparency_point2 = -(float)SSI_PITCH_VANISH_HEIGHT;

    for (int i = 0; i < SSI_PITCH_NUM_LINES; i++) {
        reset_modelmatrix();
        set_position(vPosX,vPosY);
        rotate(rotZ);
        set_position(0.0f,-current_pos);

        float alpha = 1.0;

        if (current_pos < SSI_bottom || current_pos > SSI_top)
            alpha = 0.0;
        else if (current_pos < transparency_point1) {
            alpha = (current_pos - SSI_bottom)/(transparency_point1 - SSI_bottom);
        } else if (current_pos > transparency_point2) {
            alpha = 1.0 - (current_pos - transparency_point2)/(SSI_top - transparency_point2);
        }

        float color[4] = {1.0f, 1.0f, 1.0f, alpha};

        set_color(color);
        draw(
            i%2 == 0 ? SSI_pitch_line_1_VBO : SSI_pitch_line_2_VBO,
            6, GL_TRIANGLES
        );

        if (i%2 == 0) {
            int num = abs(-180 + 5*i);
            char num_string[3];

            sprintf(num_string, "%d", num);

            set_position(SSI_PITCH_SPACE_NUM + (float)SSI_PITCH_WIDTH_BIG/2.0, 0.0f);
            render_text(num_string,0.3f,0);
            set_position(-SSI_PITCH_SPACE_NUM*2 - (float)SSI_PITCH_WIDTH_BIG, 0.0f);
            render_text(num_string,0.3f,0);

        }

        current_pos += (float)SSI_PITCH_SPACE_BETWEEN;
    }

    //Center mark

    reset_modelmatrix();
    set_position(vPosX, vPosY);
    set_color(BLACK);
    draw(SSI_center_mark_VBO,6,GL_TRIANGLES);
    resize(
        (SSI_CENTER_MARK_SIZE - 2.0f)/SSI_CENTER_MARK_SIZE,
        (SSI_CENTER_MARK_SIZE - 2.0f)/SSI_CENTER_MARK_SIZE
    );
    set_color(YELLOW);
    draw(SSI_center_mark_VBO,6,GL_TRIANGLES);

    //Horizon marks

    for (int i = -1; i <= 1; i += 2) {
        reset_modelmatrix();
        set_position(vPosX, vPosY);
        rotate(rotZ);
        set_position(SSI_HORIZON_LINES_DIST*i,0.0f);
        set_color(BLACK);
        draw(SSI_horizon_line_VBO,6,GL_TRIANGLES);
        resize(
            (SSI_HORIZON_LINES_WIDTH - 2.0f)/SSI_HORIZON_LINES_WIDTH,
            (SSI_HORIZON_LINES_HEIGHT - 2.0f)/SSI_HORIZON_LINES_HEIGHT
        );
        set_color(YELLOW);
        draw(SSI_horizon_line_VBO,6,GL_TRIANGLES);
    }
}
#include <stdio.h>

#include "VSI.h"

void gen_VSI() {
    float box_x1 = -VSI_WIDTH/2.0f;
    float box_x2 = VSI_WIDTH/2.0f - VSI_CATET;
    float box_x3 = VSI_WIDTH/2.0f;

    float box_y1 = -VSI_HEIGHT/2.0f;
    float box_y2 = -VSI_HEIGHT/2.0f + VSI_CATET;
    float box_y3 = VSI_HEIGHT/2.0f - VSI_CATET;
    float box_y4 = VSI_HEIGHT/2.0f;

    float box_vertices[] = {
        box_x1, box_y4, 0.0f,
        box_x2, box_y4, 0.0f,
        box_x3, box_y3, 0.0f,
        box_x3, box_y2, 0.0f,
        box_x2, box_y1, 0.0f,
        box_x1, box_y1, 0.0f,
        box_x1, box_y4, 0.0f
    };

    add_to_buffer(&VSI_VBO, box_vertices, sizeof(box_vertices));

    float line_vertices[18];
    
    gen_rectangle(
        VSI_LINE_WIDTH_BIG,
        VSI_LINE_THICKNESS,
        (vec2){0.0f,0.0f},
        0.0f,
        0.0f,
        line_vertices
    );

    add_to_buffer(&VSI_line_1_VBO, line_vertices, sizeof(line_vertices));

    gen_rectangle(
        VSI_LINE_WIDTH_SMALL,
        VSI_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0f,
        0.0f,
        line_vertices
    );

    add_to_buffer(&VSI_line_2_VBO, line_vertices, sizeof(line_vertices));

    //Pointer icon

    const float p_thickness = 3.0f;

    float p_x1 = -VSI_CURSOR_LENGTH/2.0f;
    float p_x2 = p_x1 + p_thickness;
    float p_x3 = VSI_CURSOR_LENGTH/2.0f;

    float p_y1 = -p_thickness/2;
    float p_y2 = 0.0f;
    float p_y3 = p_thickness/2;

    float pointer_vertices[] = {
        p_x1, p_y2, 0.0f,
        p_x2, p_y1, 0.0f,
        p_x3, p_y1, 0.0f,
        p_x3, p_y3, 0.0f,
        p_x2, p_y3, 0.0f,
        p_x1, p_y2, 0.0f
    };

    add_to_buffer(&VSI_pointer_VBO, pointer_vertices, sizeof(pointer_vertices));
}


void draw_VSI(float vPosX, float vPosY) {
    //Setting stencil
    
    enable_stencil_test();
    
    float vsiPosX = vPosX;
    float vsiPosY = vPosY;

    reset_modelmatrix();
    set_position(vsiPosX, vsiPosY);
    set_color((vec4) {0.0,0.0,0.0,VSI_ALPHA});
    draw(VSI_VBO, 7, GL_TRIANGLE_FAN);

    disable_stencil_buffer_changes();
    pass_stencil_if_1();

    //Drawing VSI content

    set_color(WHITE);

    float base_posY = vsiPosY - (VSI_HEIGHT - VSI_LINE_MARGIN*2.0f - VSI_STROKE_THICKNESS*2.0f)/2.0f;
    float space_between_lines = (VSI_HEIGHT - VSI_LINE_MARGIN*2.0f - VSI_STROKE_THICKNESS*2.0f)/12.0f;

    for (int i = 0; i <= 12; i++) {
        reset_modelmatrix();
        
        if (i%2 == 0) {
            float lineXPos = vsiPosX - VSI_WIDTH/2.0f + VSI_LINE_WIDTH_BIG/2.0f;
            set_position(lineXPos, base_posY);
            glBindBuffer(GL_ARRAY_BUFFER, VSI_line_1_VBO);
            draw(VSI_line_1_VBO, 6, GL_TRIANGLES);
        } else {
            float lineXPos = vsiPosX - VSI_WIDTH/2.0f + VSI_LINE_WIDTH_SMALL/2.0f;
            set_position(lineXPos, base_posY);
            draw(VSI_line_2_VBO, 6, GL_TRIANGLES);
        }

        if (i != 6 && i%2 == 0) {
            reset_modelmatrix();
            set_position(vsiPosX, base_posY);
            
            char digit_char[2];
            sprintf(digit_char, "%d", abs(6 - i));
            render_text(digit_char,VSI_NUMBER_SIZE,0);
        }

        base_posY += space_between_lines;
    }

    //Cursor

    //Vertical speed in 100 meters per second
    float vert_speed = -speed[1]*KIL_PER_UNIT*10;

    reset_modelmatrix();
    set_position(
        vsiPosX + (-VSI_WIDTH + VSI_CURSOR_LENGTH)/2,
        vsiPosY + vert_speed*space_between_lines
    );
    set_color(YELLOW);
    draw(VSI_pointer_VBO, 6, GL_TRIANGLE_FAN);

    //Border

    pass_stencil_if_0();

    reset_modelmatrix();
    set_position(vsiPosX, vsiPosY);
    resize(
        (VSI_WIDTH + VSI_STROKE_THICKNESS*2)/VSI_WIDTH,
        (VSI_HEIGHT + VSI_STROKE_THICKNESS*2)/VSI_HEIGHT
    );

    set_color(BLACK);

    draw(VSI_VBO, 7, GL_TRIANGLE_FAN);

    disable_stencil_test();
}

#include <stdio.h>

#include "airspeed.h"

float inner_width = 
    AIRSPEED_WIDTH - AIRSPEED_STROKE_THICKNESS*2;
float inner_height = 
    AIRSPEED_HEIGHT - AIRSPEED_STROKE_THICKNESS*2 - AIRSPEED_INFO_MARGIN_HEIGHT*2;

void gen_airspeed() {
    float inner_box_vertices[18];

    gen_rectangle(
        inner_width,
        inner_height,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        inner_box_vertices
    );
    
    add_to_buffer(&airspeed_inner_VBO, inner_box_vertices, sizeof(inner_box_vertices));

    float outer_box_vertices[27];

    gen_octagon(
        AIRSPEED_WIDTH,
        AIRSPEED_HEIGHT,
        AIRSPEED_CORNER_CATET,
        (vec2){0.0f,0.0f},
        outer_box_vertices
    );

    add_to_buffer(&airspeed_outer_VBO, outer_box_vertices, sizeof(outer_box_vertices));

    //Lines

    float rect_vertices[18];

    gen_rectangle(
        AIRSPEED_LINE_WIDTH_BIG,
        AIRSPEED_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices
    );

    add_to_buffer(&airspeed_line_1_VBO, rect_vertices, sizeof(rect_vertices));

    gen_rectangle(
        AIRSPEED_LINE_WIDTH_SMALL,
        AIRSPEED_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices
    );

    add_to_buffer(&airspeed_line_2_VBO, rect_vertices, sizeof(rect_vertices));

    //Pointer

    float pointer_x1 = -AIRSPEED_POINTER_WIDTH;
    float pointer_x2 = pointer_x1 + AIRSPEED_POINTER_CATET;
    float pointer_x3 = - AIRSPEED_POINTER_TRIANGLE;
    float pointer_x4 = -AIRSPEED_STROKE_THICKNESS;

    float pointer_y1 = - AIRSPEED_POINTER_HEIGHT/2.0f;
    float pointer_y2 = pointer_y1 + AIRSPEED_POINTER_CATET;
    float pointer_y3 = 0.0;
    float pointer_y5 = AIRSPEED_POINTER_HEIGHT/2.0f;
    float pointer_y4 = pointer_y5 - AIRSPEED_POINTER_CATET;

    float pointer_vertices[] = {
        pointer_x4, pointer_y3, 0.0,
        pointer_x3, pointer_y1, 0.0,
        pointer_x2, pointer_y1, 0.0,
        pointer_x1, pointer_y2, 0.0,
        pointer_x1, pointer_y4, 0.0,
        pointer_x2, pointer_y5, 0.0,
        pointer_x3, pointer_y5, 0.0,
        pointer_x4, pointer_y3, 0.0
    };

    add_to_buffer(&airspeed_pointer_VBO, pointer_vertices, sizeof(pointer_vertices));
}

void draw_airspeed(float vPosX, float vPosY) {
    //Uniforms

    reset_modelmatrix();
    set_position(vPosX, vPosY);


    set_color((vec4){0.0f,0.0f,0.0f,AIRSPEED_ALPHA});

    //Drawing inner box

    //Setting stencil
    enable_stencil_test();
    enable_stencil_buffer_changes();

    draw(airspeed_inner_VBO,6,GL_TRIANGLES);

    disable_stencil_buffer_changes();
    pass_stencil_if_1();

    //Drawing unit lines

    //float speed = glm_vec3_distance(pos,prev_pos);

    float airspeed = glm_vec3_norm(speed);
    float knots = 1.94384449f*airspeed*1000;

    //printf("Knots: %0.6f\n",knots);

    int num_lines = floor(AIRSPEED_HEIGHT/AIRSPEED_LINE_SPACE_BETWEEN) + 1;

    int last_line_index = floor(knots/5.0f) - num_lines/2;
    float last_line_pos =
        AIRSPEED_LINE_SPACE_BETWEEN * 
        last_line_index - AIRSPEED_LINE_SPACE_BETWEEN*knots/5.f;

    reset_modelmatrix();
    set_position(
        vPosX + inner_width/2.0f,
        vPosY + last_line_pos
    );

    for (int i = last_line_index; i < last_line_index + num_lines; i++) {
        float line_width =
            i%2 == 0 ? AIRSPEED_LINE_WIDTH_BIG : AIRSPEED_LINE_WIDTH_SMALL;

        set_position(-line_width/2.0f, 0.0f);

        set_color(WHITE);
        draw(
            i%2 == 0 ? airspeed_line_1_VBO : airspeed_line_2_VBO,
            6,GL_TRIANGLES
        );

        set_position(line_width/2.0f, 0.0f);

        if (i%2 == 0) {
            set_position(-inner_width/2, 0.0);

            char num_string[100];
            sprintf(num_string, "%d", i*5);
            render_text(num_string,AIRSPEED_NUMBER_SIZE,0);

            set_position(inner_width/2, 0.0);
        }

        set_position(0.0f, AIRSPEED_LINE_SPACE_BETWEEN);
    }

    //Outer box

    reset_modelmatrix();
    set_position(vPosX, vPosY);

    pass_stencil_if_0();

    color[3] = 1.0f;

    set_color(BLACK);

    draw(airspeed_outer_VBO, 9, GL_TRIANGLE_FAN);

    disable_stencil_test();

    //Drawing TAS and GS

    float info_margin = AIRSPEED_STROKE_THICKNESS*2;

    reset_modelmatrix();
    set_position(
        vPosX - AIRSPEED_WIDTH/2.0 + info_margin,
        vPosY + 
            AIRSPEED_HEIGHT/2.0 - AIRSPEED_INFO_MARGIN_HEIGHT/2.0 - AIRSPEED_STROKE_THICKNESS
    );

    set_color(WHITE);

    render_text("TAS",0.3f,1);

    set_position(AIRSPEED_WIDTH - info_margin*2, 0.0);

    int tas = floor(knots*sqrt(1.225/sl_density));

    char num_string[100];
    sprintf(num_string, "%dkt", tas);
    render_text(num_string,AIRSPEED_NUMBER_SIZE, 2);

    reset_modelmatrix();
    set_position(
        vPosX - AIRSPEED_WIDTH/2.0 + info_margin,
        vPosY -
            AIRSPEED_HEIGHT/2.0 + AIRSPEED_INFO_MARGIN_HEIGHT/2.0 + AIRSPEED_STROKE_THICKNESS
    );

    render_text("GS",0.3f,1);

    set_position(AIRSPEED_WIDTH - info_margin*2,0.0);

    sprintf(num_string, "%dkt", (int)floor(knots*cos(rotX)));
    render_text(num_string,AIRSPEED_NUMBER_SIZE, 2);

    //Marker

    reset_modelmatrix();
    set_position(vPosX + AIRSPEED_WIDTH/2.0, vPosY);

    set_color(BLACK);

    draw(airspeed_pointer_VBO, 8, GL_TRIANGLE_FAN);

    //Reduced marker for stroke and stencil

    resize(
        (AIRSPEED_POINTER_WIDTH-AIRSPEED_STROKE_THICKNESS*2)/AIRSPEED_POINTER_WIDTH,
        (AIRSPEED_POINTER_HEIGHT-AIRSPEED_STROKE_THICKNESS*2)/AIRSPEED_POINTER_HEIGHT
    );

    //Setting stencil

    enable_stencil_test();
    enable_stencil_buffer_changes();


    draw(airspeed_pointer_VBO, 8, GL_TRIANGLE_FAN);

    pass_stencil_if_1();

    //Rolling numbers

    set_color(WHITE);

    int curr_digit = (int)floor(knots)%10;
    int next_digit = (curr_digit+1)%10;
    float phase = mod(knots,1.0f);

    for (int i = 1; i <= 3; i++) {
        reset_modelmatrix();
        set_position(
            vPosX + AIRSPEED_POINTER_DIGIT_HOR_SPACE*(2 - i),
            vPosY - AIRSPEED_POINTER_DIGIT_VERT_SPACE * phase
        );

        char digit_char[2];

        sprintf(digit_char, "%d", curr_digit);
        render_text(digit_char,AIRSPEED_POINTER_DIGIT_SIZE,0);
        set_position(0.0, AIRSPEED_POINTER_DIGIT_VERT_SPACE);

        sprintf(digit_char, "%d", next_digit);

        render_text(digit_char,AIRSPEED_POINTER_DIGIT_SIZE,0);

        phase = mod(knots,10.0f);
        phase = phase > 9.0f && curr_digit == 9 ? (phase-9.0f) : 0.0f;
        curr_digit = ((int)floor(knots)/((int)pow(10,i)))%10;
        next_digit = (curr_digit + 1)%10;
    }

    disable_stencil_test();
}
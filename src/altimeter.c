#include <stdio.h>

#include "altimeter.h"

void gen_altimeter() {
    float inner_width = 
        ALTIMETER_WIDTH - ALTIMETER_STROKE_THICKNESS*2;
    float inner_height = 
        ALTIMETER_HEIGHT - ALTIMETER_STROKE_THICKNESS*2 - ALTIMETER_INFO_MARGIN_HEIGHT*2;

    float inner_box_vertices[18];

    gen_rectangle(
        inner_width,
        inner_height,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        inner_box_vertices
    );
    
    add_to_buffer(&altimeter_inner_VBO, inner_box_vertices, sizeof(inner_box_vertices));

    float outer_box_vertices[27];

    gen_octagon(
        ALTIMETER_WIDTH,
        ALTIMETER_HEIGHT,
        ALTIMETER_CORNER_CATET,
        (vec2){0.0f,0.0f},
        outer_box_vertices
    );

    add_to_buffer(&altimeter_outer_VBO, outer_box_vertices, sizeof(outer_box_vertices));

    //Lines

    float rect_vertices[18];

    gen_rectangle(
        ALTIMETER_LINE_WIDTH_BIG,
        ALTIMETER_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices
    );

    add_to_buffer(&altimeter_line_1_VBO, rect_vertices, sizeof(rect_vertices));

    gen_rectangle(
        ALTIMETER_LINE_WIDTH_SMALL,
        ALTIMETER_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices
    );

    glGenBuffers(1, &altimeter_line_2_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_line_2_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

    add_to_buffer(&altimeter_line_2_VBO, rect_vertices, sizeof(rect_vertices));

    //Pointer

    float pointer_x1 = ALTIMETER_STROKE_THICKNESS;
    float pointer_x2 = ALTIMETER_POINTER_TRIANGLE;
    float pointer_x4 = ALTIMETER_POINTER_WIDTH;
    float pointer_x3 = pointer_x4 - ALTIMETER_POINTER_CATET;

    float pointer_y1 = - ALTIMETER_POINTER_HEIGHT/2.0f;
    float pointer_y2 = pointer_y1 + ALTIMETER_POINTER_CATET;
    float pointer_y3 = 0.0;
    float pointer_y5 = ALTIMETER_POINTER_HEIGHT/2.0f;
    float pointer_y4 = pointer_y5 - ALTIMETER_POINTER_CATET;

    float pointer_vertices[] = {
        pointer_x1, pointer_y3, 0.0,
        pointer_x2, pointer_y5, 0.0,
        pointer_x3, pointer_y5, 0.0,
        pointer_x4, pointer_y4, 0.0,
        pointer_x4, pointer_y2, 0.0,
        pointer_x3, pointer_y1, 0.0,
        pointer_x2, pointer_y1, 0.0,
        pointer_x1, pointer_y3, 0.0
    };
    
    add_to_buffer(&altimeter_pointer_VBO, pointer_vertices, sizeof(pointer_vertices));
}

void draw_altimeter(float vPosX, float vPosY) {
    //Uniforms

    reset_modelmatrix();
    set_position(vPosX, vPosY);

    set_color((vec4){0.0f,0.0f,0.0f,ALTIMETER_ALPHA});

    //Drawing inner box

    //Setting stencil
    enable_stencil_test();
    enable_stencil_buffer_changes();

    glBindBuffer(GL_ARRAY_BUFFER, altimeter_inner_VBO);
   
    draw(altimeter_inner_VBO, 6, GL_TRIANGLES);

    //Drawing unit lines

    pass_stencil_if_1();

    float inner_width = 
    ALTIMETER_WIDTH - ALTIMETER_STROKE_THICKNESS*2;

    float altitude = pos[1]*1000.0f;

    int num_lines = floor(ALTIMETER_HEIGHT/ALTIMETER_LINE_SPACE_BETWEEN) + 1;
    float meters_unit = 100.0f;

    int last_line_index = floor(altitude/meters_unit) - num_lines/2;
    float last_line_pos = vPosY + 
        ALTIMETER_LINE_SPACE_BETWEEN * 
        last_line_index - ALTIMETER_LINE_SPACE_BETWEEN*altitude/meters_unit;

    reset_modelmatrix();
    set_position(vPosX - inner_width/2.0f, last_line_pos);
    set_color(WHITE);

    for (int i = last_line_index; i < last_line_index + num_lines; i++) {
        float line_width =
            i%2 == 0 ? ALTIMETER_LINE_WIDTH_BIG : ALTIMETER_LINE_WIDTH_SMALL;

        set_position(line_width/2.0f, 0.0);
        draw(i%2 == 0 ? altimeter_line_1_VBO : altimeter_line_2_VBO, 6, GL_TRIANGLES);

        set_position(-line_width/2.0f, 0.0);

        if (i%4 == 0) {
            set_position(inner_width/2.0, 0.0);

            char num_string[100];
            sprintf(num_string, "%d", i*100);
            render_text(num_string,ALTIMETER_NUMBER_SIZE,0);

            set_position(-inner_width/2.0, 0.0f);
        }

        set_position(0.0f, ALTIMETER_LINE_SPACE_BETWEEN);
    }

    //Drawing outer box

    reset_modelmatrix();
    set_position(vPosX, vPosY);
    pass_stencil_if_0();
    set_color(BLACK);
    draw(altimeter_outer_VBO, 9, GL_TRIANGLE_FAN);

    disable_stencil_test();

    //Drawing ALT and PR

    reset_modelmatrix();
    set_position(
        vPosX,
        vPosY + ALTIMETER_HEIGHT/2.0
            - ALTIMETER_INFO_MARGIN_HEIGHT/2.0 - ALTIMETER_STROKE_THICKNESS);

    set_color(WHITE);

    char num_string[100];
    sprintf(num_string, "%dm", (int)floor(altitude));
    render_text(num_string,ALTIMETER_NUMBER_SIZE, 0);

    reset_modelmatrix();
    set_position(
        vPosX,
        vPosY -
            ALTIMETER_HEIGHT/2.0 + ALTIMETER_INFO_MARGIN_HEIGHT/2.0 + ALTIMETER_STROKE_THICKNESS
    );

    float airPressure = 1013.25f* (1 - (0.0065f*altitude)/288.15f);

    sprintf(num_string, "%0.2fhPa", airPressure);
    render_text(num_string,ALTIMETER_NUMBER_SIZE, 0);

    //Marker

    reset_modelmatrix();
    set_position(
        vPosX - ALTIMETER_WIDTH/2.0,
        vPosY);

    set_color(BLACK);

    draw(altimeter_pointer_VBO, 8, GL_TRIANGLE_FAN);

    //Reduced marker for stroke and stencil

    resize(
        (ALTIMETER_POINTER_WIDTH-ALTIMETER_STROKE_THICKNESS*2)/ALTIMETER_POINTER_WIDTH,
        (ALTIMETER_POINTER_HEIGHT-ALTIMETER_STROKE_THICKNESS*2)/ALTIMETER_POINTER_HEIGHT
    );

    //Setting stencil

    enable_stencil_test();
    enable_stencil_buffer_changes();

    draw(altimeter_pointer_VBO, 8, GL_TRIANGLE_FAN);

    pass_stencil_if_1();

    //Rolling numbers
    
    set_color(WHITE);
    
    int altit_unit = (int)floor(altitude/10);
    int curr_digit = altit_unit%10 - altit_unit%2;
    int next_num_digit = (curr_digit + 2)%10;
    float phase = mod(altitude,20.0f)/20.0f;
    
    //First digit

    float digit_posX = vPosX + ALTIMETER_POINTER_DIGIT_HOR_SPACE*2.5f;
    float digit_posY = vPosY - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    render_text("0\0",ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;
    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    render_text("0\0",ALTIMETER_POINTER_DIGIT_SIZE,0);

    //Second digit

    digit_posX -= ALTIMETER_POINTER_DIGIT_HOR_SPACE;
    digit_posY -= ALTIMETER_POINTER_DIGIT_VERT_SPACE;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    char digit_char[2];
    sprintf(digit_char, "%d", curr_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;
    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", next_num_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    //Third digit

    altit_unit /= 10;
    curr_digit = altit_unit%10;
    next_num_digit = (curr_digit + 1)%10;

    phase = 100.0f - mod(altitude,100) <= 20 ? mod(altitude,20.0f)/20.0f : 0.0f;

    digit_posX -= ALTIMETER_POINTER_DIGIT_HOR_SPACE;
    digit_posY = vPosY - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", curr_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", next_num_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    //Forth digit

    altit_unit /= 10;
    curr_digit = altit_unit%10;
    next_num_digit = (curr_digit + 1)%10;

    phase = 1000.0f - mod(altitude,1000) <= 20 ? mod(altitude,20.0f)/20.0f : 0.0f;

    digit_posX -= ALTIMETER_POINTER_DIGIT_HOR_SPACE;
    digit_posY = vPosY - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", curr_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", next_num_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    //Fifth digit

    altit_unit /= 10;
    curr_digit = altit_unit%10;
    next_num_digit = (curr_digit + 1)%10;

    phase = 10000.0f - mod(altitude,10000) <= 20 ? mod(altitude,20.0f)/20.0f : 0.0f;

    digit_posX -= ALTIMETER_POINTER_DIGIT_HOR_SPACE;
    digit_posY = vPosY- ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", curr_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;
    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    disable_stencil_test();
}
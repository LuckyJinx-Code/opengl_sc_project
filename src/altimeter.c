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
    
    glGenBuffers(1, &altimeter_inner_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_inner_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(inner_box_vertices), inner_box_vertices, GL_STATIC_DRAW);

    float outer_box_vertices[27];

    gen_octagon(
        ALTIMETER_WIDTH,
        ALTIMETER_HEIGHT,
        ALTIMETER_CORNER_CATET,
        (vec2){0.0f,0.0f},
        outer_box_vertices
    );

    glGenBuffers(1, &altimeter_outer_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_outer_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(outer_box_vertices), outer_box_vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &altimeter_line_1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_line_1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &altimeter_pointer_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_pointer_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointer_vertices), pointer_vertices, GL_STATIC_DRAW);
}

void draw_altimeter() {
    //Uniforms

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0 - ALTIMETER_RIGHT_MARGIN,
        WINDOW_HEIGHT/2.0 - ALTIMETER_TOP_MARGIN,
        0.0
    });

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    glUniform4fv(colorLoc,1, (vec4){0.0f,0.0f,0.0f,ALTIMETER_ALPHA});
    glUniform1i(setBrightnessLoc, 0);
    glUniform1i(hasTextureLoc, 0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);

    //Drawing inner box

    //Setting stencil
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    glBindBuffer(GL_ARRAY_BUFFER, altimeter_inner_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Drawing unit lines

    glStencilMask(0x00); // Do not write to stencil buffer    
    glStencilFunc(GL_EQUAL, 1, 0xFF);

    float inner_width = 
    ALTIMETER_WIDTH - ALTIMETER_STROKE_THICKNESS*2;

    float altitude = posY*1000.0f;

    float box_pos_y = WINDOW_HEIGHT/2.0 - ALTIMETER_TOP_MARGIN;

    int num_lines = floor(ALTIMETER_HEIGHT/ALTIMETER_LINE_SPACE_BETWEEN) + 1;
    float meters_unit = 100.0f;

    int last_line_index = floor(altitude/meters_unit) - num_lines/2;
    float last_line_pos = box_pos_y + 
        ALTIMETER_LINE_SPACE_BETWEEN * 
        last_line_index - ALTIMETER_LINE_SPACE_BETWEEN*altitude/meters_unit;

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0f - ALTIMETER_RIGHT_MARGIN - inner_width/2.0f,
        last_line_pos,
        0.0
    });

    for (int i = last_line_index; i < last_line_index + num_lines; i++) {
        float line_width =
            i%2 == 0 ? ALTIMETER_LINE_WIDTH_BIG : ALTIMETER_LINE_WIDTH_SMALL;

        glm_translate(modelMatrix,(vec3){
            line_width/2.0f,
            0.0,
            0.0
        });

        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
        glUniform4fv(colorLoc,1, WHITE);
        glBindBuffer(GL_ARRAY_BUFFER, i%2 == 0 ? altimeter_line_1_VBO : altimeter_line_2_VBO);
        glEnableVertexAttribArray(POS_ATTR);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glm_translate(modelMatrix,(vec3){
            -line_width/2.0f,
            0.0,
            0.0
        });

        if (i%4 == 0) {
            glm_translate(modelMatrix,(vec3){
                inner_width/2.0,
                0.0,
                0.0
            });
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

            glUniform1i(hasTextureLoc,1);

            char num_string[100];
            sprintf(num_string, "%d", i*100);
            render_text(num_string,ALTIMETER_NUMBER_SIZE,0);

            glUniform1i(hasTextureLoc,0);

            glm_translate(modelMatrix,(vec3){
                -inner_width/2.0,
                0.0,
                0.0
            });
        }

        glm_translate(modelMatrix,(vec3){
            0.0f,
            ALTIMETER_LINE_SPACE_BETWEEN,
            0.0f
        });
    }

    //Drawing outer box

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0 - ALTIMETER_RIGHT_MARGIN,
        WINDOW_HEIGHT/2.0 - ALTIMETER_TOP_MARGIN,
        0.0
    });
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE); 

    color[3] = 1.0f;

    glUniform4fv(colorLoc,1, BLACK);
    
    glBindBuffer(GL_ARRAY_BUFFER, altimeter_outer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniform1i(hasTextureLoc,1);
    glDisable(GL_STENCIL_TEST);

    //Drawing ALT and PR

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0 - ALTIMETER_RIGHT_MARGIN,
        WINDOW_HEIGHT/2.0 - ALTIMETER_TOP_MARGIN + ALTIMETER_HEIGHT/2.0
            - ALTIMETER_INFO_MARGIN_HEIGHT/2.0 - ALTIMETER_STROKE_THICKNESS,
        0.0
    });

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, (vec4){1.0,1.0,1.0,1.0});
    glUniform1i(hasTextureLoc,1);

    char num_string[100];
    sprintf(num_string, "%dm", (int)floor(altitude));
    render_text(num_string,ALTIMETER_NUMBER_SIZE, 0);

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0 - ALTIMETER_RIGHT_MARGIN,
        WINDOW_HEIGHT/2.0 - ALTIMETER_TOP_MARGIN -
            ALTIMETER_HEIGHT/2.0 + ALTIMETER_INFO_MARGIN_HEIGHT/2.0 + ALTIMETER_STROKE_THICKNESS,
        0.0
    });
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    float airPressure = 1013.25f* (1 - (0.0065f*altitude)/288.15f);

    sprintf(num_string, "%0.2fhPa", airPressure);
    render_text(num_string,ALTIMETER_NUMBER_SIZE, 0);

    //Marker

    glUniform1i(hasTextureLoc,0);

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        WINDOW_WIDTH/2.0f - ALTIMETER_RIGHT_MARGIN - ALTIMETER_WIDTH/2.0,
        WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN,
        0.0
    });

    glUniform1i(hasTextureLoc,0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, (vec4){0.0,0.0,0.0,1.0});

    glBindBuffer(GL_ARRAY_BUFFER, altimeter_pointer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Reduced marker for stroke and stencil

    glm_scale(modelMatrix,(vec3){
        (ALTIMETER_POINTER_WIDTH-ALTIMETER_STROKE_THICKNESS*2)/ALTIMETER_POINTER_WIDTH,
        (ALTIMETER_POINTER_HEIGHT-ALTIMETER_STROKE_THICKNESS*2)/ALTIMETER_POINTER_HEIGHT,
        0.0
    });

    //Setting stencil

    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    glBindBuffer(GL_ARRAY_BUFFER, altimeter_pointer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);

    //Rolling numbers
    
    glUniform1i(hasTextureLoc,1);
    glUniform4fv(colorLoc,1, WHITE);
    
    int altit_unit = (int)floor(altitude/10);
    int curr_digit = altit_unit%10 - altit_unit%2;
    int next_num_digit = (curr_digit + 2)%10;
    float phase = mod(altitude,20.0f)/20.0f;
    
    //First digit

    float digit_posX = WINDOW_WIDTH/2.0f - ALTIMETER_RIGHT_MARGIN + ALTIMETER_POINTER_DIGIT_HOR_SPACE*2.5f;
    float digit_posY = WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

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
    digit_posY = WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

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
    digit_posY = WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

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
    digit_posY = WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN - ALTIMETER_POINTER_DIGIT_VERT_SPACE * phase;

    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    sprintf(digit_char, "%d", curr_digit);
    render_text(digit_char,ALTIMETER_POINTER_DIGIT_SIZE,0);

    digit_posY += ALTIMETER_POINTER_DIGIT_VERT_SPACE;
    reset_modelmatrix();
    set_position(digit_posX, digit_posY);

    glUniform1i(hasTextureLoc,0);
    glDisable(GL_STENCIL_TEST);
}
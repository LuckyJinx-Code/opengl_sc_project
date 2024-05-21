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

    glGenBuffers(1, &VSI_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VSI_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(box_vertices), box_vertices, GL_STATIC_DRAW);

    float line_vertices[18];
    
    gen_rectangle(
        VSI_LINE_WIDTH_BIG,
        VSI_LINE_THICKNESS,
        (vec2){0.0f,0.0f},
        0.0f,
        0.0f,
        line_vertices
    );

    glGenBuffers(1, &VSI_line_1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VSI_line_1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);

    gen_rectangle(
        VSI_LINE_WIDTH_SMALL,
        VSI_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0f,
        0.0f,
        line_vertices
    );

    glGenBuffers(1, &VSI_line_2_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VSI_line_2_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line_vertices), line_vertices, GL_STATIC_DRAW);
}


void draw_VSI() {
    //Setting stencil
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
    
    float vsiPosX = WINDOW_WIDTH/2.0f - ALTIMETER_RIGHT_MARGIN + ALTIMETER_WIDTH/2.0f + VSI_WIDTH/2.0f - VSI_STROKE_THICKNESS;
    float vsiPosY = WINDOW_HEIGHT/2.0f - ALTIMETER_TOP_MARGIN;

    reset_modelmatrix();
    set_position(vsiPosX, vsiPosY);
    set_figure_size(
        (vec2){VSI_WIDTH,VSI_HEIGHT},
        (vec2){VSI_WIDTH - VSI_STROKE_THICKNESS*2,VSI_HEIGHT - VSI_STROKE_THICKNESS*2}
    );
    
    glUniform4fv(colorLoc,1, (vec4) {0.0,0.0,0.0,VSI_ALPHA});

    glBindBuffer(GL_ARRAY_BUFFER, VSI_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);

    glStencilMask(0x00); // Do not write to stencil buffer

    //Drawing VSI content

    glStencilFunc(GL_EQUAL, 1, 0xFF);

    glUniform4fv(colorLoc,1, WHITE);
    glBindBuffer(GL_ARRAY_BUFFER, VSI_line_1_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    float base_posY = vsiPosY - (VSI_HEIGHT - VSI_LINE_MARGIN*2.0f - VSI_STROKE_THICKNESS*2.0f)/2.0f;
    float space_between_lines = (VSI_HEIGHT - VSI_LINE_MARGIN*2.0f - VSI_STROKE_THICKNESS*2.0f)/12.0f;

    for (int i = 0; i <= 12; i++) {
        reset_modelmatrix();
        
        if (i%2 == 0) {
            float lineXPos = vsiPosX - VSI_WIDTH/2.0f + VSI_LINE_WIDTH_BIG/2.0f;
            set_position(lineXPos, base_posY);
            glBindBuffer(GL_ARRAY_BUFFER, VSI_line_1_VBO);
        } else {
            float lineXPos = vsiPosX - VSI_WIDTH/2.0f + VSI_LINE_WIDTH_SMALL/2.0f;
            set_position(lineXPos, base_posY);
            glBindBuffer(GL_ARRAY_BUFFER, VSI_line_2_VBO);
        }

        glEnableVertexAttribArray(POS_ATTR);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (i != 6 && i%2 == 0) {
            glUniform1i(hasTextureLoc,1);
            reset_modelmatrix();
            set_position(vsiPosX, base_posY);
            
            char digit_char[2];
            sprintf(digit_char, "%d", abs(6 - i));
            render_text(digit_char,VSI_NUMBER_SIZE,0);

            glUniform1i(hasTextureLoc,0);
        }

        base_posY += space_between_lines;
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    reset_modelmatrix();
    set_position(vsiPosX, vsiPosY);

    glUniform4fv(colorLoc,1, BLACK);

    glBindBuffer(GL_ARRAY_BUFFER, VSI_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 7);

    glDisable(GL_STENCIL_TEST);
}

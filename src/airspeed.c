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
    
    glGenBuffers(1, &airspeed_inner_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, airspeed_inner_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(inner_box_vertices), inner_box_vertices, GL_STATIC_DRAW);

    float outer_box_vertices[27];

    gen_octagon(
        AIRSPEED_WIDTH,
        AIRSPEED_HEIGHT,
        AIRSPEED_CORNER_CATET,
        (vec2){0.0f,0.0f},
        outer_box_vertices
    );

    glGenBuffers(1, &airspeed_outer_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, airspeed_outer_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(outer_box_vertices), outer_box_vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &airspeed_line_1_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, airspeed_line_1_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

    gen_rectangle(
        AIRSPEED_LINE_WIDTH_SMALL,
        AIRSPEED_LINE_THICKNESS,
        (vec2){0.0,0.0},
        0.0,
        0.0,
        rect_vertices
    );

    glGenBuffers(1, &airspeed_line_2_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, airspeed_line_2_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &airspeed_pointer_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, airspeed_pointer_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointer_vertices), pointer_vertices, GL_STATIC_DRAW);
}

void draw_airspeed() {
    //Uniforms

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0 + AIRSPEED_LEFT_MARGIN,
        WINDOW_HEIGHT/2.0 - AIRSPEED_TOP_MARGIN,
        0.0
    });

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    glUniform4fv(colorLoc,1, (vec4){0.0f,0.0f,0.0f,AIRSPEED_ALPHA});
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

    glBindBuffer(GL_ARRAY_BUFFER, airspeed_inner_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glStencilMask(0x00); // Do not write to stencil buffer
    
    glStencilFunc(GL_EQUAL, 1, 0xFF);

    //Drawing unit lines

    float knots = 1.94384449f*speed*1000.0f;

    float box_pos_y = WINDOW_HEIGHT/2.0 - AIRSPEED_TOP_MARGIN;

    int num_lines = floor(AIRSPEED_HEIGHT/AIRSPEED_LINE_SPACE_BETWEEN) + 1;

    int last_line_index = floor(knots/5.0f) - num_lines/2;
    float last_line_pos = box_pos_y + 
        AIRSPEED_LINE_SPACE_BETWEEN * 
        last_line_index - AIRSPEED_LINE_SPACE_BETWEEN*knots/5.f;

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0f + AIRSPEED_LEFT_MARGIN + inner_width/2.0f,
        last_line_pos,
        0.0
    });

    for (int i = last_line_index; i < last_line_index + num_lines; i++) {
        float line_width =
            i%2 == 0 ? AIRSPEED_LINE_WIDTH_BIG : AIRSPEED_LINE_WIDTH_SMALL;

        glm_translate(modelMatrix,(vec3){
            -line_width/2.0f,
            0.0,
            0.0
        });

        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
        glUniform4fv(colorLoc,1, WHITE);
        glBindBuffer(GL_ARRAY_BUFFER, i%2 == 0 ? airspeed_line_1_VBO : airspeed_line_2_VBO);
        glEnableVertexAttribArray(POS_ATTR);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glm_translate(modelMatrix,(vec3){
            line_width/2.0f,
            0.0,
            0.0
        });

        if (i%2 == 0) {
            glm_translate(modelMatrix,(vec3){
                - inner_width/2.0,
                0.0,
                0.0
            });
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

            glUniform1i(hasTextureLoc,1);

            char num_string[100];
            sprintf(num_string, "%d", i*5);
            render_text(num_string,AIRSPEED_NUMBER_SIZE,0);

            glUniform1i(hasTextureLoc,0);

            glm_translate(modelMatrix,(vec3){
                inner_width/2.0,
                0.0,
                0.0
            });
        }

        glm_translate(modelMatrix,(vec3){
            0.0f,
            AIRSPEED_LINE_SPACE_BETWEEN,
            0.0f
        });
    }

    //Outer box

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0 + AIRSPEED_LEFT_MARGIN,
        WINDOW_HEIGHT/2.0 - AIRSPEED_TOP_MARGIN,
        0.0
    });
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE); 

    color[3] = 1.0f;

    glUniform4fv(colorLoc,1, BLACK);

    glBindBuffer(GL_ARRAY_BUFFER, airspeed_outer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 9);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDisable(GL_STENCIL_TEST);

    //Drawing TAS and GS

    float info_margin = AIRSPEED_STROKE_THICKNESS*2;

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0 + AIRSPEED_LEFT_MARGIN - AIRSPEED_WIDTH/2.0 + info_margin,
        WINDOW_HEIGHT/2.0 - AIRSPEED_TOP_MARGIN + 
            AIRSPEED_HEIGHT/2.0 - AIRSPEED_INFO_MARGIN_HEIGHT/2.0 - AIRSPEED_STROKE_THICKNESS,
        0.0
    });

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, (vec4){1.0,1.0,1.0,1.0});
    glUniform1i(hasTextureLoc,1);

    render_text("TAS",0.3f,1);

    glm_translate(modelMatrix,(vec3){
        AIRSPEED_WIDTH - info_margin*2,
        0.0,
        0.0
    });
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    int tas = floor(knots*sqrt(1.225/sl_density));

    char num_string[100];
    sprintf(num_string, "%dkt", tas);
    render_text(num_string,AIRSPEED_NUMBER_SIZE, 2);

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0 + AIRSPEED_LEFT_MARGIN - AIRSPEED_WIDTH/2.0 + info_margin,
        WINDOW_HEIGHT/2.0 - AIRSPEED_TOP_MARGIN -
            AIRSPEED_HEIGHT/2.0 + AIRSPEED_INFO_MARGIN_HEIGHT/2.0 + AIRSPEED_STROKE_THICKNESS,
        0.0
    });

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    render_text("GS",0.3f,1);

    glm_translate(modelMatrix,(vec3){
        AIRSPEED_WIDTH - info_margin*2,
        0.0,
        0.0
    });
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    sprintf(num_string, "%dkt", (int)floor(knots*cos(rotX)));
    render_text(num_string,AIRSPEED_NUMBER_SIZE, 2);

    //Marker

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){
        -WINDOW_WIDTH/2.0f + AIRSPEED_LEFT_MARGIN + AIRSPEED_WIDTH/2.0,
        WINDOW_HEIGHT/2.0f - AIRSPEED_TOP_MARGIN,
        0.0
    });

    glUniform1i(hasTextureLoc,0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, (vec4){0.0,0.0,0.0,1.0});

    glBindBuffer(GL_ARRAY_BUFFER, airspeed_pointer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Reduced marker for stroke and stencil

    glm_scale(modelMatrix,(vec3){
        (AIRSPEED_POINTER_WIDTH-AIRSPEED_STROKE_THICKNESS*2)/AIRSPEED_POINTER_WIDTH,
        (AIRSPEED_POINTER_HEIGHT-AIRSPEED_STROKE_THICKNESS*2)/AIRSPEED_POINTER_HEIGHT,
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

    glBindBuffer(GL_ARRAY_BUFFER, airspeed_pointer_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);

    //Rolling numbers

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform1i(hasTextureLoc,1);
    glUniform4fv(colorLoc,1, WHITE);

    int curr_digit = (int)floor(knots)%10;
    int next_digit = (curr_digit+1)%10;
    float phase = mod(knots,1.0f);

    for (int i = 1; i <= 3; i++) {
        glm_mat4_identity(modelMatrix);
        glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
        glm_translate(modelMatrix,(vec3){
            -WINDOW_WIDTH/2.0f + AIRSPEED_LEFT_MARGIN + AIRSPEED_POINTER_DIGIT_HOR_SPACE*(2 - i),
            WINDOW_HEIGHT/2.0f - AIRSPEED_TOP_MARGIN - AIRSPEED_POINTER_DIGIT_VERT_SPACE * phase,
            0.0
        });
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

        char digit_char[2];

        sprintf(digit_char, "%d", curr_digit);

        render_text(digit_char,AIRSPEED_POINTER_DIGIT_SIZE,0);

        glm_translate(modelMatrix,(vec3){
            0.0,
            AIRSPEED_POINTER_DIGIT_VERT_SPACE,
            0.0
        });
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

        sprintf(digit_char, "%d", next_digit);

        render_text(digit_char,AIRSPEED_POINTER_DIGIT_SIZE,0);

        phase = mod(knots,10.0f);
        phase = phase > 9.0f && curr_digit == 9 ? (phase-9.0f) : 0.0f;
        curr_digit = ((int)floor(knots)/((int)pow(10,i)))%10;
        next_digit = (curr_digit + 1)%10;
    }

    glUniform1i(hasTextureLoc,1);
    glDisable(GL_STENCIL_TEST);
}
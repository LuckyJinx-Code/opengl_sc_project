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
}

void draw_SSI() {
    //Draw triangle

    float tr_height = sqrt(pow(SSI_TRIANGLE_SIDE/2.0f,2.0f) + pow(SSI_TRIANGLE_SIDE,2.0f));

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,(float)SSI_Y_POS + SSI_RADIUS_1 + tr_height/2.0f,0.0});
    
    glUniform4fv(colorLoc,1, WHITE);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    glBindBuffer(GL_ARRAY_BUFFER, SSI_triangle_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    //Drawing arc and lines

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,(float)SSI_Y_POS,0.0});
    glm_rotate(modelMatrix,-rotZ,(vec3){0.0,0.0,1.0});

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

    glBindBuffer(GL_ARRAY_BUFFER, SSI_arc_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, SSI_ARC_NUM_VERTICES);

    glBindBuffer(GL_ARRAY_BUFFER, SSI_lines_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, SSI_LINES_NUM_VERTICES);
    
    float angle = 2*GLM_PI - rotX;
    if (angle > GLM_PI) angle = -2*GLM_PI + angle; //180,360 range turns to -180,0

    float SSI_height = SSI_PITCH_NUM_LINES*SSI_PITCH_SPACE_BETWEEN;

    float current_pos = (float)SSI_Y_POS - SSI_height/2  + (angle/GLM_PI) * SSI_height/2;

    float SSI_bottom = (float)SSI_Y_POS - (float)SSI_PITCH_HEIGHT/2;
    float SSI_top = (float)SSI_Y_POS + (float)SSI_PITCH_HEIGHT/2;

    float transparency_point1 = SSI_bottom + (float)SSI_PITCH_VANISH_HEIGHT;
    float transparency_point2 = SSI_top - (float)SSI_PITCH_VANISH_HEIGHT;

    for (int i = 0; i < SSI_PITCH_NUM_LINES; i++) {
        glm_mat4_identity(modelMatrix);
        glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
        glm_translate(modelMatrix,(vec3){0.0,SSI_Y_POS,0.0});
        glm_rotate(modelMatrix,-rotZ,(vec3){0.0,0.0,1.0});
        glm_translate(modelMatrix,(vec3){0.0,-current_pos+SSI_Y_POS,0.0});

        float alpha = 1.0;

        if (current_pos < SSI_bottom || current_pos > SSI_top)
            alpha = 0.0;
        else if (current_pos < transparency_point1) {
            alpha = (current_pos - SSI_bottom)/(transparency_point1 - SSI_bottom);
        } else if (current_pos > transparency_point2) {
            alpha = 1.0 - (current_pos - transparency_point2)/(SSI_top - transparency_point2);
        }

        color[3] = alpha;

        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
        glUniform1i(hasTextureLoc,0);
        glUniform4fv(colorLoc,1, &color[0]);

        glBindBuffer(GL_ARRAY_BUFFER, i%2 == 0 ? SSI_pitch_line_1_VBO : SSI_pitch_line_2_VBO);
   
        glEnableVertexAttribArray(POS_ATTR);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (i%2 == 0) {
            int num = abs(-180 + 5*i);
            char num_string[3];

            sprintf(num_string, "%d", num);

            glm_translate(modelMatrix,(vec3){SSI_PITCH_SPACE_NUM + (float)SSI_PITCH_WIDTH_BIG/2.0,0.0,0.0});
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
            glUniform1i(hasTextureLoc,1);

            render_text(num_string,0.3f,0);

            glm_translate(modelMatrix,(vec3){-SSI_PITCH_SPACE_NUM*2 - (float)SSI_PITCH_WIDTH_BIG,0.0,0.0});
            glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);

            render_text(num_string,0.3f,0);

        }

        current_pos += (float)SSI_PITCH_SPACE_BETWEEN;
    }

    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 0.0;
    color[3] = 1.0;

    float size_with_stroke = ((float)SSI_CENTER_MARK_SIZE+2)/SSI_CENTER_MARK_SIZE;

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,(float)SSI_Y_POS,0.0});
    glm_scale(modelMatrix,(vec3){size_with_stroke,size_with_stroke});

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, &color[0]);

    glBindBuffer(GL_ARRAY_BUFFER, SSI_center_mark_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    color[0] = 1.0;
    color[1] = 250.0/255.0;
    color[2] = 0.0;
    color[3] = 1.0;

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,(float)SSI_Y_POS,0.0});

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniform4fv(colorLoc,1, &color[0]);

    glBindBuffer(GL_ARRAY_BUFFER, SSI_center_mark_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
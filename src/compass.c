#include <stdio.h>

#include "compass.h"

void gen_compass() {
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

    //for (int i = 0; i < 18*COMPASS_NUM_MARKERS/2; i++)
        //printf("Vertice: %.2f\n",vertices[i]);

    glGenBuffers(1, &compass_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, compass_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void draw_compass() {
    //Uniforms

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,MARGIN_BOTTOM-(float)WINDOW_HEIGHT/2.0,0.0});
    glm_rotate(modelMatrix,rotY,(vec3){0.0,0.0,1.0});

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;

    glUniform4fv(colorLoc,1, &color[0]);
    glUniform1i(hasTextureLoc, 0);
    glUniform1i(setBrightnessLoc, 0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);

    //Drawing lines

    glBindBuffer(GL_ARRAY_BUFFER, compass_VBO);
   
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, COMPASS_NUM_VERTICES);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //Drawing units

    glUniform1i(hasTextureLoc, 1);

    char compass_units[][3] = {
        "N\0\0","3\0\0","6\0\0","E\0\0","12\0","15\0","S\0\0","21\0","24\0","W\0\0","30\0","33\0"
    };

    for (int i = 0; i < 12; i++) {
        glm_mat4_identity(modelMatrix);
        glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
        glm_translate(modelMatrix,(vec3){0.0,MARGIN_BOTTOM-(float)WINDOW_HEIGHT/2.0,0.0});
        glm_translate(modelMatrix,(vec3){
            (float)COMPASS_NSWE_RADIUS*cos(glm_rad(90) - 2*GLM_PI*i/12 + rotY),
            (float)COMPASS_NSWE_RADIUS*sin(glm_rad(90) - 2*GLM_PI*i/12 + rotY),
            0.0}
        );
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
        render_text(compass_units[i],i%3 == 0 ? 0.5f : 0.32f,0);
    }

    glUniform1i(hasTextureLoc, 0);
}
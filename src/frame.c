#include "frame.h"

void gen_frame() {
    //Octagonal shape
    float pointX1 = -(float)WINDOW_WIDTH/2.0 + (float)MARGIN_LEFT;
    float pointX2 = pointX1 + (float)CORNER_SQUARE;
    float pointX4 = (float)WINDOW_WIDTH/2.0 - (float)MARGIN_RIGHT;
    float pointX3 = pointX4 - (float)CORNER_SQUARE;

    float pointY1 = -(float)WINDOW_HEIGHT/2.0 + (float)MARGIN_BOTTOM;
    float pointY2 = pointY1 + (float)CORNER_SQUARE;
    float pointY4 = (float)WINDOW_HEIGHT/2.0 - (float)MARGIN_TOP;
    float pointY3 = pointY4 - (float)CORNER_SQUARE;

    float oct_vertices[] = {
        0.0,0.0,0.0,
        pointX1,pointY2, 0.0,
        pointX1,pointY3, 0.0,
        pointX2,pointY4, 0.0,
        pointX3,pointY4, 0.0,
        pointX4,pointY3, 0.0,
        pointX4,pointY2, 0.0,
        pointX3,pointY1, 0.0,
        pointX2,pointY1, 0.0,
        pointX1,pointY2, 0.0
    };

    glGenBuffers(1, &frame_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(oct_vertices), oct_vertices, GL_STATIC_DRAW);

    //Circular shape

    float circle_vertices[3 + FRAME_CIRCLE_SIDES*3];

    gen_arc((float)(COMPASS_RADIUS + COMPASS_MARGIN),(vec2){0.0,pointY1},0.0,GLM_PI,FRAME_CIRCLE_SIDES,circle_vertices);

    glGenBuffers(1, &frame_circle_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, frame_circle_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle_vertices), circle_vertices, GL_STATIC_DRAW);
}

void draw_frame() {

    //Setting uniforms

    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    glUniform4fv(colorLoc,1, SKY_COLOR);
    glUniform1i(setBrightnessLoc, 0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);
    glUniform1i(hasTextureLoc, 0);

    
    // Stencil
    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glDepthMask(GL_FALSE); // Don't write to depth buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glBindBuffer(GL_ARRAY_BUFFER, frame_circle_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, FRAME_CIRCLE_SIDES + 1);
    
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE); 

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    glBindBuffer(GL_ARRAY_BUFFER, frame_VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 10);

    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass only if stencil value is exactly 2
    glStencilMask(0x00); // Don't write anything to stencil buffer
}

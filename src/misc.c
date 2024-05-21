#include "misc.h"

void draw_destination_msg() {
    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glm_translate(modelMatrix,(vec3){0.0,(float)WINDOW_HEIGHT/2.0 - MARGIN_TOP/2.0,0.0});

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    glUniform4fv(colorLoc,1, (vec4){1.0f, 1.0f, 1.0f, 1.0f});
    glUniform1i(setBrightnessLoc, 0);
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);
    glUniform1i(hasTextureLoc, 1);

    render_text("Destination: Barcelona\0",0.5f,0);

    glUniform1i(hasTextureLoc, 0);
}
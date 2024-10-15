#include <stdio.h>
#include "utils.h"

//Load text from file

#define INIT_ALLOC 1024
#define MIN_SPACE   256
#define MAX_EXCESS  256

char *read_file(FILE *fp)
{
    size_t offset = 0;
    size_t bufsiz  = INIT_ALLOC;
    char *buffer = malloc(bufsiz);
    if (buffer == NULL)
        return NULL;
    while (fgets(buffer + offset, bufsiz - offset, fp) != NULL)
    {
        /* Assumes data does not contain null bytes */
        /* Generic problem using fgets() */
        size_t newlen = strlen(buffer + offset);
        offset += newlen;
        if (bufsiz - offset < MIN_SPACE)
        {
            size_t new_size = bufsiz * 2;
            char  *new_data = realloc(buffer, new_size);
            if (new_data == NULL)
            {
                free(buffer);
                return NULL;
            }
            bufsiz = new_size;
            buffer = new_data;
        }
    }
    if (bufsiz - offset > MAX_EXCESS)
        buffer = realloc(buffer, offset + 1);
    return buffer;
}

int file_exists(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

void reset_modelmatrix() {
    glm_mat4_identity(modelMatrix);
    glm_scale(modelMatrix,(vec3){2.0/WINDOW_WIDTH,2.0/WINDOW_HEIGHT,0.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

//Move to posX and posY

void set_position(float posX, float posY) {
    glm_translate(modelMatrix,(vec3){posX, posY, 0.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

//Rotate

void rotate(float angle) {
    glm_rotate(modelMatrix,angle,(vec3){0.0,0.0,1.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

void set_figure_size(vec2 elem_size, vec2 new_size) {
    glm_scale(modelMatrix,(vec3){new_size[0]/elem_size[0],new_size[1]/elem_size[1],0.0});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

//Generates a rectangle in triangle format (6 vertices, 18 numbers)

void gen_rectangle(float w, float h, vec2 pos, float rad, float ang, float *vertices) {
    float centerX = pos[0] + rad*cos(ang);
    float centerY = pos[1] + rad*sin(ang);

    vec3 ul_corner = {
        centerX - cos(ang)*w/2 - sin(ang)*h/2,
        centerY + cos(ang)*h/2 - sin(ang)*w/2,
        0.0
    };

    vec3 ur_corner = {
        centerX + cos(ang)*w/2 - sin(ang)*h/2,
        centerY + cos(ang)*h/2 + sin(ang)*w/2,
        0.0
    };

    vec3 lr_corner = {
        centerX + cos(ang)*w/2 + sin(ang)*h/2,
        centerY - cos(ang)*h/2 + sin(ang)*w/2,
        0.0
    };

    vec3 ll_corner = {
        centerX - cos(ang)*w/2 + sin(ang)*h/2,
        centerY - cos(ang)*h/2 -sin(ang)*w/2,
        0.0
    };

    float temp[] = {
        ll_corner[0], ll_corner[1], 0.0f,
        ul_corner[0], ul_corner[1], 0.0f,
        ur_corner[0], ur_corner[1], 0.0f,
        ll_corner[0], ll_corner[1], 0.0f,
        ur_corner[0], ur_corner[1], 0.0f,
        lr_corner[0], lr_corner[1], 0.0f
    };

    memcpy(vertices, temp, sizeof(temp));
}
//Creates an octagonal form in triangle fan format (9 vertices, 27 numbers)

void gen_octagon(float w, float h, float corner_catet, vec2 pos, float *vertices) {

    float pointX1 = pos[0] - w/2;
    float pointX2 = pointX1 + corner_catet;
    float pointX4 = pos[0] + w/2;
    float pointX3 = pointX4 - corner_catet;

    float pointY1 = pos[1] - h/2;
    float pointY2 = pointY1 + corner_catet;
    float pointY4 = pos[1] + h/2;
    float pointY3 = pointY4 - corner_catet;

    float temp[] = {
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
    memcpy(vertices, temp, sizeof(temp));
}

void gen_arc(float rad, vec2 pos, float ang1, float ang2, int sides, float *vertices) {
    vertices[0] = pos[0];
    vertices[1] = pos[1];
    vertices[2] = 0.0;

    for (int i = 0; i < sides; i++) {
        float ang = ang1 + (ang2-ang1)*i/(sides-1);
        vertices[3 + 3*i] = pos[0] + rad*cos(ang);
        vertices[3 + 3*i + 1] = pos[1] +  rad*sin(ang);
        vertices[3 + 3*i + 2] = 0.0;
    }
}

void gen_normal(float* triangle, float* normal) {
    vec3 AB = {
        triangle[3] - triangle[0], 
        triangle[4] - triangle[1], 
        triangle[5] - triangle[2]
    };

    vec3 AC = {
        triangle[6] - triangle[0], 
        triangle[7] - triangle[1], 
        triangle[8] - triangle[2]
    };
   
    glm_vec3_cross(AB,AC,normal);
    glm_normalize(normal);
    /*
    printf("Normal of triangle: \
        (%0.6f, %0.6f, %0.6f),  \
        (%0.6f, %0.6f, %0.6f),  \
        (%0.6f, %0.6f, %0.6f):  \
        (%0.6f, %0.6f, %0.6f)\n", \
        triangle[0], triangle[1], triangle[2], 
        triangle[3] ,triangle[4], triangle[5],
        triangle[6], triangle[7],triangle[8], 
        normal[0], normal[1], normal[2]);*/
}

void gen_arc_section(float rad1, float rad2, vec2 pos, float ang1, float ang2, int sides, float *vertices) {
    vertices[0] = pos[0] + rad2*cos(ang1);
    vertices[1] = pos[1] + rad2*sin(ang1);
    vertices[2] = 0.0;
    vertices[3] = pos[0] + rad1*cos(ang1);
    vertices[4] = pos[1] + rad1*sin(ang1);
    vertices[5] = 0.0;

    int num_reps = (sides-1);

    for (int i = 0; i < num_reps; i++) {
        float a1 = ang1 + (ang2 - ang1)*i/num_reps;
        float a3 = ang1 + (ang2 - ang1)*(i+1)/num_reps;
        float a2 = (a1 + a3)/2.0;

        vertices[6 + 6*i] = pos[0] + rad2*cos(a2);
        vertices[6 + 6*i + 1] = pos[1] + rad2*sin(a2);
        vertices[6 + 6*i + 2] = 0.0;

        vertices[6 + 6*i + 3] = pos[0] + rad1*cos(a3);
        vertices[6 + 6*i + 4] = pos[1] + rad1*sin(a3);
        vertices[6 + 6*i + 5] = 0.0;
    }

    vertices[6 + 6*num_reps] = pos[0] + rad2*cos(ang2);
    vertices[6 + 6*num_reps + 1] = pos[1] + rad2*sin(ang2);
    vertices[6 + 6*num_reps + 2] = 0.0;
}

void set_default_projection() {
    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);
}

void set_color(vec4 color) {
    glUniform4fv(colorLoc,1, color);
}

void set_texture(int texture_on) {
    enabledFlags[1] = texture_on;
    glUniform2i(enabledFlagsLoc, 2, enabledFlags[0]);
}

void add_to_buffer(GLuint *VBO, float *vertices, unsigned int size) {
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void draw(int VBO, int num_vertices, int mode) {
    #ifdef OPENGL_ES_MODE
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDrawArrays(mode, 0, num_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    #else
    //printf("Printing VBO %d in mode %d\n", VBO, mode);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(vPositionLoc);

    glVertexAttribPointer(vPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    /*
    float* normals = malloc(num_vertices * 3 * sizeof(float));
    for (int i = 0; i < num_vertices * 3; i++) normals[i] = 0.0f;
    */
    //glEnableVertexAttribArray(normalLoc);
    //glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    //glVertexAttribPointer(vTexCoordsLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    //printf("Here %d %d\n", mode, num_vertices);

    glDrawArrays(mode, 0, num_vertices);

    //printf("Printed successfully :)\n");
    #endif
}

void resize(float sizeX, float sizeY) {
    glm_scale(modelMatrix,(vec3){sizeX,sizeY,0.0f});
    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
}

//Stencil related

void enable_stencil_test() {
    glClearStencil(0);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF); // Write to stencil buffer
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
}

void enable_stencil_buffer_changes() {
    glStencilMask(0xFF); // Set any stencil to 1
}

void disable_stencil_buffer_changes() {
    glStencilMask(0x00); // Don't write to stencil buffer
}

void pass_stencil_if_1() {
    glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);
}

void pass_stencil_if_0() {
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Pass test if stencil value is 1
    glStencilOp(GL_ZERO, GL_ZERO, GL_REPLACE);
}

void disable_stencil_test() {
    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
    glDisable(GL_STENCIL_TEST);
}

//Rotation Matrix

void rot_matrix(float rotX, float rotY, float rotZ, mat4 mat) {

    mat4 rotXMatrix = {
        {1.0, 0.0, 0.0, 0.0},
        {0.0, cos(rotX), sin(rotX), 0.0},
        {0.0, -sin(rotX), cos(rotX), 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    mat4 rotYMatrix = {
        {cos(rotY), 0.0, -sin(rotY), 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {sin(rotY), 0.0, cos(rotY), 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    mat4 rotZMatrix = {
        {cos(rotZ), -sin(rotZ), 0.0, 0.0},
        {sin(rotZ), cos(rotZ), 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    };

    glm_mat4_mul(rotXMatrix, rotYMatrix, rotYMatrix);
    glm_mat4_mul(rotYMatrix, rotZMatrix, mat);
}



float fix_angle(float rot) {
    rot = rot > 2*GLM_PI ? rot - 2*GLM_PI : rot;
    rot = rot < 0 ? rot + 2*GLM_PI : rot;
    return rot;
}

float mod(float a, float b) {
    return a - b*floor(a/b);
}

float max(float a, float b) {
    return a >= b ? a : b;
}

float min(float a, float b) {
    return a < b ? a : b;
}


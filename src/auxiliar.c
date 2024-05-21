#include <stdio.h>
#include "auxiliar.h"


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

    vertices[0] = ll_corner[0];
    vertices[1] = ll_corner[1];
    vertices[2] = 0.0;
    vertices[3] = ul_corner[0];
    vertices[4] = ul_corner[1];
    vertices[5] = 0.0;
    vertices[6] = ur_corner[0];
    vertices[7] = ur_corner[1];
    vertices[8] = 0.0;
    vertices[9] = ll_corner[0];
    vertices[10] = ll_corner[1];
    vertices[11] = 0.0;
    vertices[12] = ur_corner[0];
    vertices[13] = ur_corner[1];
    vertices[14] = 0.0;
    vertices[15] = lr_corner[0];
    vertices[16] = lr_corner[1];
    vertices[17] = 0.0;
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


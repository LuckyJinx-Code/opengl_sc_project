#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include "esUtil.h"
#include "parameters.h"

void reset_modelmatrix();
void set_position(float posX, float posY);
void set_figure_size(vec2 elem_size, vec2 new_size);
void gen_rectangle(float w, float h, vec2 pos, float rad, float ang, float *vertices);
void gen_octagon(float w, float h, float corner_catet, vec2 pos, float *vertices);
void gen_normal(float* triangle, float* normal);
void gen_arc(float rad, vec2 pos, float ang1, float ang2, int sides, float *vertices);
void gen_arc_section(float rad1, float rad2, vec2 pos, float ang1, float ang2, int sides, float *vertices);
void rot_matrix(float rotX, float rotY, float rotZ, mat4 mat);
float fix_angle(float rot);
float mod(float a, float b);

#endif
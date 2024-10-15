#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "shared.h"

char *read_file(FILE *fp);
int file_exists(const char *filename);
void reset_modelmatrix();
void set_position(float posX, float posY);
void rotate(float angle);
void set_figure_size(vec2 elem_size, vec2 new_size);
void gen_rectangle(float w, float h, vec2 pos, float rad, float ang, float *vertices);
void gen_octagon(float w, float h, float corner_catet, vec2 pos, float *vertices);
void gen_normal(float* triangle, float* normal);
void gen_arc(float rad, vec2 pos, float ang1, float ang2, int sides, float *vertices);
//Draws an arc into an array of 6 + 6*(sides - 1) + 3 values
void gen_arc_section(float rad1, float rad2, vec2 pos, float ang1, float ang2, int sides, float *vertices);
void set_default_projection();
void set_color(vec4 color);
void set_texture(int texture_on);
void rot_matrix(float rotX, float rotY, float rotZ, mat4 mat);
void enable_stencil_test();
void disable_stencil_test();
void enable_stencil_buffer_changes();
void disable_stencil_buffer_changes();
void pass_stencil_if_1();
void pass_stencil_if_0();
void draw(int VBO, int num_vertices, int mode);
void add_to_buffer(GLuint *VBO,float *vertices, unsigned int size);
void resize(float sizeX, float sizeY);
float fix_angle(float rot);
float mod(float a, float b);
float max(float a, float b);
float min(float a, float b);

#endif
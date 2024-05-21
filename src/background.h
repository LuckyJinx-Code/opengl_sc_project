#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include "esUtil.h"
#include "auxiliar.h"
#include "parameters.h"
#include <time.h>

#define POS_ATTR 0
#define NORMAL_ATTR 1

#define GRID_SIZE 32
#define NUM_VERTICES_MOUNTAINS GRID_SIZE*GRID_SIZE*12


void init_map();
float randomize(int x, int y);
float interpolation(float a0, float a1,float x);
float dot_product_corner(int x, int y, int corner_x, int corner_y);
float sample(int x, int y);
void load_chunk(int chunk_x, int chunk_y, int VBO_index);
int is_chunk_in_range(int chunk_x, int chunk_y);
int is_chunk_loaded(int chunk_x, int chunk_y);
void update_chunks();
void draw_chunk(int VBO_index);
void draw_map();

#endif
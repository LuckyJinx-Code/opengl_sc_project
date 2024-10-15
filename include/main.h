#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esUtil.h"
#include "input.h"
#include "utils.h"
#include "text_generator.h"
#include "background.h"
#include "compass.h"
#include "SSI.h"
#include "airspeed.h"
#include "altimeter.h"
#include "VSI.h"
#include "waypoints.h"
#include "frame.h"
#include "shared.h"
#include "CDI.h"
#include "wp_info.h"
#include "misc.h"
#include "structures.h"

#ifdef OPENGL_ES_MODE
    #include <X11/Xlib.h>
    #include <X11/Xatom.h>
    #include <X11/Xutil.h>

    ESContext esContext;

#else
    #include <GLSC2/glsc2.h>
    #include "SDL2/SDL.h"

    SDL_Window* window;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture;

    int vPositionLoc, normalLoc, vTexCoordsLoc, program;

    u_int32_t *pixel_map;
#endif

GLuint background_VBO;
GLuint frame_VBO;
GLuint frame_circle_VBO;
GLuint compass_VBO;
GLuint compass_angle_display_VBO;
GLuint compass_angle_pointer_VBO;
GLuint SSI_arc_VBO;
GLuint SSI_lines_VBO;
GLuint SSI_pitch_line_1_VBO;
GLuint SSI_pitch_line_2_VBO;
GLuint SSI_center_mark_VBO;
GLuint SSI_triangle_VBO;
GLuint SSI_horizon_line_VBO;
GLuint airspeed_inner_VBO;
GLuint airspeed_outer_VBO;
GLuint airspeed_line_1_VBO;
GLuint airspeed_line_2_VBO;
GLuint airspeed_pointer_VBO;
GLuint altimeter_inner_VBO;
GLuint altimeter_outer_VBO;
GLuint altimeter_line_1_VBO;
GLuint altimeter_line_2_VBO;
GLuint altimeter_pointer_VBO;
GLuint VSI_VBO;
GLuint VSI_line_1_VBO;
GLuint VSI_line_2_VBO;
GLuint VSI_pointer_VBO;
GLuint char_VBO;
GLuint *chunk_VBO_list;
GLuint wp_window_VBO;
GLuint wp_radar_outer_VBO;
GLuint wp_radar_outer_lines_VBO;
GLuint wp_radar_inner_1_VBO;
GLuint wp_radar_inner_2_VBO;
GLuint wp_icon_inner_VBO;
GLuint wp_icon_outer_VBO;
GLuint wp_link_VBO;
GLuint wp_plane_icon_VBO;
GLuint wp_info_window_VBO;
GLuint wp_info_header_VBO;
GLuint wp_info_icon_inner_VBO;
GLuint wp_info_icon_outer_VBO;
GLuint wp_info_link_VBO;
GLuint CDI_plane_VBO;
GLuint CDI_needle_head_VBO;
GLuint CDI_needle_tail_VBO;
GLuint CDI_scale_mark_VBO;

GLuint vertexShader;
GLuint fragmentShader;

GLuint colorLoc;
GLuint modelMatrixLoc;
GLuint viewMatrixLoc;
GLuint projectionMatrixLoc;
GLuint normalMatrixLoc;
GLuint enabledFlagsLoc;

struct timeval exec_time;
struct timeval prev_time;

unsigned int limit_set;
float time_limit;
float delta_time;

//Airplane position

vec3 pos = {300.0f, 4.0f, 300.0f};
vec3 prev_pos = {300.0f, 4.0f, 300.0f};
vec3 speed = {0.0f, 0.0f, 0.0f};

//Airplane rotation

GLfloat rotX = 0.0;
GLfloat rotY = 0.0;
GLfloat rotZ = 0.0;

//Terrain values

unsigned int is_terrain_active;
unsigned int vertices_per_square;
unsigned int squares_per_chunk;
unsigned int terrain_diameter;
float mountain_max_height;
float chunk_size;

//Airplane speed in k/s
//GLfloat speed = INITIAL_SPEED;
//Airplane accel in k/s^2
GLfloat speed_accel = 0.0;

GLfloat rotSpeedX = 0.0;
GLfloat rotSpeedY = 0.0;
GLfloat rotSpeedZ = 0.0;

GLint chunk_pos_x = 0.0;
GLint chunk_pos_y = 0.0;

int  num_widgets;

#define MAX_WAYPOINTS 256

Widgets widgets;
Controllers controllers;
Waypoints waypoints;
//Waypoint route[256];

int map_test_mode = 0;

//Other parameters
float sl_density = 1.225; //Air density at sea level

vec4 color;
mat4 modelMatrix;
mat4 rotMat;
mat4 viewMatrix;
mat4 projectionMatrix;
mat4 normalMatrix;
int enabledFlags[2];

char input_key = ' ';

#endif
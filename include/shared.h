#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "structures.h"

#ifdef OPENGL_EC_MODE
#else
    extern int vPositionLoc, normalLoc, vTexCoordsLoc, program;
#endif

//Main program

extern GLuint vertexShader;
extern GLuint fragmentShader;

extern GLuint colorLoc;
extern GLuint modelMatrixLoc;
extern GLuint viewMatrixLoc;
extern GLuint projectionMatrixLoc;
extern GLuint normalMatrixLoc;
extern GLuint enabledFlagsLoc;

extern struct timeval exec_time;
extern struct timeval prev_time;
extern unsigned int limit_set;
extern float time_limit;
extern float delta_time;

extern vec3 pos;
extern vec3 prev_pos;
extern vec3 speed;

extern GLfloat rotX;
extern GLfloat rotY;
extern GLfloat rotZ;

extern GLfloat rotSpeedX;
extern GLfloat rotSpeedY;
extern GLfloat rotSpeedZ;

extern unsigned int is_terrain_active;
extern unsigned int vertices_per_square;
extern unsigned int squares_per_chunk;
extern unsigned int terrain_diameter;
extern float chunk_size;
extern float mountain_max_height;

extern GLint chunk_pos_x;
extern GLint chunk_pos_y;

extern int map_test_mode;

extern vec4 color;
extern mat4 modelMatrix;
extern mat4 normalMatrix;
extern mat4 rotMat;
extern mat4 viewMatrix;
extern mat4 projectionMatrix;
extern int enabledFlags[2];

//typedef struct Waypoint_t Waypoint;

extern Waypoints waypoints;
extern Widgets widgets;
extern Controllers controllers;

//Parameters

//Window

#define WINDOW_WIDTH 1000.0f
#define WINDOW_HEIGHT 650.0f

//Plane parameters

#define KIL_PER_UNIT 1.0f

//Attribute ids

#define POS_ATTR 0
#define NORMAL_ATTR 1
#define TEX_ATTR 2

//Background

#define NUM_CHUNKS terrain_diameter*terrain_diameter
#define SEED 12345
#define CUBIC_NOISE_RAND_A 134775813
#define CUBIC_NOISE_RAND_B 1103515245

//Frame

#define FRAME_CIRCLE_SIDES 200
#define MARGIN_TOP 60
#define MARGIN_BOTTOM 60
#define MARGIN_LEFT 80
#define MARGIN_RIGHT 80
#define CORNER_SQUARE 50

//Compass

#define COMPASS_RADIUS 100.0f
#define COMPASS_MARGIN 10
#define COMPASS_NUM_MARKERS 72
#define COMPASS_MARKER_SMALL_LEN 8
#define COMPASS_MARKER_BIG_LEN 14
#define COMPASS_MARKER_THICKNESS 2
#define COMPASS_NUM_VERTICES COMPASS_NUM_MARKERS*6
#define COMPASS_NSWE_RADIUS 72
#define COMPASS_NSWE_SIZE 0.2f
#define COMPASS_ANGLE_DISPLAY_WIDTH 56.0f
#define COMPASS_ANGLE_DISPLAY_HEIGHT 24.0f
#define COMPASS_ANGLE_DISPLAY_CATET 5.0f
#define COMPASS_ANGLE_DISPLAY_STROKE 3.0f
#define COMPASS_ANGLE_POINTER_SIZE 10.0f
#define COMPASS_ANGLE_FONT_SIZE 0.45f

//SSI

#define SSI_ARC_NUM_SIDES 200
#define SSI_THICKNESS 2
#define SSI_RADIUS_1 120
#define SSI_RADIUS_2 (SSI_RADIUS_1 + SSI_THICKNESS)
#define SSI_ARC_NUM_VERTICES (2*(SSI_ARC_NUM_SIDES) + 1)
#define SSI_Y_POS 90
#define SSI_ANGLE 15
#define SSI_NUM_LINES 9
#define SSI_LINE_LEN_SMALL 10
#define SSI_LINE_LEN_BIG 14
#define SSI_LINES_NUM_VERTICES 6*SSI_NUM_LINES
#define SSI_CENTER_MARK_SIZE 6
#define SSI_TRIANGLE_SIDE 18.0f
#define SSI_HORIZON_LINES_WIDTH 40.0f
#define SSI_HORIZON_LINES_HEIGHT 4.0f
#define SSI_HORIZON_LINES_DIST 120.0f

#define SSI_PITCH_SPACE_BETWEEN 20
#define SSI_PITCH_WIDTH_SMALL 40
#define SSI_PITCH_WIDTH_BIG 80
#define SSI_PITCH_HEIGHT 180.0f
#define SSI_PITCH_VANISH_HEIGHT 20
#define SSI_PITCH_NUM_LINES (360/5)
#define SSI_PITCH_NUM_VERTICES SSI_PITCH_NUM_LINES*6
#define SSI_PITCH_SPACE_NUM 12.0f

//Airspeed indicator

#define AIRSPEED_WIDTH 80.0f
#define AIRSPEED_HEIGHT 260.0f
#define AIRSPEED_INFO_MARGIN_HEIGHT 20.0f
#define AIRSPEED_STROKE_THICKNESS 3.0f
#define AIRSPEED_CORNER_CATET 6.0f
#define AIRSPEED_ALPHA 0.3f
#define AIRSPEED_LINE_SPACE_BETWEEN 10.0f
#define AIRSPEED_LINE_WIDTH_BIG 12.0f
#define AIRSPEED_LINE_WIDTH_SMALL 8.0f
#define AIRSPEED_LINE_THICKNESS 2.0f
#define AIRSPEED_NUMBER_SIZE 0.3f

#define AIRSPEED_POINTER_WIDTH 68.0f
#define AIRSPEED_POINTER_HEIGHT 28.0f
#define AIRSPEED_POINTER_CATET 6.0f
#define AIRSPEED_POINTER_TRIANGLE 20.0f
#define AIRSPEED_POINTER_DIGIT_SIZE 0.4f
#define AIRSPEED_POINTER_DIGIT_VERT_SPACE 20.0f
#define AIRSPEED_POINTER_DIGIT_HOR_SPACE 12.0f

//Altimeter

#define ALTIMETER_WIDTH 80.0f
#define ALTIMETER_HEIGHT 260.0f
#define ALTIMETER_INFO_MARGIN_HEIGHT 20.0f
#define ALTIMETER_STROKE_THICKNESS 3.0f
#define ALTIMETER_CORNER_CATET 6.0f
#define ALTIMETER_ALPHA 0.3f
#define ALTIMETER_LINE_SPACE_BETWEEN 10.0f
#define ALTIMETER_LINE_WIDTH_BIG 12.0f
#define ALTIMETER_LINE_WIDTH_SMALL 8.0f
#define ALTIMETER_LINE_THICKNESS 2.0f
#define ALTIMETER_NUMBER_SIZE 0.3f

#define ALTIMETER_POINTER_WIDTH 80.0f
#define ALTIMETER_POINTER_HEIGHT 28.0f
#define ALTIMETER_POINTER_CATET 6.0f
#define ALTIMETER_POINTER_TRIANGLE 20.0f
#define ALTIMETER_POINTER_DIGIT_SIZE 0.36f
#define ALTIMETER_POINTER_DIGIT_VERT_SPACE 20.0f
#define ALTIMETER_POINTER_DIGIT_HOR_SPACE 10.0f

#define VSI_WIDTH 40.0f
#define VSI_HEIGHT 160.0f
#define VSI_STROKE_THICKNESS 3.0f
#define VSI_CATET 6.0f
#define VSI_NUM_UNITS 2
#define VSI_ALPHA 0.3f
#define VSI_LINE_WIDTH_BIG 12.0f
#define VSI_LINE_WIDTH_SMALL 8.0f
#define VSI_LINE_THICKNESS 2.0f
#define VSI_LINE_MARGIN 10.0f
#define VSI_UNIT_EXPONENTIAL 1.8f
#define VSI_NUMBER_SIZE 0.26f
#define VSI_CURSOR_LENGTH 25.0f

//Waypoints

#define WP_WINDOW_WIDTH 240.0f
#define WP_WINDOW_HEIGHT 180.0f
#define WP_WINDOW_CATET 5
#define WP_RIGHT_MARGIN 260.0f
#define WP_BOTTOM_MARGIN 170.0f
#define WP_WINDOW_ALPHA 0.5f
#define WP_WINDOW_STROKE_THICKNESS 3.0f
#define WP_NAV_RADIUS 140.0f
#define WP_NAV_UNITS_FONT_SIZE 0.25f
#define WP_RANGE_LINE_LENGTH 10.0f
#define WP_OUTER_RANGE_NUM_SIDES 300
#define WP_OUTER_RANGE_BLINE_SIZE 6.0f
#define WP_OUTER_RANGE_SLINE_SIZE 3.0f
#define WP_RANGE_LINE_THICKNESS 2.0f
#define WP_RANGE_CENTER_POS_Y -70.0f
#define WP_ICON_OUTER_SIZE 20.0f
#define WP_ICON_INNER_SIZE 10.0f
#define WP_ICON_STROKE 2.0f
#define WP_ICON_LINK_THICKNESS 5.0f
#define WP_DISPLAYED_KM 10.0f
#define WP_PLANE_ICON_WIDTH 18.0f
#define WP_PLANE_ICON_HEIGHT 24.0f
#define WP_PLANE_ICON_STROKE 3.0f

//Waypoint information

#define DEST_INFO_WINDOW_WIDTH 240.0f
#define DEST_INFO_WINDOW_HEIGHT 180.0f
#define DEST_INFO_WINDOW_CATET 5.0f
#define DEST_INFO_WINDOW_STROKE 3.0f
#define DEST_INFO_HEADER_HEIGHT 30.0f
#define DEST_INFO_HEADER_FONT_SIZE 0.45f
#define DEST_INFO_WINDOW_LEFT_MARGIN 260.0f
#define DEST_INFO_WINDOW_BOTTOM_MARGIN 170.0f
#define DEST_INFO_WINDOW_ALPHA 0.5f
#define DEST_INFO_TEXT_LEFT_MARGIN 20.0f
#define DEST_INFO_INNER_MARGIN 6.0f
#define DEST_INFO_ICON_OUTER_SIZE 20.0f
#define DEST_INFO_ICON_INNER_SIZE 10.0f
#define DEST_INFO_ICON_STROKE 2.0f
#define DEST_INFO_ICON_LINK_THICKNESS 5.0f
#define DEST_INFO_TEXT_SPACE_1 60.0f
#define DEST_INFO_TEXT_SPACE_2 30.0f
#define DEST_INFO_FONT_SIZE_CURRWP 0.5f
#define DEST_INFO_FONT_SIZE_NEXTWPS 0.35f
#define DEST_INFO_FONT_SIZE_CURRWPDET 0.3f
#define DEST_INFO_FONT_SIZE_CURRWPTIM 0.45f
#define DEST_INFO_FONT_SIZE_NEXTWPSTIM 0.40f
#define DEST_INFO_CURRWPDET_MRGTOP 24.0f

//CDI

#define CDI_NEEDLE_HEAD_WIDTH 12.0f
#define CDI_NEEDLE_HEAD_HEIGHT 12.0f
#define CDI_NEEDLE_TAIL_WIDTH 3.0f
#define CDI_NEEDLE_TAIL_HEIGHT 180.0f
#define CDI_SCALE_MARK_SIZE 8.0f
#define CDI_SCALE_RADIUS 50.0f

//Color pallette

#define BLACK (vec4){0.0,0.0,0.0,1.0}
#define WHITE (vec4){1.0,1.0,1.0,1.0}
#define MAP_COLOR (vec4){142.0f/255.0f,89.0f/255.0f,89.0f/255.0f,1.0f}
#define SKY_COLOR (vec4){0.1f, 0.3f, 0.3f, 1.0f}
#define COLOR_BASE_1 (vec4){200.0f/255.0f,1.0f,1.0,1.0f}
#define YELLOW (vec4){1.0f, 250.0f/255.0f, 0.0f, 1.0f}
#define PURPLE (vec4){255.0f/255.0f,0.0f,214.0f/255.0f,1.0f}

//VBOs

extern GLuint *chunk_VBO_list;

extern GLuint compass_VBO;
extern GLuint compass_angle_display_VBO;
extern GLuint compass_angle_pointer_VBO;

extern GLuint frame_VBO;
extern GLuint frame_circle_VBO;

extern GLuint SSI_arc_VBO;
extern GLuint SSI_lines_VBO;
extern GLuint SSI_pitch_line_1_VBO;
extern GLuint SSI_pitch_line_2_VBO;
extern GLuint SSI_center_mark_VBO;
extern GLuint SSI_triangle_VBO;
extern GLuint SSI_horizon_line_VBO;

extern GLuint airspeed_inner_VBO;
extern GLuint airspeed_outer_VBO;
extern GLuint airspeed_line_1_VBO;
extern GLuint airspeed_line_2_VBO;
extern GLuint airspeed_pointer_VBO;

extern GLuint altimeter_inner_VBO;
extern GLuint altimeter_outer_VBO;
extern GLuint altimeter_line_1_VBO;
extern GLuint altimeter_line_2_VBO;
extern GLuint altimeter_pointer_VBO;

extern GLuint VSI_VBO;
extern GLuint VSI_line_1_VBO;
extern GLuint VSI_line_2_VBO;
extern GLuint VSI_pointer_VBO;

extern GLuint wp_window_VBO;
extern GLuint wp_radar_outer_VBO;
extern GLuint wp_radar_outer_lines_VBO;
extern GLuint wp_radar_inner_1_VBO;
extern GLuint wp_radar_inner_2_VBO;
extern GLuint wp_icon_inner_VBO;
extern GLuint wp_icon_outer_VBO;
extern GLuint wp_link_VBO;
extern GLuint wp_plane_icon_VBO;

extern GLuint wp_info_window_VBO;
extern GLuint wp_info_header_VBO;
extern GLuint wp_info_icon_inner_VBO;
extern GLuint wp_info_icon_outer_VBO;
extern GLuint wp_info_link_VBO;

extern GLuint CDI_plane_VBO;
extern GLuint CDI_needle_head_VBO;
extern GLuint CDI_needle_tail_VBO;
extern GLuint CDI_scale_mark_VBO;

#endif
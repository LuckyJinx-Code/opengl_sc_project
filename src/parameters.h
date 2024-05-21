#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include "esUtil.h"

//Main program

extern GLuint vertexShader;
extern GLuint fragmentShader;

extern GLuint colorLoc;
extern GLuint setBrightnessLoc;
extern GLuint modelMatrixLoc;
extern GLuint viewMatrixLoc;
extern GLuint projectionMatrixLoc;
extern GLuint hasTextureLoc;

extern GLfloat posX;
extern GLfloat posY;
extern GLfloat posZ;

extern GLfloat rotX;
extern GLfloat rotY;
extern GLfloat rotZ;

extern GLfloat speed;

extern GLfloat rotSpeedX;
extern GLfloat rotSpeedY;
extern GLfloat rotSpeedZ;

extern GLint chunk_pos_x;
extern GLint chunk_pos_y;

extern int map_test_mode;

extern vec4 color;
extern mat4 modelMatrix;
extern mat4 rotMat;
extern mat4 viewMatrix;
extern mat4 projectionMatrix;

//Parameters

//Window

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

//Plane parameters

#define KIL_PER_UNIT 10.0f
#define INITIAL_SPEED 0.2f //k/s
#define ACCEL 0.001f //k/s^2
#define ROT_SPEED map_test_mode == 0 ? 0.01f : 5.0f

//Attribute ids

#define POS_ATTR 0
#define NORMAL_ATTR 1

//Map

#define CELL_SIZE 10 //In vertices
#define CHUNK_UNITS 40 //In number of squares
#define MOUNTAIN_HEIGHT 5.0f //In Kilometers
#define VIEWER_DIAMETER 7 //In number of chunks
#define NUM_CHUNKS VIEWER_DIAMETER*VIEWER_DIAMETER
#define SEED 12345
#define CUBIC_NOISE_RAND_A 134775813
#define CUBIC_NOISE_RAND_B 1103515245
#define CHUNK_SIZE 10.0f //In Kilometers

//Frame

#define FRAME_CIRCLE_SIDES 200
#define MARGIN_TOP 60
#define MARGIN_BOTTOM 60
#define MARGIN_LEFT 80
#define MARGIN_RIGHT 80
#define CORNER_SQUARE 50

//Compass

#define COMPASS_RADIUS 100
#define COMPASS_MARGIN 10
#define COMPASS_NUM_MARKERS 72
#define COMPASS_MARKER_SMALL_LEN 8
#define COMPASS_MARKER_BIG_LEN 14
#define COMPASS_MARKER_THICKNESS 2
#define COMPASS_NUM_VERTICES COMPASS_NUM_MARKERS*6
#define COMPASS_NSWE_RADIUS 72
#define COMPASS_NSWE_SIZE 0.2f

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
#define SSI_CENTER_MARK_SIZE 4
#define SSI_TRIANGLE_SIDE 18.0f

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
#define AIRSPEED_LEFT_MARGIN 260.0f
#define AIRSPEED_TOP_MARGIN 200.0f
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
#define ALTIMETER_RIGHT_MARGIN 260.0f
#define ALTIMETER_TOP_MARGIN 200.0f
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

//Distance and Time to Waypoints

#define WP_WINDOW_WIDTH 160
#define WP_WINDOW_HEIGHT 120
#define WP_WINDOW_CATET 5
#define WP_LEFT_MARGIN 150
#define WP_BOTTOM_MARGIN 100

//Color pallette

#define BLACK (vec4){0.0,0.0,0.0,1.0}
#define WHITE (vec4){1.0,1.0,1.0,1.0}
#define MAP_COLOR (vec4){142.0f/255.0f,89.0f/255.0f,89.0f/255.0f,1.0f}
#define SKY_COLOR (vec4){0.1f, 0.3f, 0.3f, 1.0f}
#define COLOR_BASE_1 (vec4){200.0f/255.0f,1.0f,1.0,1.0f}

//VBOs

extern GLuint chunk_VBO_list[NUM_CHUNKS];

extern GLuint compass_VBO;

extern GLuint frame_VBO;
extern GLuint frame_circle_VBO;

extern GLuint SSI_arc_VBO;
extern GLuint SSI_lines_VBO;
extern GLuint SSI_pitch_line_1_VBO;
extern GLuint SSI_pitch_line_2_VBO;
extern GLuint SSI_center_mark_VBO;
extern GLuint SSI_triangle_VBO;

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

extern GLuint wp_window_VBO;

#endif
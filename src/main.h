#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "esUtil.h"
#include "auxiliar.h"
#include "text_generator.h"
#include "background.h"
#include "compass.h"
#include "SSI.h"
#include "airspeed.h"
#include "altimeter.h"
#include "VSI.h"
#include "waypoints.h"
#include "frame.h"
#include "parameters.h"
#include "misc.h"

GLuint background_VBO;
GLuint frame_VBO;
GLuint frame_circle_VBO;
GLuint compass_VBO;
GLuint SSI_arc_VBO;
GLuint SSI_lines_VBO;
GLuint SSI_pitch_line_1_VBO;
GLuint SSI_pitch_line_2_VBO;
GLuint SSI_center_mark_VBO;
GLuint SSI_triangle_VBO;
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
GLuint text_VBO;
GLuint chunk_VBO_list[NUM_CHUNKS];
GLuint wp_window_VBO;

GLuint vertexShader;
GLuint fragmentShader;

GLuint colorLoc;
GLuint setBrightnessLoc;
GLuint modelMatrixLoc;
GLuint viewMatrixLoc;
GLuint projectionMatrixLoc;
GLuint hasTextureLoc;

//Airplane position
GLfloat posX = 0.0f;
GLfloat posY = 4.0f;
GLfloat posZ = 0.0f;

//Airplane rotation
GLfloat rotX = 0.0;
GLfloat rotY = 0.0;
GLfloat rotZ = 0.0;

//Airplane speed in k/s
GLfloat speed = INITIAL_SPEED;
//Airplane accel in k/s^2
GLfloat speed_accel = 0.0;

GLfloat rotSpeedX = 0.0;
GLfloat rotSpeedY = 0.0;
GLfloat rotSpeedZ = 0.0;

GLint chunk_pos_x = 0.0;
GLint chunk_pos_y = 0.0;

int map_test_mode = 0;

//Other parameters
float sl_density = 1.225; //Air density at sea level

vec4 color;
mat4 modelMatrix;
mat4 rotMat;
mat4 viewMatrix;
mat4 projectionMatrix;

char input_key = ' ';

char *read_file(FILE *fp);
void keyCallback(ESContext* esContext, unsigned char key, int x, int y);
int load_font_textures();
void gen_uniforms();
GLuint LoadShader(GLenum type, const char *shaderSrc);
void gen_triangle();
int Init(ESContext *esContext);
void Draw( ESContext *esContext);

#endif
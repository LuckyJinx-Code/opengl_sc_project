#ifndef COMPONENT_CONFIG_H
#define COMPONENT_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "background.h"
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

#define POSX_PARAM 0
#define POSY_PARAM 1
#define POSZ_PARAM 2
#define ROTX_PARAM 3
#define ROTY_PARAM 4
#define ROTZ_PARAM 5

#define LINEAR_EASING 0
#define QUAD_EASING 1
#define SINUSOID_EASING 2
#define COSINUSOID_EASING 3

#define RESTART_LOOPMODE 0
#define REVERSE_LOOPMODE 1
#define ENDLESS_LOOPMODE 2
#define NOLOOP_LOOPMODE 3

#define VSI_WIDGET 0
#define CDI_WIDGET 1
#define AIRSPEED_WIDGET 2
#define ALTIMETER_WIDGET 3
#define COMPASS_WIDGET 4
#define SSI_WIDGET 5
#define WAYPOINTS_WIDGET 6
#define WPINFO_WIDGET 7

void add_controller(
    unsigned int parameter, unsigned int easing, float first_value, float last_value,
    float duration, unsigned int loop_mode);
void add_widget(int id, int posX, int posY);
void add_waypoint(char* id, int posX, int posZ, time_t arrival_time);
void draw_widgets();
void update_parameters();

#endif
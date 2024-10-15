#ifndef AIRSPEED_H
#define AIRSPEED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "utils.h"
#include "shared.h"
#include "text_generator.h"

void gen_airspeed();
void draw_airspeed(float vPosX, float vPosY);

extern float sl_density;

#endif
#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "utils.h"
#include "shared.h"
#include "config_control.h"

void read_config_json(char* json_str);

#endif
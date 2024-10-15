#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cglm/cglm.h"
#include "esUtil.h"
#include "utils.h"
#include "structures.h"

#define NUM_CHARS_FONT 128

extern GLuint char_VBO;

int load_font_textures();

/**
 * Renders text in the actual position
 * @param text The text which will be displayed on the screen
 * @param align_mode Align mode. 0 = Center. 1 = Right. 2 = Left
 */
void render_text(char *text, float size, int align_mode);

#endif
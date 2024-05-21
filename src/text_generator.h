#ifndef TEXT_GENERATOR_H
#define TEXT_GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include "esUtil.h"
#include "auxiliar.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#define NUM_CHARS_FONT 128

extern GLuint text_VBO;

int load_font_textures();
void render_text(char *text, float scale, int align_mode);

#endif
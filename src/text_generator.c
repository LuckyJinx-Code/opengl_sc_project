
#include "text_generator.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

#define MAX_CHARS 128
#define ROWS_BM 12
#define COLS_BM 12
#define BM_SIZE 1020.0f
#define BM_FONT_W BM_SIZE/COLS_BM
#define BM_FONT_H BM_SIZE/ROWS_BM
#define BM_AVG_H 32.0f

Character characters[MAX_CHARS];

int load_glyph_info() {
   #ifdef OPENGL_ES_MODE
   for (unsigned int i = 0; i < MAX_CHARS; i++) {
      characters[i].id = ' ';
      characters[i].width = 0;
      characters[i].height = 0;
      characters[i].advance = 0;
      characters[i].baseline = 0;
   }

   FILE *file = fopen("res/ascii_parameters.txt", "r");
   if (file == NULL) {
      perror("Error opening file");
      return 0;
   }

   int index = ' ';

   char id;
   unsigned int width, height, advance, baseline;
   
   while (fscanf(file, "%c %u %u %u %u\n", &id, &width, &height, &baseline, &advance) == 5) {
      if (index >= 128) break;

      characters[index].id = id;
      characters[index].width = width;
      characters[index].height = height;
      characters[index].advance = advance;
      characters[index].baseline = baseline;

      //Character VBO

      //float char_width = width/BM_SIZE;
      //float left_margin = (font_width - char_width)/2.0f;

      int num_char = id - ' ';

      int num_row = ROWS_BM - 1 - num_char/COLS_BM;
      int num_col = num_char%COLS_BM;

      float bm_left = (float)num_col/COLS_BM;
      float bm_right = (float)(num_col + 1)/COLS_BM;

      float bm_bottom = (float)num_row/ROWS_BM;
      float bm_top = (float)(num_row + 1)/ROWS_BM;

      /*
      float bm_left = 1.0f/COLS_BM + (float)num_col/COLS_BM + left_margin;
      float bm_right = bm_left + char_width;

      float bm_bottom = (float)num_row/ROWS_BM;
      float bm_top = bm_bottom + 1.0f/ROWS_BM;
      */

      //printf("Character %c has width %d and advance %d.\n", id, width, advance);

      //printf("Character %c: Left: %0.4f Right: %0.4f Top: %0.4f Bottom: %0.4f\n",
      //   id, bm_left, bm_right, bm_top, bm_bottom);
      
      //printf("Char %c is ID %d with row %d and column %d.\n", id, num_char, num_row, num_col);

      float vertices[20] = { //Vertex coords and texture coords
         -BM_FONT_W/2.0f, -BM_FONT_H/2.0f, 0.0f,  bm_left, bm_bottom,
         -BM_FONT_W/2.0f, BM_FONT_H/2.0f, 0.0f,  bm_left, bm_top,
         BM_FONT_W/2.0f, BM_FONT_H/2.0f, 0.0f,  bm_right, bm_top,
         BM_FONT_W/2.0f, -BM_FONT_H/2.0f, 0.0f,  bm_right, bm_bottom,
      };

      glGenBuffers(1, &characters[index].VBO);
      glBindBuffer(GL_ARRAY_BUFFER, characters[index].VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      //printf("ID: %c\n", id);
      index++;
   }
   /*
   for (unsigned int i = 0; i < MAX_CHARS; i++) {
      printf("Character: %c, PosX: %u, PosY: %u, Width: %u, Height: %u\n",
         characters[i].id, characters[i].width, characters[i].height,
         characters[i].advance, characters[i].baseline);
   }
   */

   #endif
   return 1;
}

int load_font_textures() {
   //printf("Reached 1\n");

   #ifdef OPENGL_ES_MODE

   unsigned int bm_texture;

    glGenTextures(1, &bm_texture);
    glBindTexture(GL_TEXTURE_2D, bm_texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // note that we set the container wrapping method to GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    stbi_set_flip_vertically_on_load(true);  
    unsigned char *data = stbi_load("res/ascii_bitmap.png", &width, &height, &nrChannels, 0);

    if (data)
    {
         //printf("Reached 2\n");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bm_texture);
    }
    else
    {
        printf("Failed to load texture");
    }

    stbi_image_free(data);

   //Reading CSV

   load_glyph_info();

   #endif
   return 1;
}

void render_text(char *text, float size, int align_mode) {
   #ifdef OPENGL_ES_MODE
   if (text[0] == '\0') return;

   enabledFlags[1] = 1;
   glUniform1iv(enabledFlagsLoc, 2, enabledFlags);

   mat4 old_pos;
   glm_mat4_copy(modelMatrix,old_pos);

   resize(size,size);

   //if (align_mode > 0) set_position(characters[fchar_index].advance/2.0f,0.0f);

   if (align_mode == 0 || align_mode == 2) {
      float total_width = 0;

      unsigned int i = 0;
      unsigned char c = text[i];

      while (c != '\0') {
         int index = c;

         total_width += characters[index].advance;

         i++;
         c = text[i];
      }
      
      if (align_mode == 0) {
         set_position(-total_width/2.0f,0.0f);
      } else if (align_mode == 2) {
         set_position(-total_width,0.0f);
      }
   }

   set_position(0.0f,-BM_AVG_H/2.0f);

   unsigned int i = 0;
   unsigned char c = text[i];

   /*
   set_figure_size(
      square_size,
      (vec2) {1.0f,1.0f}
   );
   */
  
   while (c != '\0') {
      int index = c;

      float base_bottom = 
         characters[index].baseline - characters[index].height/2.0f;

      //float base_bottom = -BM_FONT_H/2.0f + characters[index].height/2.0f;
      //float base_bottom = 0.0f;
      float base_left = characters[index].width/2.0f;

      set_position(base_left,base_bottom);

      //printf("Writing %c with advance %d\n",c,characters[index].advance);

      glBindBuffer(GL_ARRAY_BUFFER, characters[index].VBO);

      glEnableVertexAttribArray(POS_ATTR);
      glVertexAttribPointer(POS_ATTR, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

      glEnableVertexAttribArray(TEX_ATTR);
      glVertexAttribPointer(TEX_ATTR, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      set_position(-base_left,-base_bottom);
      set_position(characters[index].advance,0.0f);

      i++;
      c = text[i];
   }

   glm_mat4_copy(old_pos, modelMatrix);

   enabledFlags[1] = 0;
   glUniform1iv(enabledFlagsLoc, 2, enabledFlags);
   #endif
}



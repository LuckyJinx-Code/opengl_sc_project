
#include "text_generator.h"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    vec2   Size;       // Size of glyph
    vec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

struct Character characters[NUM_CHARS_FONT];

int load_font_textures() {
   FT_Library ft;
   if (FT_Init_FreeType(&ft)) {
      printf("ERROR::FREETYPE: Could not init FreeType Library\n");
      return -1;
   }

   FT_Face face;
   if (FT_New_Face(ft, "../fonts/cairo.ttf", 0, &face)) {
      printf("ERROR::FREETYPE: Failed to load font\n");  
      return -1;
   }

   //How many pixels we want to extract

   FT_Set_Pixel_Sizes(face, 0, 48);

   //Let's load X character

   if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
   {
      printf("ERROR::FREETYTPE: Failed to load Glyph\n");  
      return -1;
   }

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
   for (unsigned char c = 0; c < NUM_CHARS_FONT; c++)
   {
      // load character glyph 
      if (FT_Load_Char(face, c, FT_LOAD_RENDER))
      {
         printf("ERROR::FREETYTPE: Failed to load Glyph\n");
         continue;
      }
      // generate texture
      unsigned int texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
         GL_TEXTURE_2D,
         0,
         GL_LUMINANCE,
         face->glyph->bitmap.width,
         face->glyph->bitmap.rows,
         0,
         GL_LUMINANCE,
         GL_UNSIGNED_BYTE,
         face->glyph->bitmap.buffer
      );

      // set texture options
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      // now store character for later use
      characters[c].TextureID = texture;
      characters[c].Size[0] = face->glyph->bitmap.width;
      characters[c].Size[1] = face->glyph->bitmap.rows;
      characters[c].Bearing[0] = face->glyph->bitmap_left;
      characters[c].Bearing[1] = face->glyph->bitmap_top;
      characters[c].Advance = face->glyph->advance.x;
   }

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

   FT_Done_Face(face);
   FT_Done_FreeType(ft);
   
   glGenBuffers(1, &text_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   
   return 1;
}

//Align_mode = 0 -> Center
//Align_mode = 1 -> Right
//Aign_mode = 2 -> Left

void render_text(char *text, float scale, int align_mode) {
    glActiveTexture(GL_TEXTURE0);

   //Calculating offset to center text

   float x = 0.0;
   float y = 0.0;

   int i = 0;
   unsigned char c = text[i];

   while (c != '\0') {
      struct Character *ch = &characters[c];
      x += (ch->Advance >> 6) * scale;
      float ySize = ch->Size[1] * scale;
      y = y < ySize ? ySize : y;
      i++;
      c = text[i];
   }

   if (align_mode == 0) x = -x/2.0;
   else if (align_mode == 2) x = -x;
   else x = 0;

   y = -y/2.0;

   //Drawing text

   i = 0;
   c = text[i];

   while (c != '\0') {
      struct Character *ch = &characters[c];

      float xpos = x + ch->Bearing[0] * scale;
      float ypos = y - (ch->Size[1] - ch->Bearing[1]) * scale;

      float w = ch->Size[0] * scale;
      float h = ch->Size[1] * scale;
        // update VBO for each character
      float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
      };

      glGenBuffers(1, &text_VBO);
      glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindTexture(GL_TEXTURE_2D, ch->TextureID);
        // update content of VBO memory
      glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
      glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
      // render quad
      glDrawArrays(GL_TRIANGLES, 0, 6);// render glyph texture over quad
      
      // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
      x += (ch->Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
      i++;
      c = text[i];
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

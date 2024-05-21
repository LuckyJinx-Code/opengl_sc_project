
#include "main.h"

typedef struct
{
   // Handle to a program object
   GLuint programObject;

} UserData;

#define INIT_ALLOC 1024
#define MIN_SPACE   256
#define MAX_EXCESS  256

//Load text from file

char *read_file(FILE *fp)
{
    size_t offset = 0;
    size_t bufsiz  = INIT_ALLOC;
    char *buffer = malloc(bufsiz);
    if (buffer == NULL)
        return NULL;
    while (fgets(buffer + offset, bufsiz - offset, fp) != NULL)
    {
        /* Assumes data does not contain null bytes */
        /* Generic problem using fgets() */
        size_t newlen = strlen(buffer + offset);
        offset += newlen;
        if (bufsiz - offset < MIN_SPACE)
        {
            size_t new_size = bufsiz * 2;
            char  *new_data = realloc(buffer, new_size);
            if (new_data == NULL)
            {
                free(buffer);
                return NULL;
            }
            bufsiz = new_size;
            buffer = new_data;
        }
    }
    if (bufsiz - offset > MAX_EXCESS)
        buffer = realloc(buffer, offset + 1);
    return buffer;
}

void keyCallback(ESContext* esContext, unsigned char key, int x, int y) {
   input_key = key;
}

void process_input() {
   if (input_key == 'w') {
      speed_accel = ACCEL; //5 meters/s^2
   } else if (input_key == 's') {
      speed_accel = 0.0;
      speed = INITIAL_SPEED;
      rotSpeedX = 0;
      rotSpeedY = 0;
   } else if (input_key == 'd') {
      rotSpeedY = glm_rad(ROT_SPEED/60.0f);
   } else if (input_key == 'a') {
      rotSpeedY = -glm_rad(ROT_SPEED/60.0f);
   } else if (input_key == 'q') {
      rotSpeedX = glm_rad(ROT_SPEED/60.0f);
   } else if (input_key == 'e') {
      rotSpeedX = -glm_rad(ROT_SPEED/60.0f);
   } else if (input_key == 'z') {
      rotSpeedZ = -glm_rad(ROT_SPEED/60.0f);
   } else if (input_key == 'c') {
      rotSpeedZ = glm_rad(ROT_SPEED/60.0f);
   }

   speed += speed_accel/60.0f;
   posX += (speed/60.0f)*sin(rotY)*cos(rotX);
   posY -= (speed/60.0f)*sin(rotX);
   posZ += -(speed/60.0f)*cos(rotY)*cos(rotX);

   rotX = fix_angle(rotX + rotSpeedX);
   rotY = fix_angle(rotY + rotSpeedY);
   rotZ = fix_angle(rotZ + rotSpeedZ);
}

//Generate uniforms

void gen_uniforms(GLuint programObject) {
   colorLoc = glGetUniformLocation(programObject, "color");
   setBrightnessLoc = glGetUniformLocation(programObject, "setBrightness");
   modelMatrixLoc = glGetUniformLocation(programObject, "modelMatrix");
   viewMatrixLoc = glGetUniformLocation(programObject, "viewMatrix");
   projectionMatrixLoc = glGetUniformLocation(programObject, "projectionMatrix");
   hasTextureLoc = glGetUniformLocation(programObject,"hasTexture");
   //printf("Uniform! %d\n", programObject);
}

///
// Create a shader object, load the shader source, and
// compile the shader.
//
GLuint LoadShader ( GLenum type, const char *shaderSrc )
{
   GLuint shader;
   GLint compiled;
   
   // Create the shader object
   shader = glCreateShader ( type );

   if ( shader == 0 )
   	return 0;

   // Load the shader source
   glShaderSource ( shader, 1, &shaderSrc, NULL );
   
   // Compile the shader
   glCompileShader ( shader );

   // Check the compile status
   glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );

   if ( !compiled ) 
   {
      GLint infoLen = 0;

      glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
         esLogMessage ( "Error compiling shader:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteShader ( shader );
      return 0;
   }

   return shader;
}

///
// Initialize the shader and program object
//
// gcc main.c esUtil.c background.c -lGLESv2 -lEGL $(pkg-config --cflags --libs x11) -o main
int Init ( ESContext *esContext )
{
   esContext->userData = malloc(sizeof(UserData));

   UserData *userData = esContext->userData;

   FILE *vsFile = fopen("../shaders/vertex_shader.vert","r");
   char *vShaderStr = read_file(vsFile);
   
   FILE *fsFile = fopen("../shaders/fragment_shader.frag","r");
   char *fShaderStr = read_file(fsFile);

   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 )
      return 0;

   glAttachShader ( programObject, vertexShader );
   glAttachShader ( programObject, fragmentShader );

   // Bind vPosition to attribute 0   
   glBindAttribLocation ( programObject, 0, "vPosition" );
   glBindAttribLocation ( programObject, 1, "normal" );

   // Link the program
   glLinkProgram ( programObject );

   // Check the link status
   glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );

   if ( !linked ) 
   {
      GLint infoLen = 0;

      glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
      
      if ( infoLen > 1 )
      {
         char* infoLog = malloc (sizeof(char) * infoLen );

         glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
         esLogMessage ( "Error linking program:\n%s\n", infoLog );            
         
         free ( infoLog );
      }

      glDeleteProgram ( programObject );
      return GL_FALSE;
   }

   // Store the program object
   userData->programObject = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

   gen_uniforms(programObject);

   return GL_TRUE;
}

///
// Draw a triangle using the shader pair created in Init()
//

void Draw ( ESContext *esContext )
{
   UserData *userData = esContext->userData;
      
   // Set the viewport
   glViewport ( 0, 0, esContext->width, esContext->height );
   
   // Clear the color buffer
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   // Use the program object
   glUseProgram ( userData->programObject );

   process_input();

   draw_frame();
   draw_map();
   draw_compass();
   draw_SSI();
   draw_airspeed();
   draw_altimeter();
   draw_VSI();
   draw_waypoints();
   draw_destination_msg();
}

int main ( int argc, char *argv[] ) {
   ESContext esContext;
   UserData  userData;

   esInitContext ( &esContext );
   esContext.userData = &userData;

   esCreateWindow ( &esContext, "Safety-Critical System", 900, 600, ES_WINDOW_RGB );

   if ( !Init ( &esContext ) )
      return 0;

   load_font_textures();

   //gen_triangle();
   init_map();
   gen_frame();
   gen_compass();
   gen_SSI();
   gen_airspeed();
   gen_altimeter();
   gen_VSI();
   gen_waypoints();

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterKeyFunc ( &esContext, keyCallback );

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   esMainLoop ( &esContext );
}
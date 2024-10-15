
#include "main.h"

int square_VBO;

#ifdef OPENGL_ES_MODE

typedef struct
{
   // Handle to a program object
   GLuint programObject;

} UserData;

void keyCallback(ESContext* esContext, unsigned char key, int x, int y) {
   input_key = key;
}

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

void render_frame(ESContext *esContext) {

   UserData *userData = esContext->userData;
      
   glViewport ( 0, 0, esContext->width, esContext->height );
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
   glUseProgram ( userData->programObject );

   update_parameters();
   draw_widgets();
}

#else

void render_frame() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   update_parameters();
   //draw_widgets();

   //cle("Painted successfully :)\n");

   glReadnPixels(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 
            GL_RGBA, GL_UNSIGNED_BYTE, WINDOW_WIDTH*WINDOW_HEIGHT*4, pixel_map);
   //SDL_UpdateTexture(texture, NULL, pixel_map, WINDOW_WIDTH * sizeof(unsigned int));
   draw(square_VBO, 6, GL_TRIANGLES);

   // Render the texture
   SDL_RenderClear(renderer);
   (renderer, texture, NULL, NULL);
   SDL_RenderPresent(renderer);
}

#endif

//Generate uniforms

void gen_uniforms(GLuint programObject) {
   colorLoc = glGetUniformLocation(programObject, "color");
   modelMatrixLoc = glGetUniformLocation(programObject, "modelMatrix");
   viewMatrixLoc = glGetUniformLocation(programObject, "viewMatrix");
   projectionMatrixLoc = glGetUniformLocation(programObject, "projectionMatrix");
   normalMatrixLoc = glGetUniformLocation(programObject, "normalMatrix");
   enabledFlagsLoc = glGetUniformLocation(programObject, "enabledFlags");
   //printf("Uniform! %d\n", programObject);
}

void init_window() {
   #ifdef OPENGL_ES_MODE
   esInitContext ( &esContext );
   esCreateWindow ( &esContext, "Safety-Critical System", WINDOW_WIDTH, WINDOW_HEIGHT, ES_WINDOW_RGB );

   esContext.userData = malloc(sizeof(UserData));

   UserData *userData = esContext.userData;

   FILE *vsFile = fopen("shaders/vertex_shader.vert","r");
   char *vShaderStr = read_file(vsFile);

   FILE *fsFile = fopen("shaders/fragment_shader.frag","r");
   char *fShaderStr = read_file(fsFile);

   GLuint programObject;
   GLint linked;

   // Load the vertex/fragment shaders
   vertexShader = LoadShader ( GL_VERTEX_SHADER, vShaderStr );
   fragmentShader = LoadShader ( GL_FRAGMENT_SHADER, fShaderStr );

   // Create the program object
   programObject = glCreateProgram ( );
   
   if ( programObject == 0 ) {
      printf("Could not create program.\n");
      return;
   }

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
      printf("Error: could not link program.\n");
      exit(1);
   }

   // Store the program object
   userData->programObject = programObject;

   glClearColor ( 0.0f, 0.0f, 0.0f, 1.0f );

   gen_uniforms(programObject);

   //gen_triangle();

   init_map();
   gen_frame();
   gen_compass();
   gen_SSI();
   gen_airspeed();
   gen_altimeter();
   gen_VSI();
   gen_waypoints();
   gen_CDI();
   gen_wp_info();

   load_font_textures();

   esRegisterDrawFunc ( &esContext, render_frame );

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   gettimeofday(&exec_time, NULL);
   gettimeofday(&prev_time, NULL);

   esMainLoop ( &esContext );
   
   #else

   pixel_map = malloc((int)WINDOW_WIDTH * (int)WINDOW_HEIGHT * sizeof(uint32_t));

   if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        /* Failed, exit. */
        fprintf( stderr, "Video initialization failed: %s\n",
             SDL_GetError( ) );
        exit(1);
    }

   window = SDL_CreateWindow("SDL Animation",
   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
   WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
   if (!window) {
      printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
      exit(1);
   }

   renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if (!renderer) {
      printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
      exit(1);
   }

   texture = 
      SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
      SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

   if (!texture) {
      printf("Texture could not be created! SDL_Error: %s\n", SDL_GetError());
      exit(1);
   }

   GLuint framebuffer, colorbuffer, depthbuffer;

   // Set Up Frame Context
   glGenFramebuffers(1, &framebuffer);
   glGenRenderbuffers(1, &colorbuffer);
   glGenRenderbuffers(1, &depthbuffer);

   glBindRenderbuffer(GL_RENDERBUFFER, colorbuffer);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA4, WINDOW_WIDTH, WINDOW_HEIGHT);

   glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, WINDOW_WIDTH, WINDOW_HEIGHT);

   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbuffer);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer);

   program = glCreateProgram();

   FILE *kernel_file = fopen("kernel.ocl","r");
    
   fseek(kernel_file , 0 , SEEK_END);
   long fsize = ftell(kernel_file);
   rewind(kernel_file);
    
   void* data = malloc(fsize);
   size_t size = fread(data, 1, fsize, kernel_file);
    
   fclose(kernel_file);
    
   glProgramBinary(program, 0, data, size);

   //LINK_PROGRAM(program, "kernel");
   glUseProgram(program);
    
   // Set Up Vertex Attributes
   vPositionLoc  = glGetAttribLocation(program, "vPosition");
   normalLoc     = glGetAttribLocation(program, "normal");
   vTexCoordsLoc     = glGetAttribLocation(program, "vTexCoords");

   //init_map();
   gen_frame();
   gen_compass();
   gen_SSI();
   gen_airspeed();
   gen_altimeter();
   gen_VSI();
   gen_waypoints();
   gen_CDI();
   gen_wp_info();

   colorLoc = glGetUniformLocation(program,"color");
   modelMatrixLoc = glGetUniformLocation(program,"modelMatrix");
   viewMatrixLoc = glGetUniformLocation(program,"viewMatrix");
   projectionMatrixLoc = glGetUniformLocation(program,"projectionMatrix");
   normalMatrixLoc = glGetUniformLocation(program,"normalMatrix");
   enabledFlagsLoc = glGetUniformLocation(program,"enabledFlags");

   unsigned int frameDelay = 1000 / 60;
   unsigned int frameStart;
   int frameTime;

   int quit = 0;
   SDL_Event e;

   float posX = 0.0f;

   int curr_time = 0;
   int num_frames = 0;

   //PRUEBAAAA
   
   
    float square_vertices[18];

    float rect_width = 100.0f;
    float rect_height = 60.0f;

    gen_rectangle(rect_width, rect_height, (vec2) {0.0f,0.0f}, 0, 0, square_vertices);
    add_to_buffer(&square_VBO, square_vertices, sizeof(square_vertices));
    set_color(WHITE);
    //printf("Drawing square :)\n");
    draw(square_VBO, 6, GL_TRIANGLES);



   // Main loop
   while (!quit) {
      frameStart = SDL_GetTicks();  // Start the frame timer

      // Handle events
      while (SDL_PollEvent(&e) != 0) {
         if (e.type == SDL_QUIT) {
               quit = 1;
         }
      }

      render_frame();

      frameTime = SDL_GetTicks() - frameStart;
      if (frameDelay > frameTime) {
         SDL_Delay(frameDelay - frameTime);
      }
        
      num_frames++;
      int new_time = curr_time + frameTime;
      if (new_time/1000 != curr_time/1000) {
         printf("FPS: %d\n", num_frames);
         num_frames = 0;
      }
      curr_time = new_time;
    }

    // Clean up
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    free(pixel_map);

   #endif
}

int main ( int argc, char *argv[] ) {

   if (argc != 2 && argc != 3) {
      printf("Usage: main <json_file> [runtime_in_seconds]\n");
      return 1;
   }

   //Read JSON

   const char *filename = argv[1];

   if (!file_exists(filename)) {
      printf("Error: File '%s' does not exist.\n", filename);
      return 1;
   }

   if (strlen(filename) < 5 || strcmp(filename + strlen(filename) - 5, ".json") != 0) {
      printf("Error: File '%s' is not a valid JSON file.\n", filename);
      return 1;
   }

   FILE *jsonFile = fopen(filename,"r");
   char *jsonStr = read_file(jsonFile);

   read_config_json(jsonStr);

   //Runtime seconds

   limit_set = argc == 3;
   if (limit_set) time_limit = atoi(argv[2]);
   
   init_window();

   return 0;
}

float4 mul(float16 mat, float4 vec) {
  float4 result = 0;

  for(int i=0; i<16; ++i) {
    result[i%4] += mat[i]*vec[i/4]; 
  }

  return result;
}

float3 toFloat3(float4 vec4) {
  float3 vec3 = 0;
  for (int i = 0; i < 3; i++) vec3[i] = vec4[i];
  return vec3;
}

float4 toFloat4(float3 vec3) {
  float4 vec4 = 0;
  for (int i = 0; i < 3; i++) vec4[i] = vec3[i];
  vec4[3] = 1.0f; 
  return vec4;
}

float4 vec4mul(float4 a, float4 b){
  float4 result = 0;
  for (int i = 0; i < 4; i++) result[i] = a[i] * b[i];
  return result;
}

#define SET_BRIGHTNESS_FLAG 0
#define HAS_TEXTURE_FLAG 1

kernel void main_vs (
  //Uniforms
  constant float4 *color,
  constant float16 *modelMatrix,
  constant float16 *viewMatrix,
  constant float16 *projectionMatrix,
  constant int2 *enabledFlags,
  //Attributes
  global const float4 *vPosition,
  global const float3 *normal,
  global const float2 *vTexCoords,
  //Out variables
  global float4 *out_color,
  global float *brightness,
  global float2 *texCoords,
  global float *hasTexCoords,
  global float4 *gl_Position
) {
  int gid = get_global_id(0);

  if ((*enabledFlags)[SET_BRIGHTNESS_FLAG] == 1) {
    float3 vertexPos = 
      toFloat3(mul(*modelMatrix, toFloat4(toFloat3(*vPosition))));
    float3 vertexCam = toFloat3(*vPosition) - vertexPos;
    float perp = sqrt(1.0-dot(*normal,vertexCam)/(length(*normal)*length(vertexCam)));
    brightness[gid] = perp;
  } else brightness[gid] = 1.0f;

  gl_Position[gid] = 
      mul(*projectionMatrix, mul(*viewMatrix,mul(*modelMatrix,toFloat4(toFloat3(vPosition[gid])))));
  out_color[gid] = *color;
  texCoords[gid] = *vTexCoords;
  hasTexCoords[gid] = (*enabledFlags)[HAS_TEXTURE_FLAG];
}

typedef global unsigned char* image_t;

typedef struct {
    ushort s, t, min_filter, mag_filter;
} texture_wraps_t;

typedef struct __attribute__((packed)) {
  uint width, height;
  uint internalformat;
  texture_wraps_t wraps;
} sampler2D_t;

kernel void main_fs (
  //Uniforms
  constant float4 *color,
  //In variables
  global const float2 *texCoords,
  global const float4 *out_color,
  global const float *brightness,
  global const float *hasTexCoords,
  //Out variables
  global float4 *gl_FragColor,
  //Others
  sampler2D_t texture,
  image_t image
)
{
  int gid = get_global_id(0);
  
  
  if (hasTexCoords[gid] == 1) {

    float2 coord = 0;
    coord.x = (texCoords[gid].x - floor(texCoords[gid].x));
    coord.y = (texCoords[gid].y - floor(texCoords[gid].y));

    uint width   = texture.width * coord.x;
    uint height  = texture.height * coord.y;

    global uchar* texPixelColor = image + (height * texture.width + width)*4;
    float4 pixelRGBA = 0;
    pixelRGBA[0] = (float)*texPixelColor;
    pixelRGBA[1] = (float)*(texPixelColor + 1);
    pixelRGBA[2] = (float)*(texPixelColor + 2);
    pixelRGBA[3] = (float)*(texPixelColor + 3);
    
    gl_FragColor[gid] = vec4mul(pixelRGBA, out_color[gid]);
  } else {
    gl_FragColor[gid][0] = out_color[gid][0];
    gl_FragColor[gid][1] = out_color[gid][1];
    gl_FragColor[gid][2] = out_color[gid][2];
    gl_FragColor[gid][3] = out_color[gid][3];
  }
}

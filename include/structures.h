#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_ID_SIZE 32

#include <time.h>

typedef struct {
  char id[MAX_ID_SIZE];
  float posX;
  float posZ;
  time_t arrival_time;
} Waypoint;

typedef struct {
   char id;
   unsigned int VBO;
   unsigned int width;
   unsigned int height;
   unsigned int advance;
   unsigned int baseline;
} Character;

struct Widget {
  unsigned int id;
  int posX;
  int posY;
  struct Widget* next;
};

typedef struct {
  struct Widget* first;
  unsigned int size;
} Widgets;

struct Controller {
  unsigned int parameter;
  unsigned int easing;
  float first_value;
  float last_value;
  float duration;
  unsigned int loop_mode;
  struct Controller *next;
};

typedef struct {
  struct Controller* first;
  unsigned int size;
} Controllers;

struct Waypoint {
  char* id;
  float posX;
  float posZ;
  time_t arrival_time;
  struct Waypoint* next;
};

typedef struct {
  struct Waypoint* first;
  unsigned int size;
} Waypoints;

#endif // STRUCTURES_H
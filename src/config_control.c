#include "config_control.h"

#define COORD_LIMIT 10000000.0f
#define SPEED_LIMIT 1000.0f
#define ROT_LIMIT 3.141592653589793238462643383279502884197f * 2

void add_widget(int id, int posX, int posY) {
    // Allocate memory for the new Widget
    struct Widget* new_widget = (struct Widget*)malloc(sizeof(struct Widget));
    if (new_widget == NULL) {
        // Handle memory allocation failure if necessary
        return;
    }

    // Initialize the new Widget
    new_widget->id = id;
    new_widget->posX = posX;
    new_widget->posY = posY;
    new_widget->next = NULL;

    // Insert the new Widget at the beginning of the list

    if (widgets.size == 0) {
        widgets.first = new_widget;
    } else {
        struct Widget* curr_widget = widgets.first;
        for (int i = 1; i < widgets.size; i++)
            curr_widget = curr_widget->next;
        curr_widget->next = new_widget;
    }

    // Increment the size of the Widgets
    widgets.size++;
}

void add_controller(
    unsigned int parameter, unsigned int easing, float first_value, float last_value,
    float duration, unsigned int loop_mode) {
    
    struct Controller* new_controller = (struct Controller*)malloc(sizeof(struct Controller));
    if (new_controller == NULL) {
        // Handle memory allocation failure if necessary
        return;
    }

    // Initialize the new Widget
    new_controller->parameter = parameter;
    new_controller->easing = easing;
    new_controller->first_value = first_value;
    new_controller->last_value = last_value;
    new_controller->duration = duration;
    new_controller->loop_mode = loop_mode;

    // Insert the new Widget at the beginning of the list

    if (controllers.size == 0) {
        controllers.first = new_controller;
    } else {
        struct Controller* curr_controller = controllers.first;
        for (int i = 1; i < controllers.size; i++)
            curr_controller = curr_controller->next;
        curr_controller->next = new_controller;
    }

    // Increment the size of the Widgets
    controllers.size++;
}

void add_waypoint(char* id, int posX, int posZ, time_t arrival_time) {
    // Allocate memory for the new Widget
    struct Waypoint* new_waypoint = (struct Waypoint*)malloc(sizeof(struct Waypoint));
    if (new_waypoint == NULL) {
        // Handle memory allocation failure if necessary
        return;
    }

    // Initialize the new waypoint
    new_waypoint->id = id;
    new_waypoint->posX = posX;
    new_waypoint->posZ = posZ;
    new_waypoint->arrival_time = arrival_time;
    new_waypoint->next = NULL;

    // Insert the new Widget at the beginning of the list

    if (waypoints.size == 0) {
        waypoints.first = new_waypoint;
    } else {
        struct Waypoint* curr_waypoint = waypoints.first;
        for (int i = 1; i < waypoints.size; i++)
            curr_waypoint = curr_waypoint->next;
        curr_waypoint->next = new_waypoint;
    }

    // Increment the size of the Widgets
    waypoints.size++;
}

void update_parameters() {
    struct timeval curr_time;
    gettimeofday(&curr_time, NULL);
    
    delta_time = (float)(curr_time.tv_sec - prev_time.tv_sec + 
            (curr_time.tv_usec - prev_time.tv_usec) * 1e-6);

    float passed_time = 
            (float)(curr_time.tv_sec - exec_time.tv_sec + 
            (curr_time.tv_usec - exec_time.tv_usec) * 1e-6);

    struct Controller* curr_controller = controllers.first;
    for (int i = 0; i < controllers.size; i++) {
        float a = curr_controller->first_value;
        float b = curr_controller->last_value;
        unsigned int easing = curr_controller->easing;
        unsigned int loop_mode = curr_controller->loop_mode;
        unsigned int parameter = curr_controller->parameter;
        float duration = curr_controller->duration;
        float x = passed_time/duration;

        curr_controller = curr_controller->next;

        int is_odd = ((int)x)%2 == 0;

        x = 
            loop_mode == RESTART_LOOPMODE ? mod(x,1.0f) :
            loop_mode == REVERSE_LOOPMODE ? (is_odd ? mod(x,1.0f) : 1 - mod(x,1.0f)) :
            loop_mode == NOLOOP_LOOPMODE ? (x > 1 ? 1 : x) :
        x; 

        float actual_value =
            easing == LINEAR_EASING ? a + (b - a)*x :
            easing == QUAD_EASING ? a + (b - a)*x*x :
            easing == SINUSOID_EASING ? a + (b - a) * (1 - sin(GLM_PI/2 + x*GLM_PI))/2 :
            easing == COSINUSOID_EASING ? a + (b - a) * (1 - cos(GLM_PI/2 + x*GLM_PI))/2 :
        a;

        float dev_actual_value = (
            easing == LINEAR_EASING ? b - a :
            easing == QUAD_EASING ? 2*(b - a)*x :
            easing == SINUSOID_EASING ? GLM_PI*(b - a)*sin(GLM_PI*x)/2:
            easing == COSINUSOID_EASING ? GLM_PI*(b - a)*cos(GLM_PI*x)/2:
        0)/duration;
        

        //printf("Loop: %d Easing: %d Param: %d\n",
        //    loop_mode, easing, parameter);
       
        switch (parameter) {
            case POSX_PARAM:
                pos[0] = max(min(actual_value, COORD_LIMIT), -COORD_LIMIT);
                speed[0] = max(min(dev_actual_value, SPEED_LIMIT), -SPEED_LIMIT);
                speed[0] *= loop_mode != REVERSE_LOOPMODE || !is_odd ? 1 : -1;
                break;
            case POSY_PARAM:
                pos[1] = max(min(actual_value, COORD_LIMIT), -COORD_LIMIT);
                speed[1] = max(min(dev_actual_value, SPEED_LIMIT), -SPEED_LIMIT);;
                speed[1] *= loop_mode != REVERSE_LOOPMODE || !is_odd ? 1 : -1;
                break;
            case POSZ_PARAM:
                pos[2] = max(min(actual_value, COORD_LIMIT), -COORD_LIMIT);
                speed[2] = max(min(dev_actual_value, SPEED_LIMIT), -SPEED_LIMIT);;
                speed[2] *= loop_mode != REVERSE_LOOPMODE || !is_odd ? 1 : -1;
                break;
            case ROTX_PARAM:
                rotX = mod(actual_value, GLM_PI*2);
                break;
            case ROTY_PARAM:
                rotY = mod(actual_value, GLM_PI*2);
                break;
            case ROTZ_PARAM:
                rotZ = mod(actual_value, GLM_PI*2);
                break;
        }
    }

    //printf("Speed: %0.6f, %0.6f, %0.6f\n", speed[0], speed[1], speed[2]);

    prev_pos[0] = pos[0];
    prev_pos[1] = pos[1];
    prev_pos[2] = pos[2];

    gettimeofday(&prev_time, NULL);

    if (limit_set && passed_time > time_limit)
        exit(1);

    //speed = sqrt()
}

void draw_widgets() {
    //struct Widget* curr_widget = widgets.first;
    //for (int i = 0; i < )

    if (is_terrain_active) {
        draw_frame();
        draw_map();
        draw_destination_msg();
    }

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);

    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);
    glUniform2i(enabledFlagsLoc, 2, enabledFlags[0]);

    struct Widget* curr_widget = widgets.first;
    for (int i = 0; i < widgets.size; i++) {
        float vPosX = curr_widget->posX;
        float vPosY = curr_widget->posY;
        //printf("%d: drawing component %d.\n",i,curr_widget->id);
        
        switch (curr_widget->id) {
            case VSI_WIDGET:
                draw_VSI(vPosX,vPosY);
                break;
            case CDI_WIDGET:
                draw_CDI(vPosX,vPosY);
                break;
            case AIRSPEED_WIDGET:
                draw_airspeed(vPosX,vPosY);
                break;
            case ALTIMETER_WIDGET:
                draw_altimeter(vPosX,vPosY);
                break;
            case COMPASS_WIDGET:
                draw_compass(vPosX,vPosY);
                break;
            case SSI_WIDGET:
                draw_SSI(vPosX,vPosY);
                break;
            case WAYPOINTS_WIDGET:
                draw_waypoints(vPosX,vPosY);
                break;
            case WPINFO_WIDGET:
                draw_wp_info(vPosX,vPosY);
                break;
        }
        curr_widget = curr_widget->next;
    }
}
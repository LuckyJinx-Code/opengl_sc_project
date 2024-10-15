#include "input.h"

void read_config_json(char* json_text) {
    //printf("JSON: %s\n",json_text);

    cJSON *json = cJSON_Parse(json_text);
    if (json == NULL) {
        printf("Wrong JSON format.\n");
        return;
    }

    //Background configuration

    cJSON *background = cJSON_GetObjectItem(json, "terrain");
    if (background) {
        cJSON *js_vert_square = cJSON_GetObjectItem(background, "vertices_per_square");
        cJSON *js_sqs_chunk = cJSON_GetObjectItem(background, "squares_per_chunk");
        cJSON *js_chk_size = cJSON_GetObjectItem(background, "chunk_size");
        cJSON *js_terr_diam = cJSON_GetObjectItem(background, "terrain_diameter");
        cJSON *js_mount_height = cJSON_GetObjectItem(background, "mountain_max_height");
        if (js_vert_square && js_sqs_chunk &&
            js_chk_size && js_terr_diam && js_mount_height) {
            is_terrain_active = 1;
            vertices_per_square = js_vert_square->valueint;
            squares_per_chunk = js_sqs_chunk->valueint;
            chunk_size = js_chk_size->valuedouble;
            terrain_diameter = js_terr_diam->valueint;
            mountain_max_height = js_mount_height->valuedouble;
        }
    }

    cJSON *js_controllers = cJSON_GetObjectItem(json, "controllers");
    if (js_controllers) {
        int cont_count = cJSON_GetArraySize(js_controllers);
        for (int i = 0; i < cont_count; i++) {
            cJSON *js_controller = cJSON_GetArrayItem(js_controllers, i);
            cJSON *js_param = cJSON_GetObjectItem(js_controller, "parameter");
            cJSON *js_easing = cJSON_GetObjectItem(js_controller, "easing");
            cJSON *js_firstValue = cJSON_GetObjectItem(js_controller, "first_value");
            cJSON *js_lastValue = cJSON_GetObjectItem(js_controller, "last_value");
            cJSON *js_duration = cJSON_GetObjectItem(js_controller, "duration");
            cJSON *js_loopMode = cJSON_GetObjectItem(js_controller, "loop_mode");

            if (js_controller && js_param && js_easing && js_firstValue
                && js_lastValue && js_duration && js_loopMode) {

                char* param = js_param->valuestring;
                char* easing = js_easing->valuestring;
                float first_value = js_firstValue->valuedouble;
                float last_value = js_lastValue->valuedouble;
                float duration = js_duration->valuedouble;
                char* loop_mode = js_loopMode->valuestring;

                unsigned int param_id = 
                    strcmp(param,"posX") == 0 ? POSX_PARAM :
                    strcmp(param,"posY") == 0 ? POSY_PARAM :
                    strcmp(param,"posZ") == 0 ? POSZ_PARAM :
                    strcmp(param,"rotX") == 0 ? ROTX_PARAM :
                    strcmp(param,"rotY") == 0 ? ROTY_PARAM :
                    strcmp(param,"rotZ") == 0 ? ROTZ_PARAM :
                    0;

                unsigned int easing_id =
                    strcmp(easing,"linear") == 0 ? LINEAR_EASING :
                    strcmp(easing,"quadratic") == 0 ? QUAD_EASING :
                    strcmp(easing,"sinusoid") == 0 ? SINUSOID_EASING :
                    strcmp(easing,"cosinusoid") == 0 ? COSINUSOID_EASING :
                    0;
                
                unsigned int loop_id =
                    strcmp(loop_mode, "restart") == 0 ? RESTART_LOOPMODE :
                    strcmp(loop_mode, "reverse") == 0 ? REVERSE_LOOPMODE :
                    strcmp(loop_mode, "endless") == 0 ? ENDLESS_LOOPMODE :
                    strcmp(loop_mode, "no_loop") == 0 ? NOLOOP_LOOPMODE :
                    0;

                add_controller(param_id, easing_id, first_value, last_value, duration, loop_id);
            }
        }
    }

    cJSON *components = cJSON_GetObjectItem(json, "components");
    if (components) {
        int comp_count = cJSON_GetArraySize(components);
        for (int i = 0; i < comp_count; i++) {
            cJSON *component = cJSON_GetArrayItem(components, i);
            cJSON *js_name = cJSON_GetObjectItem(component, "component_name");
            cJSON *js_posX = cJSON_GetObjectItem(component, "posX");
            cJSON *js_posY = cJSON_GetObjectItem(component, "posY");
            if (js_name && js_posX && js_posY) {
                //printf("Component %d Name: %s\n", i + 1, component_name->valuestring);
                char* comp_name = js_name->valuestring;
                int posX = js_posX->valueint;
                int posY = js_posY->valueint;
                if (strcmp(comp_name,"VSI") == 0)
                    add_widget(VSI_WIDGET,posX,posY);
                else if (strcmp(comp_name,"CDI") == 0)
                    add_widget(CDI_WIDGET,posX,posY);
                else if (strcmp(comp_name,"Airspeed") == 0)
                    add_widget(AIRSPEED_WIDGET,posX,posY);
                else if (strcmp(comp_name,"Altimeter") == 0)
                    add_widget(ALTIMETER_WIDGET,posX,posY);
                else if (strcmp(comp_name,"Compass") == 0)
                    add_widget(COMPASS_WIDGET,posX,posY);
                else if (strcmp(comp_name,"SSI") == 0)
                    add_widget(SSI_WIDGET,posX,posY);
                else if (strcmp(comp_name,"Waypoints") == 0)
                    add_widget(WAYPOINTS_WIDGET,posX,posY);
                else if (strcmp(comp_name,"WaypointInfo") == 0)
                    add_widget(WPINFO_WIDGET,posX,posY);
            }
        }
    }

    cJSON *waypoints = cJSON_GetObjectItem(json, "waypoints");
    if (waypoints) {
        int wp_count = cJSON_GetArraySize(waypoints);
        for (int i = 0; i < wp_count; i++) {
            cJSON *js_waypoint = cJSON_GetArrayItem(waypoints, i);
            cJSON *js_id = cJSON_GetObjectItem(js_waypoint, "id");
            cJSON *js_posX = cJSON_GetObjectItem(js_waypoint, "coord_X");
            cJSON *js_posZ = cJSON_GetObjectItem(js_waypoint, "coord_Y");
            cJSON *js_arrival_time = cJSON_GetObjectItem(js_waypoint, "arrival_time");
            if (js_id && js_posX && js_posZ && js_arrival_time) {
                //printf("Component %d Name: %s\n", i + 1, component_name->valuestring);
                char* wp_id = js_id->valuestring;
                int posX = js_posX->valueint;
                int posZ = js_posZ->valueint;
                time_t arrival_time = js_arrival_time->valueint;
                add_waypoint(wp_id, posX, posZ, arrival_time);
            }
        }
    }
}
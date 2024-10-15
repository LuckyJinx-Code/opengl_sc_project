#include <stdio.h>

#include "waypoints.h"

void gen_waypoints() {
    //Generating box

    float window_vertices[27];

    gen_octagon(
        WP_WINDOW_WIDTH,
        WP_WINDOW_HEIGHT,
        WP_WINDOW_CATET,
        (vec2){0.0f,0.0f},
        window_vertices
    );

    add_to_buffer(&wp_window_VBO, window_vertices, sizeof(window_vertices));

    //Generating main radius area

    float rad_outer_vertices[6 + 6*(WP_OUTER_RANGE_NUM_SIDES - 1) + 3];

    gen_arc_section(
        WP_NAV_RADIUS,
        WP_NAV_RADIUS - WP_RANGE_LINE_THICKNESS,
        (vec2){0.0f,0.0f},
        0.0f,
        2*GLM_PI,
        WP_OUTER_RANGE_NUM_SIDES,
        rad_outer_vertices
    );

    add_to_buffer(&wp_radar_outer_VBO, rad_outer_vertices, sizeof(rad_outer_vertices));

    //Generating main radius lines area

    float rad_outer_lines_vertices[18*72];

    for (int i = 0; i < 72; i++) {
        float rect_vertices[18];

        gen_rectangle(
            i%2 == 0 ? WP_OUTER_RANGE_BLINE_SIZE : WP_OUTER_RANGE_SLINE_SIZE,
            WP_RANGE_LINE_THICKNESS,
            (vec2){0.0f,0.0f},
            WP_NAV_RADIUS + (i%2 == 0 ? WP_OUTER_RANGE_BLINE_SIZE : WP_OUTER_RANGE_SLINE_SIZE)/2.0f,
            2*GLM_PI*i/72,
            rect_vertices
        );

        for (int j = 0; j < 18; j++) {
            rad_outer_lines_vertices[18*i + j] = rect_vertices[j];
        }
    }

    add_to_buffer(&wp_radar_outer_lines_VBO, rad_outer_lines_vertices, sizeof(rad_outer_lines_vertices));

    float rad_inner_1_vertices[18*36];

    for (int i = 0; i < 36; i++) {
        float rect_vertices[18];

        gen_rectangle(
            WP_RANGE_LINE_THICKNESS,
            GLM_PI*WP_NAV_RADIUS/72.0f,
            (vec2){0.0f,0.0f},
            WP_NAV_RADIUS*(2.0f/3.0f),
            2*GLM_PI*i/36.0f,
            rect_vertices
        );

        for (int j = 0; j < 18; j++) {
            rad_inner_1_vertices[18*i + j] = rect_vertices[j];
        }
    }
    
    add_to_buffer(&wp_radar_inner_1_VBO, rad_inner_1_vertices, sizeof(rad_inner_1_vertices));

    float rad_inner_2_vertices[18*36];

    for (int i = 0; i < 18; i++) {
        float rect_vertices[18];

        gen_rectangle(
            WP_RANGE_LINE_THICKNESS,
            GLM_PI*WP_NAV_RADIUS/72.0f,
            (vec2){0.0f,0.0f},
            WP_NAV_RADIUS*(1.0f/3.0f),
            2*GLM_PI*i/18.0f,
            rect_vertices
        );

        for (int j = 0; j < 18; j++) {
            rad_inner_2_vertices[18*i + j] = rect_vertices[j];
        }
    }

    add_to_buffer(&wp_radar_inner_2_VBO, rad_inner_2_vertices, sizeof(rad_inner_2_vertices));
    
    //Waypoint icon

    float wp_io_vertices[12] = {
        -WP_ICON_OUTER_SIZE/2, 0.0f, 0.0f,
        0.0f, WP_ICON_OUTER_SIZE/2, 0.0f,
        WP_ICON_OUTER_SIZE/2, 0.0f, 0.0f,
        0.0f, -WP_ICON_OUTER_SIZE/2, 0.0f
    };

    add_to_buffer(&wp_icon_outer_VBO, wp_io_vertices, sizeof(wp_io_vertices));

    float wp_ii_vertices[12] = {
        -WP_ICON_INNER_SIZE/2, 0.0f, 0.0f,
        0.0f, WP_ICON_INNER_SIZE/2, 0.0f,
        WP_ICON_INNER_SIZE/2, 0.0f, 0.0f,
        0.0f, -WP_ICON_INNER_SIZE/2, 0.0f
    };

    add_to_buffer(&wp_icon_inner_VBO, wp_ii_vertices, sizeof(wp_ii_vertices));

    //Link between waypoints

    float link_vertices[18];

    gen_rectangle(
        1.0f,
        WP_ICON_LINK_THICKNESS,
        (vec2){0.0f,0.0f},
        0.0f,
        0.0f,
        link_vertices
    );

    add_to_buffer(&wp_link_VBO, link_vertices, sizeof(link_vertices));

    float plane_icon_vertice[9] = {
        -WP_PLANE_ICON_WIDTH/2, -WP_PLANE_ICON_HEIGHT/2.0f, 0.0f,
        WP_PLANE_ICON_WIDTH/2, -WP_PLANE_ICON_HEIGHT/2.0f, 0.0f,
        0.0f, WP_PLANE_ICON_HEIGHT/2.0f, 0.0f
    };

    add_to_buffer(&wp_plane_icon_VBO, plane_icon_vertice, sizeof(plane_icon_vertice));
}

void draw_waypoints(float vPosX, float vPosY) {

    //Drawing inner window

    reset_modelmatrix();
    set_position(vPosX, vPosY);

    set_default_projection();

    set_color((vec4){0.0f,0.0f,0.0f,WP_WINDOW_ALPHA});
    set_texture(0);

    enable_stencil_test();
    enable_stencil_buffer_changes();
    
    draw(wp_window_VBO,9,GL_TRIANGLE_FAN);
    disable_stencil_buffer_changes();

    //Drawing radius lines

    pass_stencil_if_1();

    set_position(0.0f, WP_RANGE_CENTER_POS_Y);
    rotate(rotY);
    set_color(WHITE);

    draw(wp_radar_outer_VBO,3 + 3*(WP_OUTER_RANGE_NUM_SIDES - 1) + 1,GL_TRIANGLE_STRIP);
    draw(wp_radar_outer_lines_VBO,6*72,GL_TRIANGLES);
    draw(wp_radar_inner_1_VBO,6*36,GL_TRIANGLES);
    draw(wp_radar_inner_2_VBO,6*18,GL_TRIANGLES);

    //Drawing degree units

    set_color(WHITE);

    float unit_radius = 
        WP_NAV_RADIUS + WP_OUTER_RANGE_BLINE_SIZE + WP_NAV_UNITS_FONT_SIZE*25.0f;

    for (int i = 0; i < 36; i++) {
        set_position(0.0f, unit_radius);
        char num_text[3];
        sprintf(num_text,"%d",i);
        render_text(num_text, WP_NAV_UNITS_FONT_SIZE, 0);
        set_position(0.0f, -unit_radius);
        rotate(-2*GLM_PI/36);
    }

    //Drawing waypoint icons

    //View units per kilometer
    float vu_per_km = WP_NAV_RADIUS/WP_DISPLAYED_KM;

    float mapXList[waypoints.size];
    float mapZList[waypoints.size];

    struct Waypoint *curr_waypoint = waypoints.first;

    for (int i = 0; i < waypoints.size; i++) {
        float distX = (curr_waypoint->posX - pos[0]);
        float distZ = -(curr_waypoint->posZ - pos[2]);

        float coordX = distX*cos(rotY) - distZ*sin(rotY);
        float coordZ = distX*sin(rotY) + distZ*cos(rotY);

        mapXList[i] = floor(coordX*vu_per_km);
        mapZList[i] = floor(coordZ*vu_per_km);

        curr_waypoint = curr_waypoint->next;
    }

    curr_waypoint = waypoints.first;

    for (int i = 1; i < waypoints.size; i++) {
        float linkPosX = (mapXList[i - 1] + mapXList[i])/2.0f;
        float linkPosZ = (mapZList[i - 1] + mapZList[i])/2.0f;

        float x = mapXList[i] - mapXList[i - 1];
        float y = mapZList[i] - mapZList[i - 1];
        float h = sqrt(x*x + y*y);
        float rot = atan(y/x);
            
        reset_modelmatrix();
        set_position(vPosX, vPosY + WP_RANGE_CENTER_POS_Y);
        set_position(linkPosX,linkPosZ);
        rotate(rot);
        resize(h,1.0f);
        set_color(PURPLE);
        draw(wp_link_VBO,6,GL_TRIANGLES);

        curr_waypoint = curr_waypoint->next;
    }

    curr_waypoint = waypoints.first;

    for (int i = 0; i < waypoints.size; i++) {
        //printf("Distance: (%0.5f,%0.5f)\n",curr_waypoint->posX,curr_waypoint->posZ);

        reset_modelmatrix();
        set_position(vPosX, vPosY + WP_RANGE_CENTER_POS_Y);
        set_position(mapXList[i],mapZList[i]);

        set_color(BLACK);
        draw(wp_icon_outer_VBO,4,GL_TRIANGLE_FAN);
        resize(
            (WP_ICON_OUTER_SIZE - WP_ICON_STROKE*2.0f)/WP_ICON_OUTER_SIZE,
            (WP_ICON_OUTER_SIZE - WP_ICON_STROKE*2.0f)/WP_ICON_OUTER_SIZE
        );
        set_color(PURPLE);
        draw(wp_icon_outer_VBO,4,GL_TRIANGLE_FAN);
        resize(
            WP_ICON_OUTER_SIZE/(WP_ICON_OUTER_SIZE - WP_ICON_STROKE*2.0f),
            WP_ICON_OUTER_SIZE/(WP_ICON_OUTER_SIZE - WP_ICON_STROKE*2.0f)
        );
        set_color(BLACK);
        draw(wp_icon_inner_VBO,4,GL_TRIANGLE_FAN);
        resize(
            (WP_ICON_INNER_SIZE - WP_ICON_STROKE*2.0f)/WP_ICON_INNER_SIZE,
            (WP_ICON_INNER_SIZE - WP_ICON_STROKE*2.0f)/WP_ICON_INNER_SIZE
        );
        set_color(PURPLE);
        draw(wp_icon_inner_VBO,4,GL_TRIANGLE_FAN);

        curr_waypoint = curr_waypoint->next;
    }

    //Drawing plane icon
    
    reset_modelmatrix();
    set_position(
        vPosX,
        vPosY + WP_RANGE_CENTER_POS_Y
    );
    set_color(BLACK);
    draw(wp_plane_icon_VBO,3,GL_TRIANGLES);
    resize(
        WP_PLANE_ICON_WIDTH/(WP_PLANE_ICON_WIDTH + WP_PLANE_ICON_STROKE*2),
        WP_PLANE_ICON_HEIGHT/(WP_PLANE_ICON_HEIGHT + WP_PLANE_ICON_STROKE*2)
    );
    set_position(
        0.0f,
        -WP_PLANE_ICON_STROKE/2
    );
    set_color(YELLOW);
    draw(wp_plane_icon_VBO,3,GL_TRIANGLES);

    //Drawing outer box

    reset_modelmatrix();
    set_position(vPosX, vPosY);
    pass_stencil_if_0();

    resize(
        (WP_WINDOW_WIDTH + WP_WINDOW_STROKE_THICKNESS*2.0f)/WP_WINDOW_WIDTH,
        (WP_WINDOW_HEIGHT + WP_WINDOW_STROKE_THICKNESS*2.0f)/WP_WINDOW_HEIGHT
    );
    set_color(BLACK);

    draw(wp_window_VBO,9,GL_TRIANGLE_FAN);
    disable_stencil_test();
}
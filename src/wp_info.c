#include <stdio.h>
#include "wp_info.h"

void gen_wp_info() {
    float window_vertices[27];

    //Generate box

    gen_octagon(
        DEST_INFO_WINDOW_WIDTH,
        DEST_INFO_WINDOW_HEIGHT,
        DEST_INFO_WINDOW_CATET,
        (vec2){0.0f,0.0f},
        window_vertices
    );

    add_to_buffer(&wp_info_window_VBO,window_vertices,sizeof(window_vertices));
    //printf("VBO: %d\n",wp_window_VBO);

    float header_vertices[18];

    gen_rectangle(
        DEST_INFO_WINDOW_WIDTH,
        DEST_INFO_HEADER_HEIGHT,
        (vec2){0.0f,0.0f},
        0.0f,
        0.0f,
        header_vertices
    );

    add_to_buffer(&wp_info_header_VBO, header_vertices, sizeof(header_vertices));

    //Waypoint outer icon

    float wp_io_vertices[12] = {
        -DEST_INFO_ICON_OUTER_SIZE/2, 0.0f, 0.0f,
        0.0f, DEST_INFO_ICON_OUTER_SIZE/2, 0.0f,
        DEST_INFO_ICON_OUTER_SIZE/2, 0.0f, 0.0f,
        0.0f, -DEST_INFO_ICON_OUTER_SIZE/2, 0.0f
    };

    add_to_buffer(&wp_info_icon_outer_VBO, wp_io_vertices, sizeof(wp_io_vertices));

    //Waypoint inner icon

    float wp_ii_vertices[12] = {
        -DEST_INFO_ICON_INNER_SIZE/2, 0.0f, 0.0f,
        0.0f, DEST_INFO_ICON_INNER_SIZE/2, 0.0f,
        DEST_INFO_ICON_INNER_SIZE/2, 0.0f, 0.0f,
        0.0f, -DEST_INFO_ICON_INNER_SIZE/2, 0.0f
    };

    add_to_buffer(&wp_info_icon_inner_VBO, wp_ii_vertices, sizeof(wp_ii_vertices));

    //Waypoint link

    float link_vertices[18];

    gen_rectangle(
        DEST_INFO_ICON_LINK_THICKNESS,
        1.0f,
        (vec2){0.0f,0.0f},
        0.0f,
        0.0f,
        link_vertices
    );

    add_to_buffer(&wp_info_link_VBO, link_vertices, sizeof(link_vertices));
}


void draw_wp_info(float vPosX, float vPosY) {

    reset_modelmatrix();
    set_position(
        vPosX,
        vPosY
    );

    set_color((vec4){0.0f,0.0f,0.0f,DEST_INFO_WINDOW_ALPHA});

    enable_stencil_test();
    enable_stencil_buffer_changes();
    
    draw(wp_info_window_VBO,9,GL_TRIANGLE_FAN);
    disable_stencil_buffer_changes();

    pass_stencil_if_1();

    set_position(
        0.0f, 
        (DEST_INFO_WINDOW_HEIGHT - DEST_INFO_HEADER_HEIGHT)/2.0f
    );
    set_color(BLACK);
    draw(wp_info_header_VBO,9,GL_TRIANGLES);

    //set_position(-DEST_INFO_WINDOW_WIDTH/2.0f + DEST_INFO_TEXT_LEFT_MARGIN, 0.0f);
    set_color(WHITE);
    render_text("Flight Plan Information\0",DEST_INFO_HEADER_FONT_SIZE,0);

    //Drawing waypoint icons

    float wp_link_size ;

    switch (waypoints.size) {
        case 0:
        case 1:
            wp_link_size = 0.0f;
            break;
        case 2:
            wp_link_size = DEST_INFO_TEXT_SPACE_1;
            break;
        case 3:
            wp_link_size = DEST_INFO_TEXT_SPACE_1 + DEST_INFO_TEXT_SPACE_2;
            break;
        case 4:
            wp_link_size = DEST_INFO_TEXT_SPACE_1 + DEST_INFO_TEXT_SPACE_2*2;
            break;
        default:
            wp_link_size = DEST_INFO_TEXT_SPACE_1 + DEST_INFO_TEXT_SPACE_2*3;
            break;
    }

    float text_pos_x = vPosX
            - DEST_INFO_WINDOW_WIDTH/2.0f + DEST_INFO_INNER_MARGIN
            + DEST_INFO_ICON_OUTER_SIZE;
    
    float text_pos_y = vPosY
            + DEST_INFO_WINDOW_HEIGHT/2.0f - DEST_INFO_HEADER_HEIGHT
            - DEST_INFO_INNER_MARGIN - DEST_INFO_ICON_OUTER_SIZE;


    reset_modelmatrix();
    set_position(text_pos_x, text_pos_y - wp_link_size/2.0f);
    resize(1.0f,wp_link_size);
    set_color(PURPLE);
    draw(wp_info_link_VBO,6,GL_TRIANGLES);

    reset_modelmatrix();
    set_position(text_pos_x, text_pos_y);

    for (int i = 0; i < waypoints.size; i++) {
        set_color(BLACK);
        draw(wp_info_icon_outer_VBO,4,GL_TRIANGLE_FAN);
        resize(
            (DEST_INFO_ICON_OUTER_SIZE - DEST_INFO_ICON_STROKE*2.0f)/DEST_INFO_ICON_OUTER_SIZE,
            (DEST_INFO_ICON_OUTER_SIZE - DEST_INFO_ICON_STROKE*2.0f)/DEST_INFO_ICON_OUTER_SIZE
        );
        set_color(PURPLE);
        draw(wp_info_icon_outer_VBO,4,GL_TRIANGLE_FAN);
        resize(
            DEST_INFO_ICON_OUTER_SIZE/(DEST_INFO_ICON_OUTER_SIZE - DEST_INFO_ICON_STROKE*2.0f),
            DEST_INFO_ICON_OUTER_SIZE/(DEST_INFO_ICON_OUTER_SIZE - DEST_INFO_ICON_STROKE*2.0f)
        );
        set_color(BLACK);
        draw(wp_info_icon_inner_VBO,4,GL_TRIANGLE_FAN);
        resize(
            (DEST_INFO_ICON_INNER_SIZE - DEST_INFO_ICON_STROKE*2.0f)/DEST_INFO_ICON_INNER_SIZE,
            (DEST_INFO_ICON_INNER_SIZE - DEST_INFO_ICON_STROKE*2.0f)/DEST_INFO_ICON_INNER_SIZE
        );
        set_color(PURPLE);
        draw(wp_info_icon_inner_VBO,4,GL_TRIANGLE_FAN);
        resize(
            DEST_INFO_ICON_INNER_SIZE/(DEST_INFO_ICON_INNER_SIZE - DEST_INFO_ICON_STROKE*2.0f),
            DEST_INFO_ICON_INNER_SIZE/(DEST_INFO_ICON_INNER_SIZE - DEST_INFO_ICON_STROKE*2.0f)
        );
        set_position(0.0f, i == 0 ? -DEST_INFO_TEXT_SPACE_1 : -DEST_INFO_TEXT_SPACE_2);
    }

    reset_modelmatrix();
    set_position(
        text_pos_x + DEST_INFO_ICON_OUTER_SIZE/2.0f + 6.0f,
        text_pos_y
    );

    set_color(WHITE);

    struct Waypoint *curr_waypoint = waypoints.first;

    for (int i = 0; i < waypoints.size; i++) {
        render_text(
            curr_waypoint->id,
            i == 0 ? DEST_INFO_FONT_SIZE_CURRWP : DEST_INFO_FONT_SIZE_NEXTWPS,
            1
        );
        set_position(0.0f, i == 0 ? -DEST_INFO_TEXT_SPACE_1 : -DEST_INFO_TEXT_SPACE_2);
        curr_waypoint = curr_waypoint->next;
    }

    //Waypoint to next one information

    if (waypoints.size >= 1) {

        float dist_left_x = pos[0] - waypoints.first->next->posX;
        float dist_left_z = pos[2] - waypoints.first->next->posZ;
        float dist_left_units = sqrt(dist_left_x*dist_left_x + dist_left_z*dist_left_z);

        float dist_left_km = dist_left_units*KIL_PER_UNIT;

        reset_modelmatrix();
        set_position(
            text_pos_x + DEST_INFO_ICON_OUTER_SIZE/2.0f + 6.0f,
            text_pos_y - DEST_INFO_CURRWPDET_MRGTOP
        );
        render_text("Current time:",DEST_INFO_FONT_SIZE_CURRWPDET,1);
        set_position(0.0f, -64.0f*0.6f*DEST_INFO_FONT_SIZE_CURRWPDET);
        char km_text[256];
        sprintf(km_text,"%0.1fkm",dist_left_km);
        render_text(km_text,DEST_INFO_FONT_SIZE_CURRWPDET,1);

        //Time information

        text_pos_x = vPosX
                    + DEST_INFO_WINDOW_WIDTH/2.0f - DEST_INFO_INNER_MARGIN;
        text_pos_y = text_pos_y - DEST_INFO_CURRWPDET_MRGTOP;

        reset_modelmatrix();
        set_position(text_pos_x, text_pos_y);

        curr_waypoint = waypoints.first;

        for (int i = 0; i < min(waypoints.size,4); i++) {

            time_t arrival_time;

            if (i == 0)
                time(&arrival_time);
            else
                arrival_time = curr_waypoint->arrival_time;

            int hour = (arrival_time/3600) % 24;
            int minute = (arrival_time/60) % 60;

            char time_text[6];
            sprintf(time_text,"%0*d:%0*d", 2, hour, 2, minute);
            render_text(
                time_text, 
                i == 0 ? DEST_INFO_FONT_SIZE_CURRWP : DEST_INFO_FONT_SIZE_NEXTWPS,
                2);
            set_position(0.0f, i == 0 ? -(-DEST_INFO_CURRWPDET_MRGTOP + DEST_INFO_TEXT_SPACE_1) : -DEST_INFO_TEXT_SPACE_2);

            curr_waypoint = curr_waypoint->next;
        }

    }

    //Drawing border

    pass_stencil_if_0();
    reset_modelmatrix();
    set_position(vPosX, vPosY);
    resize(
        (DEST_INFO_WINDOW_WIDTH+DEST_INFO_WINDOW_STROKE*2.0f)/DEST_INFO_WINDOW_WIDTH,
        (DEST_INFO_WINDOW_HEIGHT+DEST_INFO_WINDOW_STROKE*2.0f)/DEST_INFO_WINDOW_HEIGHT
    );
    set_color(BLACK);
    draw(wp_info_window_VBO,9,GL_TRIANGLE_FAN);

}
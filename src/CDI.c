#include <stdio.h>
#include "CDI.h"

void gen_CDI() {

    float plane_vertices[] = {
        9.0f, 0.0f, 0.0f,
        9.0f, 4.0f, 0.0f,
        21.0f, 4.0f, 0.0f,

        21.0f, 4.0f, 0.0f,
        21.0f, 0.0f, 0.0f,
        9.0f, 0.0f, 0.0f,

        9.0f, 4.0f, 0.0f,
        15.0f, 7.0f, 0.0f,
        21.0f, 4.0f, 0.0f,

        12.0f, 14.0f, 0.0f,
        18.0f, 14.0f, 0.0f,
        15.0f, 0.0f, 0.0f,

        12.0f, 14.0f, 0.0f,
        2.0f, 12.0f, 0.0f,
        0.0f, 16.0f, 0.0f,

        12.0f, 14.0f, 0.0f,
        0.0f, 16.0f, 0.0f,
        14.0f, 25.0f, 0.0f,

        0.0f, 12.0f, 0.0f,
        0.0f, 16.0f, 0.0f,
        2.0f, 12.0f, 0.0f,

        18.0f, 14.0f, 0.0f,
        28.0f, 12.0f, 0.0f,
        30.0f, 16.0f, 0.0f,

        18.0f, 14.0f, 0.0f,
        30.0f, 16.0f, 0.0f,
        16.0f, 25.0f, 0.0f,

        30.0f, 12.0f, 0.0f,
        30.0f, 16.0f, 0.0f,
        28.0f, 12.0f, 0.0f,

        12.0f, 14.0f, 0.0f,
        12.0f, 32.0f, 0.0f,
        18.0f, 14.0f, 0.0f,

        12.0f, 32.0f, 0.0f,
        18.0f, 32.0f, 0.0f,
        18.0f, 14.0f, 0.0f,

        12.0f, 32.0f, 0.0f,
        13.0f, 36.0f, 0.0f,
        17.0f, 36.0f, 0.0f,

        12.0f, 32.0f, 0.0f,
        17.0f, 36.0f, 0.0f,
        18.0f, 32.0f, 0.0f,
    };

    float max_x = 0.0f;
    float max_y = 0.0f;

    for (int i = 0; i < sizeof(plane_vertices)/sizeof(float); i += 3) {
        max_x = fmax(max_x,plane_vertices[i]);
        max_y = fmax(max_y,plane_vertices[i + 1]);
    }

    for (int i = 0; i < sizeof(plane_vertices)/sizeof(float); i += 3) {
        plane_vertices[i] -= max_x/2.0f;
        plane_vertices[i + 1] -= max_y/2.0f;
    }
    
    add_to_buffer(&CDI_plane_VBO, plane_vertices, sizeof(plane_vertices));

    float needle_head_vertices[] = {
        -CDI_NEEDLE_HEAD_WIDTH/2.0f, -CDI_NEEDLE_HEAD_HEIGHT/2.0f, 0.0f,
        CDI_NEEDLE_HEAD_WIDTH/2.0f, -CDI_NEEDLE_HEAD_HEIGHT/2.0f, 0.0f,
        0.0f, CDI_NEEDLE_HEAD_HEIGHT/2.0f, 0.0f
    };

    add_to_buffer(&CDI_needle_head_VBO, needle_head_vertices, sizeof(needle_head_vertices));

    float needle_tail_vertices[18];

    gen_rectangle(
        CDI_NEEDLE_TAIL_WIDTH,
        CDI_NEEDLE_TAIL_HEIGHT, 
        (vec2){0.0f,0.0f}, 
        0.0f, 
        0.0f, 
        needle_tail_vertices
    );

    add_to_buffer(&CDI_needle_tail_VBO, needle_tail_vertices, sizeof(needle_tail_vertices));

    float scale_mark_vertices[] = {
        -CDI_SCALE_MARK_SIZE/2.0f, 0.0f, 0.0f,
        0.0f, CDI_SCALE_MARK_SIZE/2.0f, 0.0f,
        CDI_SCALE_MARK_SIZE/2.0f, 0.0f, 0.0f,

        -CDI_SCALE_MARK_SIZE/2.0f, 0.0f, 0.0f,
        CDI_SCALE_MARK_SIZE/2.0f, 0.0f, 0.0f,
        0.0f, -CDI_SCALE_MARK_SIZE/2.0f, 0.0f
    };

    add_to_buffer(&CDI_scale_mark_VBO, scale_mark_vertices, sizeof(scale_mark_vertices));

}

void draw_CDI(float vPosX, float vPosY) {
    reset_modelmatrix();
    set_position(vPosX, vPosY);
    set_default_projection();

    //float rot = fix_angle(rout rotY

    float rot = 0;

    float cdi_angle = 0;

    if (waypoints.size >= 2) {
        float x = waypoints.first->next->posX - pos[0];
        float y = waypoints.first->next->posZ - pos[2];
        float route_angle = fix_angle(atan2(y,x));

        rot = -GLM_PI/2.0f - route_angle; 

        if (waypoints.size > 1) {
            float a = waypoints.first->next->posX - waypoints.first->posX;
            float b = waypoints.first->next->posZ - waypoints.first->posZ;
            
            float cross_prod = x*b - y*a;
            cdi_angle = acos((x*a + y*b)/(sqrt(x*x + y*y) * sqrt(a*a + b*b)));
            //if is NaN
            if (cdi_angle != cdi_angle) cdi_angle = 0.0f;
            else {
                cdi_angle = fmin(cdi_angle, glm_rad(10.0f));
                cdi_angle = cross_prod < 0.0f ? cdi_angle : -cdi_angle;
            }
        }
    }

    rotate(rot + rotY);
    set_position(0.0f, COMPASS_RADIUS - CDI_NEEDLE_HEAD_HEIGHT/2.0f);
    set_color(PURPLE);
    draw(CDI_needle_head_VBO,3,GL_TRIANGLES);

    //Scale marks

    set_position(0.0f, - COMPASS_RADIUS + CDI_NEEDLE_HEAD_HEIGHT/2.0f);
    set_color(WHITE);
    set_position(-CDI_SCALE_RADIUS, 0.0f);

    for (int i = 0; i < 11; i++) {
        if (i <= 3 || i >= 7) {
            draw(CDI_scale_mark_VBO,6,GL_TRIANGLES);
            resize(
                (CDI_SCALE_MARK_SIZE - 2.0f)/CDI_SCALE_MARK_SIZE,
                (CDI_SCALE_MARK_SIZE - 2.0f)/CDI_SCALE_MARK_SIZE
            );
            set_color(BLACK);
            draw(CDI_scale_mark_VBO,6,GL_TRIANGLES);
            resize(
                CDI_SCALE_MARK_SIZE/(CDI_SCALE_MARK_SIZE - 2.0f),
                CDI_SCALE_MARK_SIZE/(CDI_SCALE_MARK_SIZE - 2.0f)
            );
            set_color(WHITE);
        }
        set_position((2.0f*CDI_SCALE_RADIUS)/10.0f, 0.0f);
    }

    //Needle

    float max_angle = atan2(CDI_SCALE_RADIUS,COMPASS_RADIUS);
    float needle_angle = max_angle * cdi_angle/glm_rad(10);

    reset_modelmatrix();
    set_position(vPosX, vPosY);
    rotate(rot + rotY);
    set_position(0.0f, COMPASS_RADIUS - CDI_NEEDLE_HEAD_HEIGHT/2.0f);
    rotate(needle_angle);
    set_position(0.0f, - CDI_NEEDLE_TAIL_HEIGHT/2.0f + CDI_NEEDLE_HEAD_HEIGHT/2.0f);
    set_color(PURPLE);
    draw(CDI_needle_tail_VBO,6,GL_TRIANGLES);
    //printf("Needle angle: %0.6f\n",glm_deg(cdi_angle));

    reset_modelmatrix();
    set_position(vPosX, vPosY);
    set_color(WHITE);
    draw(CDI_plane_VBO,42,GL_TRIANGLES);

}
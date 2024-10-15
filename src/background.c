#include "background.h"

int (*active_chunks)[2];

void init_map() {

    //Generating array
    
    chunk_VBO_list = malloc(NUM_CHUNKS * sizeof(*chunk_VBO_list));
    //printf("Num. chunks generated: %d\n", NUM_CHUNKS);
    if (chunk_VBO_list == NULL) {
        printf("Memory allocation failed\n");
        exit(1); // Error handling in case malloc fails
    }
    
    active_chunks = malloc(NUM_CHUNKS * sizeof(*active_chunks));
    if (active_chunks == NULL) {
        printf("Memory allocation failed\n");
        exit(1); // Error handling in case malloc fails
    }

    for (int i = 0; i < NUM_CHUNKS; i++) {
        active_chunks[i][0] = 9999;
        active_chunks[i][1] = 9999;
    }

    chunk_pos_x = 0;
    chunk_pos_y = 0;

    update_chunks();
    
}

uint32_t murmurhash3(uint32_t key) {
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;

    key *= c1;
    key = (key << r1) | (key >> (32 - r1));
    key *= c2;

    uint32_t hash = key;
    hash ^= key >> r2;
    hash *= m;
    hash ^= key >> r2;
    hash *= n;

    return hash;
}

// Pseudorandom function to get a float between 0 and 1
float randomize(int x, int y) {
    uint32_t combined = ((uint32_t)x << 16) | ((uint32_t)y & 0xFFFF);
    uint32_t hashed = murmurhash3(combined);
    return (float)hashed / (float)UINT32_MAX;
}

float interpolation(float a0, float a1,float x) {
    if (x <= 0) return a0;
    if (x >= 1) return a1;
    return a0 + (3*x*x - 2*x*x*x)*(a1 - a0);
}

float dot_product_corner(int x, int y, int corner_x, int corner_y) {
    //printf("X: %d Corner X: %d\n",x,corner_x);
    float corner_angle = -GLM_PI + GLM_PI*randomize(corner_x,corner_y);
    //printf("Random: %0.7f\n",randomize(corner_x,corner_y));
    float x_vec = (float)(x - corner_x + 0.0001f)/vertices_per_square;
    float y_vec = (float)(y - corner_y + 0.0001f)/vertices_per_square;
    float point_angle = atan2(x_vec,y_vec);
    //if (x == 0) printf("x: %d, corner-x: %d, y: %d, corner-y: %d, Point_angle: %0.6f\n",x,corner_x,y,corner_y,point_angle);
    float angle_diff = point_angle - corner_angle;
    angle_diff = angle_diff > GLM_PI ? angle_diff - 2*GLM_PI : angle_diff;
    angle_diff = angle_diff < -GLM_PI ? angle_diff + 2*GLM_PI : angle_diff;
    //printf("X vec: %0.6f Y vec: %0.6f\n",x_vec,y_vec);
    float dot = sqrt(x_vec*x_vec + y_vec*y_vec)*cos(angle_diff);
    //printf("Dot: %0.6f\n",dot);
    return dot;
}

float sample(int x, int y) {
    int grid_x1 = x - (int)mod(x,vertices_per_square);
    int grid_x2 = grid_x1 + vertices_per_square;
    int grid_y1 = y - (int)mod(y,vertices_per_square);
    int grid_y2 = grid_y1 + vertices_per_square;

    float dot_1 = dot_product_corner(x,y,grid_x1,grid_y1);
    float dot_2 = dot_product_corner(x,y,grid_x2,grid_y1);
    float dot_3 = dot_product_corner(x,y,grid_x1,grid_y2);
    float dot_4 = dot_product_corner(x,y,grid_x2,grid_y2);

    //if (x == 0) printf("Dot product: %0.6f\n",dot_1);

    float x_pos = mod(x,vertices_per_square)/vertices_per_square;
    float y_pos = mod(y,vertices_per_square)/vertices_per_square;

    //printf("X pos: %0.6f\n",x_pos);

    float inter_1 = interpolation(dot_1,dot_2,x_pos);
    float inter_2 = interpolation(dot_3,dot_4,x_pos);

    return 0.5f*interpolation(inter_1,inter_2,y_pos) + 0.5f;
}


void load_chunk(int chunk_x, int chunk_y, int VBO_index) {

    float vertices[squares_per_chunk*squares_per_chunk*36];

    int k = 0;

    //printf("Reached 1\n");

    float highest_sample = -10;
    float lowest_sample = 10;

    for (int i = 0; i < squares_per_chunk; i++) {
        //printf("ROW:\n");
        for (int j = 0; j < squares_per_chunk; j++) {
            int vertexX = chunk_x*squares_per_chunk + i;
            int vertexY = chunk_y*squares_per_chunk + j;

            float vertice_height_1 = mountain_max_height*sample(vertexX,vertexY);
            float vertice_height_2 = mountain_max_height*sample(vertexX+1,vertexY);
            float vertice_height_3 = mountain_max_height*sample(vertexX,vertexY+1);
            float vertice_height_4 = mountain_max_height*sample(vertexX+1,vertexY+1);

            if (sample(vertexX,vertexY) > highest_sample) highest_sample = sample(vertexX,vertexY);
            if (sample(vertexX,vertexY) < lowest_sample) lowest_sample = sample(vertexX,vertexY);

            //printf("%0.9f#",vertice_height_1);

            //printf("Heights: %0.6f - %0.6f - %0.6f - %0.6f\n",
            //    vertice_height_1,vertice_height_2,vertice_height_3,vertice_height_4);

            vec2 corner_1 = {
                chunk_size*chunk_x + chunk_size*((float)i/squares_per_chunk - 0.5f),
                chunk_size*chunk_y + chunk_size*((float)j/squares_per_chunk - 0.5f)
            };
            vec2 corner_2 = {
                chunk_size*chunk_x + chunk_size*((float)(i + 1)/squares_per_chunk - 0.5f),
                chunk_size*chunk_y + chunk_size*((float)j/squares_per_chunk - 0.5f)
            };
            vec2 corner_3 = {
                chunk_size*chunk_x + chunk_size*((float)i/squares_per_chunk - 0.5f),
                chunk_size*chunk_y + chunk_size*((float)(j + 1)/squares_per_chunk - 0.5f)
            };
            vec2 corner_4 = {
                chunk_size*chunk_x + chunk_size*((float)(i + 1)/squares_per_chunk - 0.5f),
                chunk_size*chunk_y + chunk_size*((float)(j + 1)/squares_per_chunk - 0.5f)
            };

            float triangle1[9];
            float triangle2[9];

            if ((vertice_height_1 > vertice_height_2 && vertice_height_1 > vertice_height_3)||
                (vertice_height_4 > vertice_height_2 && vertice_height_4 > vertice_height_3)) {
                float temp1[] = {
                    corner_1[0], vertice_height_1, corner_1[1],
                    corner_3[0], vertice_height_3, corner_3[1],
                    corner_4[0], vertice_height_4, corner_4[1]
                };

                float temp2[] = {
                    corner_1[0], vertice_height_1, corner_1[1],
                    corner_4[0], vertice_height_4, corner_4[1],
                    corner_2[0], vertice_height_2, corner_2[1]
                };
                
                memcpy(triangle1, temp1, sizeof(temp1));
                memcpy(triangle2, temp2, sizeof(temp2));
            } else {
                float temp1[] = {
                    corner_1[0], vertice_height_1, corner_1[1],
                    corner_3[0], vertice_height_3, corner_3[1],
                    corner_2[0], vertice_height_2, corner_2[1]
                };

                float temp2[] = {
                    corner_3[0], vertice_height_3, corner_3[1],
                    corner_4[0], vertice_height_4, corner_4[1],
                    corner_2[0], vertice_height_2, corner_2[1]
                };

                memcpy(triangle1, temp1, sizeof(temp1));
                memcpy(triangle2, temp2, sizeof(temp2));
            }

            float normal1[3];
            float normal2[3];

            gen_normal(triangle1,normal1);
            gen_normal(triangle2,normal2);
                
            float square[] = {
                triangle1[0],triangle1[1], triangle1[2],    normal1[0], normal1[1], normal1[2],
                triangle1[3],triangle1[4], triangle1[5],    normal1[0], normal1[1], normal1[2],
                triangle1[6],triangle1[7], triangle1[8],    normal1[0], normal1[1], normal1[2],

                triangle2[0],triangle2[1], triangle2[2],    normal2[0], normal2[1], normal2[2],
                triangle2[3],triangle2[4], triangle2[5],    normal2[0], normal2[1], normal2[2],
                triangle2[6],triangle2[7], triangle2[8],    normal2[0], normal2[1], normal2[2],
            };
            
            for (int i = 0; i < 36; i +=6) {
                for (int j = 0; j < 3; j++) {
                    //printf("Coord %c: %0.6f\n", 'x' + j, square[i + j]);
                }
            }

            for (int i = 3; i < 36; i +=3) {
                //printf("Normal val: %0.6f\n",square[i]);
            }
            
            for (int i = 0; i < 36; i++)
                vertices[k + i] = square[i];

            k += 36;
        }
    }

    glGenBuffers(1, &chunk_VBO_list[VBO_index]);
    glBindBuffer(GL_ARRAY_BUFFER, chunk_VBO_list[VBO_index]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

}

int is_chunk_in_range(int chunk_x, int chunk_y) {
    return abs(chunk_pos_x - chunk_x) <= terrain_diameter/2 &&  abs(chunk_pos_y - chunk_y) <= terrain_diameter/2;
}

int is_chunk_loaded(int chunk_x, int chunk_y) {
    for (int i = 0; i < NUM_CHUNKS; i++)
        if (active_chunks[i][0] == chunk_x && active_chunks[i][1] == chunk_y) return 1;
    return 0;
}

void update_chunks() {
    int VBOs_to_update[NUM_CHUNKS];

    for (int i = 0; i < NUM_CHUNKS; i++) {
        VBOs_to_update[i] = !is_chunk_in_range(active_chunks[i][0],active_chunks[i][1]);
    }

    int k = 0;

    int x_bound_1 = chunk_pos_x - terrain_diameter/2;
    int x_bound_2 = chunk_pos_x + terrain_diameter/2;

    for (int i = x_bound_1; i <= x_bound_2; i++) {
        
        int y_bound_1 = chunk_pos_y - terrain_diameter/2;
        int y_bound_2 = chunk_pos_y + terrain_diameter/2;

        for (int j = y_bound_1; j <= y_bound_2; j++) {
            //printf("Chunk %d %d\n", i, j);
            if (!is_chunk_loaded(i,j)) {
                while (VBOs_to_update[k] == 0) {k++;}
                load_chunk(i,j,k);
                active_chunks[k][0] = i;
                active_chunks[k][1] = j;
                k++;
            }
        }
    }
}


void draw_chunk(int VBO_index) {
    glm_mat4_identity(modelMatrix);
    
    if (map_test_mode == 0) {
        glm_scale(modelMatrix,(vec3){2.0f/KIL_PER_UNIT,2.0f/KIL_PER_UNIT,2.0f/KIL_PER_UNIT});

        glm_rotate(modelMatrix,rotZ,(vec3){0.0f,0.0f,1.0f});
        glm_rotate(modelMatrix,rotX,(vec3){1.0f,0.0f,0.0f});
        glm_rotate(modelMatrix,rotY,(vec3){0.0f,1.0f,0.0f});
        
        glm_translate(modelMatrix,(vec3){-pos[0],-pos[1],-pos[2]});
    } else {

        glm_scale(modelMatrix,(vec3){1.0f/chunk_size,2.0f/chunk_size,2.0f/chunk_size});
        glm_rotate(modelMatrix,rotZ,(vec3){0.0f,0.0f,1.0f});
        glm_rotate(modelMatrix,rotX,(vec3){1.0f,0.0f,0.0f});
        glm_rotate(modelMatrix,rotY,(vec3){0.0f,1.0f,0.0f});
        glm_translate(modelMatrix,(vec3){0.0f,-6.0f,0.0f});
    }

    glm_mat4_identity(viewMatrix);
    glm_mat4_identity(projectionMatrix);
    glm_mat4_inv(modelMatrix, normalMatrix);
    glm_mat4_transpose(normalMatrix);

    //printf("Transposed: %0.6f\n", *normalMatrix[0]);

    glm_perspective(glm_rad(35),WINDOW_WIDTH/WINDOW_HEIGHT,0.1f,100.0f*KIL_PER_UNIT,projectionMatrix);

    glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, modelMatrix[0]);
    glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix[0]);
    glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix[0]);
    glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix[0]);

    glUniform4fv(colorLoc,1, MAP_COLOR);
    glUniform1iv(enabledFlagsLoc, 2, enabledFlags);
    
    glBindBuffer(GL_ARRAY_BUFFER, chunk_VBO_list[VBO_index]);

    glEnableVertexAttribArray(POS_ATTR);
    glVertexAttribPointer(POS_ATTR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(NORMAL_ATTR);
    glVertexAttribPointer(NORMAL_ATTR, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));

    glDrawArrays(GL_TRIANGLES, 0, 6*squares_per_chunk*squares_per_chunk);
    //printf("Drawing chunk %d!\n",chunk_VBO_list[VBO_index]);
}


void draw_map() {

    enabledFlags[0] = 1;
    glUniform1iv(enabledFlagsLoc, 2, enabledFlags);

    int curr_chunk_pos_x = round(pos[0]/chunk_size);
    int curr_chunk_pos_y = round(pos[2]/chunk_size);

    if (curr_chunk_pos_x != chunk_pos_x || curr_chunk_pos_y != chunk_pos_y) {
        chunk_pos_x = curr_chunk_pos_x;
        chunk_pos_y = curr_chunk_pos_y;
        update_chunks();
    }
    
    /*printf("Params: %d %d %0.6f %d %0.6f\n",
    vertices_per_square, squares_per_chunk, chunk_size,
    terrain_diameter, mountain_max_height);*/

    //printf("Tests: %0.6f %0.6f %0.6f %0.6f\n",atan2(0.0f,1.0f),atan2(1.0f,0.0f),atan2(-1.0f,0.0f),atan2(-0.0001f,-1.0f));
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_TRUE); // Write to depth buffer
    glEnable(GL_DEPTH_TEST);

    for (int i = 0; i < NUM_CHUNKS; i++) {
        draw_chunk(i);
        //printf("Chunk at %d: %d\n",i,chunk_VBO_list[i]);
    }

    //draw_chunk(1);

    glDisable(GL_DEPTH_TEST);

    glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
    glDisable(GL_STENCIL_TEST);

    enabledFlags[0] = 0;
    glUniform1iv(enabledFlagsLoc, 2, enabledFlags);
}
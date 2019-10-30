#include "space_runner.h"

void sr_init(Runner* runner) {
    //Draws ground
    for (int y = MAX_Y - GROUND_LEVEL; y < MAX_Y; y++) {
        for (int x = 0; x < MAX_X; x++) {
            MAP[y][x] = 1;
        }
    }
    //Init and draw runner
    runner->velx = 4;
    runner->vely = 0;
    runner->posy = 0;
}

void sr_draw_map() {
    //Needs to update obstacles and stuff.
    
    sr_map_to_mem();
}

void sr_map_to_mem() {}


void sr_gen_obst(Runner* runner) {

}


void sr_draw_runner(Runner* runner) {

}


void sr_jump(Runner* runner) {
    if (runner->posy != GROUND_LEVEL) {
        return; //no double jumps
    }
    runner->vely = 3;
}


void sr_crash(Runner* runner) {}

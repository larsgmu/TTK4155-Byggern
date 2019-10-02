#include "adc_driver.h"

struct slider {
    int right_pos;
    int left_pos;
};

void get_slider_pos(struct slider* slider);
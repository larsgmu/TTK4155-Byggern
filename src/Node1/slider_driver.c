#include "slider_driver.h"

void get_slider_pos(Slider* slider) {
    slider->right_pos = adc_read(Right_slider);
    slider->left_pos = adc_read(Left_slider);
}

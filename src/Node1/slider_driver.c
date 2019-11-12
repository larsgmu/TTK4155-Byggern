/*!@file
* Slider interface
*/
#include "slider_driver.h"
#include "can_driver.h"

void slider_run(Slider* slider) {
    slider->right_pos = adc_read(Right_slider);
    slider->left_pos = adc_read(Left_slider);
}

void send_slider_pos(Slider* slider) {
    CANmsg slider_msg;

    slider_msg.id = 2;
    slider_msg.length = 2;
    slider_msg.data[0] = slider->left_pos; // LEFT slider
    slider_msg.data[1] = slider->right_pos; // RIGHT slider
    printf("Slider: %d                   \r", slider_msg.data[0]);
    can_send_msg(&slider_msg);
}

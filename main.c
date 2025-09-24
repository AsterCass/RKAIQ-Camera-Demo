#include <stdbool.h>
#include <stdio.h>
#include "camir_control.h"
#include "camrgb_control.h"


#define CAMERA_WIDTH 1280
#define CAMERA_HEIGHT 720
#define SAVE_FRAMES 30


int main() {
    printf("Hello World\n");

    set_rgb_param(CAMERA_WIDTH, CAMERA_HEIGHT, NULL, true);
    set_ir_param(CAMERA_WIDTH, CAMERA_HEIGHT, NULL);
    set_rgb_rotation(90);

    printf("Init finish\n");

    return 0;
}

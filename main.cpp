#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <rga/RgaApi.h>
#include "aiq_control.h"

#include "camir_control.h"
#include "camrgb_control.h"
#include "display.h"


#define CAMERA_WIDTH 1280
#define CAMERA_HEIGHT 720
#define SAVE_FRAMES 30


static bool quit = false;

static void sigterm_handler(int sig) {
    fprintf(stderr, "signal %d\n", sig);
    quit = true;
}


int main() {
    printf("Hello World\n");

    set_rgb_param(CAMERA_WIDTH, CAMERA_HEIGHT, NULL, true);
    set_ir_param(CAMERA_WIDTH, CAMERA_HEIGHT, NULL);
    set_rgb_rotation(90);

    display_switch(DISPLAY_VIDEO_RGB);
    if (display_init(500, 500)) {
        printf("Init failed\n");
        return -1;
    }

    printf("Init display finish\n");

    if (c_RkRgaInit()) {
        printf("%s: rga init fail!\n", __func__);
    }

    aiq_control_alloc();
    for (int i = 0; i < 10; i++) {
        if (aiq_control_get_status(AIQ_CONTROL_RGB)) {
            printf("%s: RGB aiq status ok.\n", __func__);
            camrgb_control_init();
            break;
        }
        sleep(1);
    }

    for (int i = 0; i < 10; i++) {
        if (aiq_control_get_status(AIQ_CONTROL_IR)) {
            printf("%s: IR aiq status ok.\n", __func__);
            // camir_control_init();
            break;
        }
        sleep(1);
    }

    printf("Init aiq finish\n");

    signal(SIGINT, sigterm_handler);
    while (!quit) {
        usleep(500000);
    }

    return 0;
}

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

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

    printf("Init finish\n");


    signal(SIGINT, sigterm_handler);
    while (!quit) {
        usleep(500000);
    }

    return 0;
}

#include <pthread.h>
#include "camir_control.h"
#include <stdbool.h>

static const struct rkisp_api_ctx *ctx;
static const struct rkisp_api_buf *buf;
static bool g_run;
static pthread_t g_tid;

static bool g_ir_en;
static int g_ir_width;
static int g_ir_height;
static display_callback g_display_cb = NULL;
static pthread_mutex_t g_display_lock = PTHREAD_MUTEX_INITIALIZER;
static int g_rotation = HAL_TRANSFORM_ROT_270;

void set_ir_rotation(int angle)
{
    if (angle == 90)
        g_rotation = HAL_TRANSFORM_ROT_90;
    else if (angle == 270)
        g_rotation = HAL_TRANSFORM_ROT_270;
}

void set_ir_display(display_callback cb)
{
    pthread_mutex_lock(&g_display_lock);
    g_display_cb = cb;
    pthread_mutex_unlock(&g_display_lock);
}

void set_ir_param(int width, int height, display_callback cb)
{
    g_ir_en = true;
    g_ir_width = width;
    g_ir_height = height;
    set_ir_display(cb);
}
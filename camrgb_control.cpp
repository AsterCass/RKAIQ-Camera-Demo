/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 * author: Zhihua Wang, hogan.wang@rock-chips.com
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <pthread.h>
#include "camrgb_control.h"
#include <rkaiq/rkisp_api.h>
#include <stdio.h>
#include <rga/rga.h>
#include <opencv2/opencv.hpp>


static bool g_def_expo_weights = false;
bool g_expo_weights_en = false;
static unsigned char weights[81];

static const struct rkisp_api_ctx *ctx;
static const struct rkisp_api_buf *buf;
static bool g_run;
static pthread_t g_tid;

bool g_rgb_en;
int g_rgb_width;
int g_rgb_height;
static display_callback g_display_cb = NULL;
static pthread_mutex_t g_display_lock = PTHREAD_MUTEX_INITIALIZER;
static int g_rotation = HAL_TRANSFORM_ROT_90;

void set_rgb_rotation(int angle) {
    if (angle == 90)
        g_rotation = HAL_TRANSFORM_ROT_90;
    else if (angle == 270)
        g_rotation = HAL_TRANSFORM_ROT_270;
}

void set_rgb_display(display_callback cb) {
    pthread_mutex_lock(&g_display_lock);
    g_display_cb = cb;
    pthread_mutex_unlock(&g_display_lock);
}

void set_rgb_param(int width, int height, display_callback cb, bool expo) {
    g_rgb_en = true;
    g_rgb_width = width;
    g_rgb_height = height;
    set_rgb_display(cb);
    g_expo_weights_en = expo;
}

void getRgbFrame(void *buf, int width, int height, int cnt) {
    time_t now;
    time(&now);
    static time_t lastTime = 0;
    if (now - lastTime > 5) {
        lastTime = now;

        uint8_t *yuv_data = (uint8_t *) buf;
        cv::Mat yuv(height * 3 / 2, width, CV_8UC1, yuv_data);
        cv::Mat bgr;
        cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGR_NV12);
        cv::rotate(bgr, bgr, cv::ROTATE_90_CLOCKWISE);

        cv::imwrite("/data/frd/rgb-" + std::to_string(cnt) + ".jpg", bgr);
    }
}


static void *process(void *arg) {
    static int cnt = 0;
    do {
        ++cnt;
        buf = rkisp_get_frame(ctx, 0);

        // if (!rockface_control_convert_detect(buf->buf, ctx->width, ctx->height, RK_FORMAT_YCbCr_420_SP, g_rotation, id))
        //     rockface_control_convert_feature(buf->buf, ctx->width, ctx->height, RK_FORMAT_YCbCr_420_SP, g_rotation, id);
        getRgbFrame(buf->buf, ctx->width, ctx->height, cnt);

        pthread_mutex_lock(&g_display_lock);
        if (g_display_cb)
            g_display_cb(buf->buf, buf->fd, RK_FORMAT_YCbCr_420_SP,
                         ctx->width, ctx->height, g_rotation);
        pthread_mutex_unlock(&g_display_lock);

        rkisp_put_frame(ctx, buf);
    } while (g_run);

    pthread_exit(NULL);
}


int camrgb_control_init(void) {
    if (!g_rgb_en)
        return 0;


    ctx = rkisp_open_device2(CAM_TYPE_RKISP1);
    if (ctx == NULL) {
        printf("%s: ctx is NULL\n", __func__);
        return -1;
    }

    rkisp_set_buf(ctx, 3, NULL, 0);

    rkisp_set_fmt(ctx, g_rgb_width, g_rgb_height, V4L2_PIX_FMT_NV12);

    if (rkisp_start_capture(ctx))
        return -1;

    g_run = true;
    if (pthread_create(&g_tid, NULL, process, NULL)) {
        printf("pthread_create fail\n");
        return -1;
    }

    return 0;
}

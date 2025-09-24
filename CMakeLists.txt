cmake_minimum_required(VERSION 3.10)

project(theTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (WIN32)
    include_directories(
            ./include
    )
else ()
    include_directories(
            ./include
    )
    link_directories(
            /data/src/rockchip/_install//lib
    )
endif ()


set(PROJECT_SOURCES
        main.cpp
        camir_control.c
        camrgb_control.c
        display.c
        rkdrm_display.c
        aiq_control.cpp

)

add_executable(theTest
        ${PROJECT_SOURCES}
)


if (WIN32)
else ()
    target_link_libraries(theTest PRIVATE
            -lrkaiq -leasymedia -ldrm -lrockchip_mpp -lavformat -lavcodec -lswresample -lavutil -lasound
            -lRKAP_3A -lRKAP_ANR -lRKAP_Common -lv4l2 -lrga -lmd_share -lod_share -lrkaiq -lz -lv4lconvert -ljpeg
            -lVSC_Lite -lGAL -lArchModelSw -lNNArchPerf -lOpenVX -lrknn_runtime -lrknn_api
            -lpthread -lrkisp_api
    )

endif ()


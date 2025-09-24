cmake_minimum_required(VERSION 3.10)

project(theTest LANGUAGES C)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED True)

#add_definitions(-DRKAIQ)

if (WIN32)
    set(RK_ROOT ./)
else ()
    set(RK_ROOT ./)
endif ()


include_directories(
        ${RK_ROOT}/include
)
link_directories(
        ${RK_ROOT}/lib
)


set(PROJECT_SOURCES
        main.c
        camir_control.c
        camrgb_control.c
        display.c
        rkdrm_display.c

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
    )

endif ()


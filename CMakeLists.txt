cmake_minimum_required(VERSION 3.10)

project(theTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


if (WIN32)
    set(OPENCV_ROOT P:/projects/cplusplus/OpenCV-MinGW-Build-OpenCV-4.5.5-x64)
    set(CMAKE_PREFIX_PATH
            "${OPENCV_ROOT}"
    )
    find_package(OpenCV REQUIRED)
    include_directories(
            ./include
            ${OpenCV_INCLUDE_DIRS}
    )
else ()
    set(OPENCV_ROOT /data/src/opencv/_install)
    set(CMAKE_PREFIX_PATH
            "${OPENCV_ROOT}"
    )
    find_package(OpenCV REQUIRED)
    include_directories(
            ./include
            ${OpenCV_INCLUDE_DIRS}
    )
    link_directories(
            /data/src/rockchip/_install//lib
            ${OPENCV_ROOT}/lib/opencv4/3rdparty
    )
endif ()


set(PROJECT_SOURCES
        main.cpp
        camir_control.cpp
        camrgb_control.cpp
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
            ${OpenCV_LIBS}
            -llibprotobuf -lade -littnotify -llibjpeg-turbo -llibwebp -llibpng
            -llibtiff -llibopenjp2 -lIlmImf -lzlib -ltegra_hal -ldl -lm -lpthread -lrt
            # Rock chip
            -lrkaiq -leasymedia -ldrm -lrockchip_mpp -lavformat -lavcodec -lswresample -lavutil -lasound
            -lRKAP_3A -lRKAP_ANR -lRKAP_Common -lv4l2 -lrga -lmd_share -lod_share -lrkaiq -lz -lv4lconvert -ljpeg
            -lVSC_Lite -lGAL -lArchModelSw -lNNArchPerf -lOpenVX -lrknn_runtime -lrknn_api
            -lrkisp_api
    )

endif ()


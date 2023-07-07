cmake_minimum_required(VERSION 3.14)

project(gui_onnx VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LINK_WHAT_YOU_USE ON)
set(CXX_STANDARD_REQUIRED true FORCE)

set(LITE_AI_DIR "${CMAKE_SOURCE_DIR}/lite.ai.toolkit")

set(OpenCV_DIR "C:/Apps2/opencv/build")
set(Pylon_DIR "C:/Program Files/Basler/pylon 7/Development/CMake/pylon")
#set(LibTorch_DIR "C:/Program Files/libtorch")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick)
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt5 REQUIRED COMPONENTS Multimedia)
find_package(Qt5 REQUIRED COMPONENTS MultimediaWidgets)
find_package(Qt5 REQUIRED COMPONENTS PrintSupport)
find_package(Qt5 REQUIRED COMPONENTS SerialPort)
find_package(Qt5 REQUIRED COMPONENTS Charts)
find_package(OpenCV REQUIRED)
#find_package(onnxruntime REQUIRED)
find_package(pylon REQUIRED)


#set(LibTorch_DIR "C:/Apps/libtorch-win-shared-with-deps-1.13.1+cu116/libtorch")
#set(LIBTORCH_INCLUDE_DIR "${LibTorch_DIR}/include")
#set(LIBTORCH_LIB_DIR "${LibTorch_DIR}/lib")
set(OpenCV_LIBS opencv_core455 opencv_imgcodecs455 opencv_imgproc455 opencv_video455 opencv_videoio455 opencv_highgui455 opencv_imgcodecs455 opencv_calib3d455 opencv_photo455 opencv_features2d455)
set(PYLON_HOME "C:/Program Files/Basler/pylon 7/Development")
set(PYLON_INCLUDE_DIR "C:/Program Files/Basler/pylon 7/Development/include")
set(PYLON_LIB_DIR "C:/Program Files/Basler/pylon 7/Development/lib/x64")
set(OpenCV_LIB_DIR "C:/Apps2/opencv/build/lib/Release")
set(OpenCV_INCLUDE_DIR "C:/Apps2/opencv/build/include")
set(ONNXRUNTIME_INCLUDE_DIR "${LITE_AI_DIR}/onnxruntime")

set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        includes.h
        controller.h controller.cpp
        ppgsensor.h ppgsensor.cpp
        videocamera.h videocamera.cpp
        landmark.h landmark.cpp
        chart.h chart.cpp
        chart_EAR.h chart_EAR.cpp
        propswidget.h propswidget.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(gui_onnx
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET gui_onnx APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(gui_onnx SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(gui_onnx
          ${PROJECT_SOURCES}
        )
    endif()
endif()

add_compile_options(
        -Wl,--no-as-needed
        -INCLUDE:?searchsorted_cuda@native@at@@YA?AVTensor@2@AEBV32@0_N1@Z%
        -INCLUDE:?warp_size@cuda@at@@YAHXZ
)

set(CMAKE_CXX_FLAGS_RELEASE "-INCLUDE:?warp_size@cuda@at@@YAHXZ -INCLUDE:?searchsorted_cuda@native@at@@YA?AVTensor@2@AEBV32@0_N1@Z%")


target_include_directories(gui_onnx PUBLIC ${OpenCV_INCLUDE_DIR})
target_include_directories(gui_onnx PUBLIC ${PYLON_INCLUDE_DIR})
#target_include_directories(gui_onnx PUBLIC ${LIBTORCH_INCLUDE_DIR})
#target_include_directories(gui_onnx PUBLIC "${LIBTORCH_INCLUDE_DIR}/torch/csrc/api/include")
target_include_directories(gui_onnx PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/include")
target_include_directories(gui_onnx PUBLIC ${ONNXRUNTIME_INCLUDE_DIR})
target_include_directories(gui_onnx PUBLIC "${LITE_AI_DIR}/build/lite.ai.toolkit/include")

#target_link_directories(gui_onnx PUBLIC ${LIBTORCH_LIB_DIR})
target_link_directories(gui_onnx PUBLIC ${PYLON_LIB_DIR})
target_link_directories(gui_onnx PUBLIC ${OpenCV_LIB_DIR})
target_link_directories(gui_onnx PUBLIC "C:/Program Files/NVIDIA Corporation/NvToolsExt/lib/x64")
target_link_directories(gui_onnx PUBLIC "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64")


target_link_libraries(gui_onnx PUBLIC
  ${OpenCV_LIBS}
  "${LITE_AI_DIR}/build/lite.ai.toolkit/lib/Release/lite.ai.toolkit.lib"
  "${LITE_AI_DIR}/lib/windows/onnxruntime.lib"
  "${LITE_AI_DIR}/lib/windows/onnxruntime_providers_cuda.lib"
  "${LITE_AI_DIR}/lib/windows/onnxruntime_providers_shared.lib"
  Qt::Charts
  Qt5::Core
  Qt5::Gui
  Qt5::Multimedia
  Qt5::MultimediaWidgets
  Qt5::PrintSupport
  Qt5::SerialPort
  "${PYLON_LIB_DIR}/GCBase_MD_VC141_v3_1_Basler_pylon.lib"
  "${PYLON_LIB_DIR}/GenApi_MD_VC141_v3_1_Basler_pylon.lib"
  "${PYLON_LIB_DIR}/PylonBase_v7_2.lib"
  "${PYLON_LIB_DIR}/PylonC.lib"
  "${PYLON_LIB_DIR}/PylonUtility_v7_2.lib"
  "${PYLON_LIB_DIR}/PylonGUI_v7_2.lib"
  "${PYLON_LIB_DIR}/PylonDataProcessing_v1_1.lib"
#  ${LIBTORCH_LIB_DIR}/torch.lib
#  ${LIBTORCH_LIB_DIR}/torch_cuda.lib
#  ${LIBTORCH_LIB_DIR}/torch_cpu.lib
#  ${LIBTORCH_LIB_DIR}/torch_cuda_cu.lib
#  ${LIBTORCH_LIB_DIR}/c10.lib
#  ${LIBTORCH_LIB_DIR}/c10_cuda.lib
#  ${LIBTORCH_LIB_DIR}/caffe2_nvrtc.lib
  "C:/Program Files/NVIDIA Corporation/NvToolsExt/lib/x64/nvToolsExt64_1.lib"
  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64/cufft.lib"
  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64/curand.lib"
  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64/cublas.lib"
  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64/cudart_static.lib"
  "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.6/lib/x64/cudnn.lib"
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick

)

set_target_properties(gui_onnx PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS gui_onnx
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(gui_onnx)
    qt_finalize_executable(gui_onnx)
endif()

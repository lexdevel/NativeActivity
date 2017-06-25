cmake_minimum_required(VERSION 3.0)
project(NativeActivity)

set(SRC)
set(LIB)

# Add android-native-app-glue package
add_library(appglue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
include_directories(${ANDROID_NDK}/sources/android/native_app_glue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SRC ${SRC} cpp/GraphicsDevice.cpp)
set(SRC ${SRC} cpp/Application.cpp)
set(SRC ${SRC} cpp/main.cpp)

set(LIB ${LIB} android)
set(LIB ${LIB} appglue)
set(LIB ${LIB} log)
set(LIB ${LIB} EGL)
set(LIB ${LIB} GLESv2)

add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} ${LIB})

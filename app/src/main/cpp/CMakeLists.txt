# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Configure CMake for full game library
cmake_minimum_required(VERSION 3.22.1)
project("codini")

# Collect all Codini source files
set(CODINI_SOURCES
        main.cpp
        AndroidOut.cpp
        Renderer.cpp
        Shader.cpp
        TextureAsset.cpp
        Utility.cpp
)

# Include GameActivity and related native_app_glue and game-text-input sources
set(GAME_ACTIVITY_DIR "$ENV{ANDROID_SDK_ROOT}/extras/android/game-activity/")
if(EXISTS "${GAME_ACTIVITY_DIR}/GameActivity.cpp")
    list(APPEND CODINI_SOURCES
        ${GAME_ACTIVITY_DIR}/GameActivity.cpp
        ${GAME_ACTIVITY_DIR}/../native_app_glue/android_native_app_glue.c
        ${GAME_ACTIVITY_DIR}/game-text-input/gametextinput.cpp
    )
else()
    message(STATUS "GameActivity sources not found at ${GAME_ACTIVITY_DIR}, skipping integration")
endif()

# Build shared library from all sources
add_library(codini SHARED ${CODINI_SOURCES})

# Link system libraries
target_link_libraries(codini
        EGL
        GLESv3
        jnigraphics
        android
        log)
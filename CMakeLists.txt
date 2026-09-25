cmake_minimum_required(VERSION 3.16)
project(FalconPluginAPI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(FalconPluginAPI INTERFACE)
add_library(Falcon::PluginAPI ALIAS FalconPluginAPI)
target_include_directories(FalconPluginAPI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/native/include)
target_compile_features(FalconPluginAPI INTERFACE cxx_std_17)

add_library(FalconInternalPluginAPI INTERFACE)
add_library(Falcon::InternalPluginAPI ALIAS FalconInternalPluginAPI)
target_include_directories(FalconInternalPluginAPI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/internal/include)

include(${CMAKE_CURRENT_SOURCE_DIR}/internal/cmake/FalconInternalPlugin.cmake)

function(falcon_add_plugin target)
    add_library(${target} SHARED ${ARGN})
    target_link_libraries(${target} PRIVATE Falcon::PluginAPI)
    set_target_properties(${target} PROPERTIES
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON)
    if (MINGW)
        target_link_options(${target} PRIVATE -static-libgcc -static-libstdc++)
    endif ()
endfunction()

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(FALCON_PLUGIN_API_TOP_LEVEL ON)
else ()
    set(FALCON_PLUGIN_API_TOP_LEVEL OFF)
endif ()

option(FALCON_PLUGIN_API_BUILD_EXAMPLES "Build the example plugins" ${FALCON_PLUGIN_API_TOP_LEVEL})

if (FALCON_PLUGIN_API_BUILD_EXAMPLES)
    add_subdirectory(examples/native/HelloPlugin)
endif ()

option(FALCON_PLUGIN_API_BUILD_INTERNAL_EXAMPLES
        "Build the internal example plugins against FALCON_SDK_DIR or FALCON_BUILD_DIR" OFF)

if (FALCON_PLUGIN_API_BUILD_INTERNAL_EXAMPLES)
    add_subdirectory(examples/internal)
endif ()

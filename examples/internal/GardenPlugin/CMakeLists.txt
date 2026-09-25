file(GLOB_RECURSE GARDEN_PLUGIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

falcon_add_internal_plugin(GardenPlugin ${GARDEN_PLUGIN_SOURCES})
target_include_directories(GardenPlugin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE HELLO_PLUGIN_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

falcon_add_plugin(HelloPlugin ${HELLO_PLUGIN_SOURCES})
target_include_directories(HelloPlugin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

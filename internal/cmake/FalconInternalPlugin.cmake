function(falcon_add_internal_plugin target)
    if(TARGET FalconServer)
        get_target_property(serverExports FalconServer ENABLE_EXPORTS)
        if(NOT serverExports)
            message(FATAL_ERROR "${target} needs a server configured with FALCON_EXPORT_SYMBOLS=ON")
        endif()

        set(server FalconServer)
        set(includeDirectories "$<TARGET_PROPERTY:FalconServer,INCLUDE_DIRECTORIES>")
        set(compileDefinitions "$<TARGET_PROPERTY:FalconServer,COMPILE_DEFINITIONS>")
    elseif(FALCON_BUILD_DIR)
        set(sdkFile "${FALCON_BUILD_DIR}/FalconInternalSDK.cmake")
        if(NOT EXISTS "${sdkFile}")
            message(FATAL_ERROR "${sdkFile} is missing: configure Falcon with FALCON_EXPORT_SYMBOLS=ON first")
        endif()
        include("${sdkFile}")

        if(NOT CMAKE_CXX_COMPILER_ID STREQUAL FALCON_INTERNAL_CXX_COMPILER_ID
                OR NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL FALCON_INTERNAL_CXX_COMPILER_VERSION)
            message(FATAL_ERROR "The server in ${FALCON_BUILD_DIR} was built with ${FALCON_INTERNAL_CXX_COMPILER_ID} "
                    "${FALCON_INTERNAL_CXX_COMPILER_VERSION}, ${target} would be built with ${CMAKE_CXX_COMPILER_ID} "
                    "${CMAKE_CXX_COMPILER_VERSION}")
        endif()

        if(NOT TARGET Falcon::Server)
            add_executable(Falcon::Server IMPORTED GLOBAL)
            set_target_properties(Falcon::Server PROPERTIES
                    IMPORTED_LOCATION "${FALCON_INTERNAL_EXECUTABLE}"
                    ENABLE_EXPORTS ON)
            if(WIN32)
                set_target_properties(Falcon::Server PROPERTIES IMPORTED_IMPLIB "${FALCON_INTERNAL_LINKER_FILE}")
            endif()
        endif()

        set(server Falcon::Server)
        set(includeDirectories ${FALCON_INTERNAL_INCLUDE_DIRECTORIES})
        set(compileDefinitions ${FALCON_INTERNAL_COMPILE_DEFINITIONS})
    else()
        message(FATAL_ERROR "${target} is an internal plugin: build it in a Falcon build tree with "
                "FALCON_BUILD_INTERNAL_PLUGINS=ON, or set FALCON_BUILD_DIR to a built Falcon")
    endif()

    add_library(${target} MODULE ${ARGN})
    target_include_directories(${target} PRIVATE ${includeDirectories})
    target_compile_definitions(${target} PRIVATE ${compileDefinitions})
    target_link_libraries(${target} PRIVATE Falcon::InternalPluginAPI ${server})
    set_target_properties(${target} PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON)

    if(WIN32)
        set_target_properties(${target} PROPERTIES PREFIX "" SUFFIX ".dll")
    elseif(APPLE)
        set_target_properties(${target} PROPERTIES PREFIX "lib" SUFFIX ".dylib")
    else()
        set_target_properties(${target} PROPERTIES PREFIX "lib" SUFFIX ".so")
    endif()

    if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${target} PRIVATE -static -fuse-ld=lld)
    endif()
endfunction()

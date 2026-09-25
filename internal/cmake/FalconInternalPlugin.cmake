function(_falcon_load_internal_sdk target directory)
    set(sdkFile "${directory}/FalconInternalSDK.cmake")
    if(NOT EXISTS "${sdkFile}")
        message(FATAL_ERROR "${sdkFile} is missing: point FALCON_SDK_DIR at an extracted Falcon SDK, or "
                "FALCON_BUILD_DIR at a Falcon build configured with FALCON_EXPORT_SYMBOLS=ON")
    endif()
    include("${sdkFile}")

    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL FALCON_INTERNAL_CXX_COMPILER_ID
            OR NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL FALCON_INTERNAL_CXX_COMPILER_VERSION)
        message(FATAL_ERROR "The server in ${directory} was built with ${FALCON_INTERNAL_CXX_COMPILER_ID} "
                "${FALCON_INTERNAL_CXX_COMPILER_VERSION}, ${target} would be built with ${CMAKE_CXX_COMPILER_ID} "
                "${CMAKE_CXX_COMPILER_VERSION}")
    endif()

    set(FALCON_INTERNAL_EXECUTABLE "${FALCON_INTERNAL_EXECUTABLE}" PARENT_SCOPE)
    set(FALCON_INTERNAL_LINKER_FILE "${FALCON_INTERNAL_LINKER_FILE}" PARENT_SCOPE)
    set(FALCON_INTERNAL_INCLUDE_DIRECTORIES "${FALCON_INTERNAL_INCLUDE_DIRECTORIES}" PARENT_SCOPE)
    set(FALCON_INTERNAL_COMPILE_DEFINITIONS "${FALCON_INTERNAL_COMPILE_DEFINITIONS}" PARENT_SCOPE)
endfunction()

function(falcon_add_internal_plugin target)
    add_library(${target} MODULE ${ARGN})

    if(TARGET FalconServer)
        get_target_property(serverExports FalconServer ENABLE_EXPORTS)
        if(NOT serverExports)
            message(FATAL_ERROR "${target} needs a server configured with FALCON_EXPORT_SYMBOLS=ON")
        endif()

        target_include_directories(${target} PRIVATE "$<TARGET_PROPERTY:FalconServer,INCLUDE_DIRECTORIES>")
        target_compile_definitions(${target} PRIVATE "$<TARGET_PROPERTY:FalconServer,COMPILE_DEFINITIONS>")
        target_link_libraries(${target} PRIVATE FalconServer)
    else()
        if(FALCON_SDK_DIR)
            _falcon_load_internal_sdk(${target} "${FALCON_SDK_DIR}")
        elseif(FALCON_BUILD_DIR)
            _falcon_load_internal_sdk(${target} "${FALCON_BUILD_DIR}")
        else()
            message(FATAL_ERROR "${target} is an internal plugin: set FALCON_SDK_DIR to the Falcon SDK of the server "
                    "release, set FALCON_BUILD_DIR to a built Falcon, or build it in a Falcon build tree with "
                    "FALCON_BUILD_INTERNAL_PLUGINS=ON")
        endif()

        target_include_directories(${target} PRIVATE ${FALCON_INTERNAL_INCLUDE_DIRECTORIES})
        target_compile_definitions(${target} PRIVATE ${FALCON_INTERNAL_COMPILE_DEFINITIONS})

        if(WIN32)
            target_link_libraries(${target} PRIVATE "${FALCON_INTERNAL_LINKER_FILE}")
        elseif(APPLE)
            target_link_options(${target} PRIVATE "LINKER:-bundle_loader,${FALCON_INTERNAL_EXECUTABLE}")
        endif()
    endif()

    target_link_libraries(${target} PRIVATE Falcon::InternalPluginAPI)
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

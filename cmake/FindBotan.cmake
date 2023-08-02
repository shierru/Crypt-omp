# This is a modified version especially for this project.
# (Original: FindBotan.cmake by Tectu)

cmake_minimum_required(VERSION 3.19)

# Header
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-m32")
endif()

# Includes
include(FetchContent)
include(CMakePrintHelpers)

# Find python
find_package(
    Python
    COMPONENTS
        Interpreter
    REQUIRED
)

# Assemble version string
set(Botan_VERSION_STRING ${Botan_FIND_VERSION_MAJOR}.${Botan_FIND_VERSION_MINOR}.${Botan_FIND_VERSION_PATCH})

# Assemble download URL
set(DOWNLOAD_URL https://github.com/randombit/botan/archive/refs/tags/${Botan_VERSION_STRING}.tar.gz)

# Just do a dummy download to see whether we can download the tarball
file(
    DOWNLOAD
    ${DOWNLOAD_URL}
    STATUS download_status
)
if (NOT download_status EQUAL 0)
    message(FATAL_ERROR "Could not download Botan tarball (status = ${download_status}): ${DOWNLOAD_URL}")
endif()

# Download the tarball
FetchContent_Declare(
    botan_upstream
    URL ${DOWNLOAD_URL}
)
FetchContent_MakeAvailable(botan_upstream)

# Heavy lifting by cmake
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Botan DEFAULT_MSG Botan_VERSION_STRING)

## Function to create a list of enabled modules 
function(botan_modules_enable MODULES)
    foreach(module_index RANGE 0 ${ARGC})
        list(APPEND modules_list ${ARGV${module_index}})
    endforeach()
    list(JOIN modules_list "," modules_list)

    set_property(GLOBAL PROPERTY enable_modules_list "${modules_list}")
endfunction()

## Function to create a list of disabled modules
function(botan_modules_disable MODULES)
    foreach(module_index RANGE 0 ${ARGC})
        list(APPEND modules_list ${ARGV${module_index}})
    endforeach()
    list(JOIN modules_list "," modules_list)

    set_property(GLOBAL PROPERTY disable_modules_list "${modules_list}")
endfunction()

## Function to generate a target named 'TARGET_NAME' with specific Botan modules enabled.
function(botan_generate TARGET_NAME)
    # Determine botan compiler ID (--cc parameter of configure.py)
    set(BOTAN_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})
    string(TOLOWER ${BOTAN_COMPILER_ID} BOTAN_COMPILER_ID)
    if (BOTAN_COMPILER_ID STREQUAL "gnu")
        set(BOTAN_COMPILER_ID "gcc")
    endif()

    # Get list of enabled and disabled modules
    get_property(ENABLE_MODULES GLOBAL PROPERTY enable_modules_list)
    get_property(DISABLE_MODULES GLOBAL PROPERTY disable_modules_list)

    # Run the configure.py script
    add_custom_command(
        OUTPUT botan_all.cpp botan_all.h
        COMMENT "Generating Botan amalgamation files botan_all.cpp and botan_all.h"
        COMMAND ${Python_EXECUTABLE}
            ${botan_upstream_SOURCE_DIR}/configure.py
            --quiet
            --cp=i386
            --cc-bin=${CMAKE_CXX_COMPILER}
            --cc=${BOTAN_COMPILER_ID}
            $<$<BOOL:${MINGW}>:--os=mingw>
            --disable-shared
            --amalgamation
            --minimized-build
            --enable-modules=${ENABLE_MODULES}
            --disable-modules=${DISABLE_MODULES}
    )

    # Create target
    set(TARGET ${TARGET_NAME})
    add_library(${TARGET} STATIC)

    target_sources(
        ${TARGET}
        PUBLIC
            ${CMAKE_CURRENT_BINARY_DIR}/botan_all.h
        PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/botan_all.cpp
    )
    target_include_directories(
        ${TARGET}
        INTERFACE
            ${CMAKE_CURRENT_BINARY_DIR}
    )
    target_compile_definitions(
        ${TARGET} 
        PRIVATE
            _CRT_SECURE_NO_WARNINGS
    )
    set_target_properties(
        ${TARGET}
        PROPERTIES
            POSITION_INDEPENDENT_CODE ON
    )
endfunction()
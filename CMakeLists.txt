cmake_minimum_required(VERSION 3.27)

project(CRYPT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CRYPT_OUTPUT_NAME "Crypt")
set(CRYPT_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})

# CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Botan
find_package(Botan 3.0.0 REQUIRED)

botan_modules_enable(
    # SHA
    sha2_32_x86
    sha3
    
    # MD
    md4
    md5
)
botan_modules_disable(pkcs11)

botan_generate(BOTAN)

# Open.MP SDK
add_subdirectory(lib/omp-sdk)

# Crypt
add_library(${PROJECT_NAME} SHARED 
    src/main.cpp
    src/crypt-component.cpp
    src/hash-function.cpp
    src/natives.cpp
)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CRYPT_OUTPUT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE
    lib

    lib/pawn/source
    lib/pawn/source/linux
)

if(CRYPT_COMPILER_ID STREQUAL "MSVC")
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DHAVE_STDINT_H=1)
elseif(CRYPT_COMPILER_ID STREQUAL "GNU")
    set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE OFF)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_OPTIONS "-m32 -O3 -w")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-m32 -O3 -static-libgcc -static-libstdc++")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE -DPAWN_CELL_SIZE=32)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    # Open.MP SDK
    OMP-SDK

    # Botan
    BOTAN
)
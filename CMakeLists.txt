cmake_minimum_required(VERSION 3.27)

project(COMPONENT VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

set(CRYPT_OUTPUT_NAME "Crypt")
set(CRYPT_COMPILER_ID ${CMAKE_CXX_COMPILER_ID})

# CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Packages
find_package(GroupSourcesByFolder REQUIRED)

# Botan package
find_package(Botan 3.0.0 REQUIRED)

# Botan functions
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

# Target
file(GLOB_RECURSE SOURCES_LIST "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES_LIST})

GroupSourcesByFolder(${PROJECT_NAME})

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
	
    add_compile_options(-m32 -O3 -w)
    add_link_options(-m32 -O3 -static-libgcc -static-libstdc++)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE -DPAWN_CELL_SIZE=32)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    # Open.MP SDK
    OMP-SDK

    # Botan
    BOTAN
)
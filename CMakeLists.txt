cmake_minimum_required(VERSION 3.29)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
	message(FATAL_ERROR "in source builds are not permitted")
endif()

if(NOT LINUX)
	message(FATAL_ERROR "luacc only supports linux as of now. sorry!")
endif()

project(luacc VERSION 1.0.0)

file(GLOB_RECURSE LUACC_SRC "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${LUACC_SRC})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_options(${PROJECT_NAME} PRIVATE "-g")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE -DLUACC_VERSION="luacc version ${PROJECT_VERSION}")

# include paths
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

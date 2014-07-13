#cmake file for CULib
#author chenfjm@gmail.com)

cmake_minimum_required(VERSION 2.8)

project(dui)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} root_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Control control_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Core core_src)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Utils utils_src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Control)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Utils)

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_library(CULib SHARED ${control_src} ${core_src} ${utils_src} ${root_src})

add_definitions(-DUILIB_EXPORTS)

set_target_properties(CULib PROPERTIES OUTPUT_NAME "CULib")
add_custom_command(TARGET CULib POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${PROJECT_BINARY_DIR}/lib/CULib.dll ${PROJECT_SOURCE_DIR}/bin/CULib.dll)
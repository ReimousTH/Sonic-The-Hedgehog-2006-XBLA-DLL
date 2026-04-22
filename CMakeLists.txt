cmake_minimum_required(VERSION 3.8)
project(Sonic2006)

set(PATCH_XEX "${CMAKE_SOURCE_DIR}/thirdparty/patch_xex")
set(DLL_PATH "${CMAKE_SOURCE_DIR}/dll")

add_subdirectory(thirdparty/patch_xex)
add_subdirectory(dll)


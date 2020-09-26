cmake_minimum_required(VERSION 3.0.0)
project(autumnframework VERSION 0.1.0)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

set(INCLUDE_ALL_DIR
    ${CMAKE_SOURCE_DIR}/include/interface
    ${CMAKE_SOURCE_DIR}/include/core
)

include_directories(${INCLUDE_ALL_DIR})

aux_source_directory(${CMAKE_SOURCE_DIR}/src DIR_LIB_SRCS)

set(ALL_SRCS ${DIR_LIB_SRCS})

add_library(
    autumnframework
    STATIC
    ${ALL_SRCS}
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

cmake_minimum_required(VERSION 3.0.0)
project(Cambricon-R-simulator.code VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD_REQUIRED 99)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

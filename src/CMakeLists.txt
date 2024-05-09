file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include/)
include_directories(${PROJECT_SOURCE_DIR}/ext/DRAMsim3/src/)

add_executable(Cambricon-R-simulator ${SOURCES})
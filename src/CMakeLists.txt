file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include/)

add_executable(Cambricon-R-simulator ${SOURCES})
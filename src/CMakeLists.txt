file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(Cambricon-R-simulator ${SOURCES})

target_include_directories(Cambricon-R-simulator PRIVATE ${PROJECT_SOURCE_DIR}/include/)
target_include_directories(Cambricon-R-simulator PRIVATE ${PROJECT_SOURCE_DIR}/ext/DRAMsim3/src/)

target_link_libraries(Cambricon-R-simulator PRIVATE dramsim3)
file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/*.cpp)

include_directories(${PROJECT_SOURCE_DIR}/include/)

foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    add_test(
        NAME ${test_name}
        COMMAND ${test_name}
    )
endforeach()
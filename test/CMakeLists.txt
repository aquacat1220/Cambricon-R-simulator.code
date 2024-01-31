include_directories(${PROJECT_SOURCE_DIR}/include/)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(REMOVE_ITEM sources ${PROJECT_SOURCE_DIR}/src/main.cpp)

file(GLOB_RECURSE test_sources ${PROJECT_SOURCE_DIR}/test/**.cpp)

foreach(test_source ${test_sources})
    get_filename_component(test_dir ${test_source} PATH)
    cmake_path(
        RELATIVE_PATH test_dir
        BASE_DIRECTORY ${PROJECT_SOURCE_DIR}/test
        OUTPUT_VARIABLE test_prefix
        )
    if(test_prefix STREQUAL ".")
        set(test_prefix "")
    else()
        set(test_prefix ${test_prefix}/)
    endif()

    get_filename_component(test_source_name ${test_source} NAME_WE)
    add_executable(${test_source_name} ${test_source} ${sources})
    add_test(
        NAME ${test_prefix}${test_source_name}
        COMMAND $<TARGET_FILE:${test_source_name}>
    )
endforeach()
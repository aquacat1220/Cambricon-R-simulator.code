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
        set(test_prefix_noslash "")
    else()
        set(test_prefix ${test_prefix}/)
        string(REPLACE "/" "_" test_prefix_noslash ${test_prefix})
    endif()

    get_filename_component(test_source_name ${test_source} NAME_WE)
    add_executable(${test_prefix_noslash}${test_source_name} ${test_source} ${sources})

    target_include_directories(${test_prefix_noslash}${test_source_name} PRIVATE ${PROJECT_SOURCE_DIR}/include/)

    target_link_libraries(${test_prefix_noslash}${test_source_name} PRIVATE dramsim3)

    add_test(
        NAME ${test_prefix}${test_source_name}
        COMMAND $<TARGET_FILE:${test_prefix_noslash}${test_source_name}>
    )
endforeach()
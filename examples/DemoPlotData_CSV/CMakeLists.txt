cmake_minimum_required(VERSION 3.16)

project(DemoPlotData_CSV
    VERSION 1.0.0
    DESCRIPTION "Cross-platform CSV plot-data generator"
    LANGUAGES C
)

add_executable(DemoPlotData_CSV src/main.c)
target_compile_features(DemoPlotData_CSV PRIVATE c_std_11)

if(MSVC)
    # C5105 originates in older Windows SDK headers, not in this project.
    target_compile_options(DemoPlotData_CSV PRIVATE /W4 /wd5105)
    target_compile_definitions(DemoPlotData_CSV PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
    )
    target_link_libraries(DemoPlotData_CSV PRIVATE ws2_32)
else()
    target_compile_options(DemoPlotData_CSV PRIVATE
        -Wall -Wextra -Wpedantic
    )
    target_link_libraries(DemoPlotData_CSV PRIVATE m)
endif()

include(CTest)
if(BUILD_TESTING)
    add_test(
        NAME generate_three_csv_samples
        COMMAND DemoPlotData_CSV --samples 3 --no-delay --header
    )
    set_tests_properties(generate_three_csv_samples PROPERTIES
        PASS_REGULAR_EXPRESSION "time_s,x,y,z"
    )
endif()

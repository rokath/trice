cmake_minimum_required(VERSION 3.16)

project(DemoPlotData_CSV
    VERSION 1.0.0
    DESCRIPTION "Cross-platform CSV plot-data generator"
    LANGUAGES C
)

# Single-configuration generators need an explicit build type. Multi-
# configuration generators such as Visual Studio select it at build time.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

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

# Keep the runnable demo separate from CMake's intermediate build tree. The
# build entry point installs this target into the demo-local ./bin directory.
install(TARGETS DemoPlotData_CSV
    RUNTIME DESTINATION bin
)

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

cmake_minimum_required(VERSION 3.16)

project(DemoPlotData_Trice
    VERSION 1.0.0
    DESCRIPTION "Cross-platform TRice plot-data generator"
    LANGUAGES C
)

# Single-configuration generators need an explicit build type. Multi-
# configuration generators such as Visual Studio select it at build time.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# The project is intended to live in trice/examples/DemoPlotData_Trice.
# Therefore the unmodified Trice target library is two levels above this file.
get_filename_component(TRICE_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    ABSOLUTE
)

if(NOT EXISTS "${TRICE_SOURCE_DIR}/trice.h")
    message(FATAL_ERROR
        "TRice sources were not found at ${TRICE_SOURCE_DIR}. "
        "Place this project in trice/examples/DemoPlotData_Trice."
    )
endif()

add_executable(DemoPlotData_Trice
    src/main.c
    "${TRICE_SOURCE_DIR}/trice.c"
    "${TRICE_SOURCE_DIR}/trice32.c"
    "${TRICE_SOURCE_DIR}/trice64.c"
    "${TRICE_SOURCE_DIR}/triceStackBuffer.c"
    "${TRICE_SOURCE_DIR}/cobsEncode.c"
    "${TRICE_SOURCE_DIR}/tcobsv1Encode.c"
)

target_compile_features(DemoPlotData_Trice PRIVATE c_std_11)

# Put the project directory first so trice.h finds this demo's triceConfig.h.
target_include_directories(DemoPlotData_Trice PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${TRICE_SOURCE_DIR}"
)

if(MSVC)
    # C5105 originates in older Windows SDK headers, not in this project.
    target_compile_options(DemoPlotData_Trice PRIVATE /W4 /wd5105)
    target_compile_definitions(DemoPlotData_Trice PRIVATE
        _CRT_SECURE_NO_WARNINGS
        WIN32_LEAN_AND_MEAN
    )
    target_link_libraries(DemoPlotData_Trice PRIVATE ws2_32)
else()
    target_compile_options(DemoPlotData_Trice PRIVATE
        -Wall -Wextra -Wpedantic
    )
    target_link_libraries(DemoPlotData_Trice PRIVATE m)
endif()

# Keep the runnable demo separate from CMake's intermediate build tree. The
# build entry point installs this target into the demo-local ./bin directory.
install(TARGETS DemoPlotData_Trice
    RUNTIME DESTINATION bin
)

include(CTest)
if(BUILD_TESTING)
    add_test(
        NAME generate_three_trice_samples
        COMMAND DemoPlotData_Trice
            --samples 3
            --no-delay
            --output DemoPlotData_Trice_test.bin
    )
endif()

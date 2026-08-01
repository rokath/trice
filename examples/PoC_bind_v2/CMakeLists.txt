cmake_minimum_required(VERSION 3.16)

project(PoC_bind_v2
    VERSION 1.0.0
    DESCRIPTION "Compiler PoC for source-line Trice ID sidecars"
    LANGUAGES C
)

# The example intentionally compiles the repository's target library directly.
get_filename_component(TRICE_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/../../src"
    ABSOLUTE
)

if(NOT EXISTS "${TRICE_SOURCE_DIR}/trice.h")
    message(FATAL_ERROR
        "Trice target sources are missing at ${TRICE_SOURCE_DIR}. "
        "Keep this example below the repository examples directory."
    )
endif()

add_executable(PoC_bind_v2
    main.c
    device/dev.c
    components/deep/device/dev.c
    "${TRICE_SOURCE_DIR}/trice.c"
    "${TRICE_SOURCE_DIR}/trice32.c"
    "${TRICE_SOURCE_DIR}/trice64.c"
    "${TRICE_SOURCE_DIR}/triceStackBuffer.c"
    "${TRICE_SOURCE_DIR}/cobsEncode.c"
    "${TRICE_SOURCE_DIR}/tcobsv1Encode.c"
)

target_compile_features(PoC_bind_v2 PRIVATE c_std_11)

# The project directory must precede src so trice.h finds this triceConfig.h.
target_include_directories(PoC_bind_v2 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/triceIDs"
    "${TRICE_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(PoC_bind_v2 PRIVATE /W4)
else()
    target_compile_options(PoC_bind_v2 PRIVATE -Wall -Wextra -Wpedantic)
endif()

cmake_minimum_required(VERSION 3.16)

project(PoC_bind_generator
    VERSION 1.0.0
    DESCRIPTION "End-to-end compiler PoC for generated Trice bind sidecars"
    LANGUAGES C
)

# The PoC deliberately builds the target library directly from the repository.
get_filename_component(TRICE_REPOSITORY_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/../.."
    ABSOLUTE
)
set(TRICE_SOURCE_DIR "${TRICE_REPOSITORY_ROOT}/src")

if(NOT EXISTS "${TRICE_SOURCE_DIR}/trice.h")
    message(FATAL_ERROR
        "Trice target sources are missing below ${TRICE_SOURCE_DIR}. "
        "Keep this PoC in the repository examples directory."
    )
endif()

find_program(GO_EXECUTABLE NAMES go REQUIRED)

# Running bind before every compile is intentional because physical source lines
# are the target-side lookup identity and stale sidecars are not build-safe.
add_custom_target(PoC_bind_generator_sidecars ALL
    COMMAND "${GO_EXECUTABLE}" run ../../cmd/trice bind
        -src .
        -exclude ./build
        -til til.json
        -li li.json
        -liRoot .
        -bindDir ./build/triceIDs
        -IDMin 1000
        -IDMax 1999
        -IDMethod upward
        -defaultStampSize 16
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    COMMENT "Generating Trice bind sidecars"
    VERBATIM
)

add_executable(PoC_bind_generator
    main.c
    device/dev.c
    components/deep/device/dev.c
    "${TRICE_SOURCE_DIR}/trice.c"
    "${TRICE_SOURCE_DIR}/trice8.c"
    "${TRICE_SOURCE_DIR}/trice16.c"
    "${TRICE_SOURCE_DIR}/trice32.c"
    "${TRICE_SOURCE_DIR}/trice64.c"
    "${TRICE_SOURCE_DIR}/triceStackBuffer.c"
    "${TRICE_SOURCE_DIR}/cobsEncode.c"
    "${TRICE_SOURCE_DIR}/tcobsv1Encode.c"
)
add_dependencies(PoC_bind_generator PoC_bind_generator_sidecars)

target_compile_features(PoC_bind_generator PRIVATE c_std_11)

# The local config and generated sidecars precede the library include directory.
target_include_directories(PoC_bind_generator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/build/triceIDs"
    "${TRICE_SOURCE_DIR}"
)

if(MSVC)
    target_compile_options(PoC_bind_generator PRIVATE /W4)
else()
    target_compile_options(PoC_bind_generator PRIVATE -Wall -Wextra -Wpedantic)
endif()

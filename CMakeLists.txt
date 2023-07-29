cmake_minimum_required(VERSION 3.5)
project(asteroids)
enable_testing()

set(CMAKE_C_STANDARD 99)

set(VCPKG_ROOT "C:/Users/wolfg/dev/vcpkg")
set(LIBRARY_PATH "${VCPKG_ROOT}/installed/x64-windows/lib")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /we4047 /we4013")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /O2 /Ob2 /GL")
    set(CMAKE_TOOLCHAIN_FILE=${VCPG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /LTCG")
    set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /LTCG")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")   
endif()

set(GRV_DIR c:/users/wolfg/dev/grv)
include_directories(include ${GRV_DIR}/include)

add_executable(
    asteroids WIN32
    src/bitmap_font.c
    src/main.c
    src/game_state.c
    src/color_palette.c
    src/frame_buffer.c
    src/math.c
    src/recti.c
    src/transform2d.c
    src/gfxa.c
    src/vec2f.c
    src/vec2i.c
    src/on_draw.c
)

target_link_libraries(
    asteroids
    SDL2.lib
    ${LIBRARY_PATH}/SDL2.lib
    ${LIBRARY_PATH}/manual-link/SDL2main.lib
)


add_executable(
    test_math
    test/test_math.c
    src/math.c
    ${GRV_DIR}/src/grv_str.c
    ${GRV_DIR}/src/grv_strarr.c
)

add_test(NAME test_math COMMAND test_math)

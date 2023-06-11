cmake_minimum_required(VERSION 3.5)

project(asteroids)

set(CMAKE_C_STANDARD 99)

set(VCPKG_ROOT "C:/Users/wolfg/dev/vcpkg")
set(LIBRARY_PATH "${VCPKG_ROOT}/installed/x64-windows/lib")

if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W2 /we4047")
    set(CMAKE_TOOLCHAIN_FILE=${VCPG_ROOT}/scripts/buildsystems/vcpkg.cmake)
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror")   
endif()

include_directories(src)

add_executable(
    asteroids WIN32
    src/main.c
    src/game_state.c
    src/color_palette.c
    src/frame_buffer.c
)

target_link_libraries(
    asteroids
    SDL2.lib
    ${LIBRARY_PATH}/SDL2.lib
    ${LIBRARY_PATH}/manual-link/SDL2main.lib
)
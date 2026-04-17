# cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# cmake --build build

cmake_minimum_required(VERSION 3.10)

project(jacknjunk LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

# include directories
include_directories(
  ${PROJECT_SOURCE_DIR}/include
)

set(SOURCES
  src/main.c 
  src/scene.c 
  src/entity.c 
  src/map.c
  src/game.c
)

# Add executable using main.c and the generated lexer.c
add_executable(jacknjunk  ${SOURCES})

target_compile_options(jacknjunk
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -fsanitize=address
        -fno-omit-frame-pointer
)

target_link_options(jacknjunk
    PRIVATE
        -fsanitize=address
        -lm
)

target_link_libraries(jacknjunk
  PRIVATE raylib
)

# Web-specific flags
if(EMSCRIPTEN)
    set_target_properties(jacknjunk PROPERTIES SUFFIX ".html")

    target_link_options(jacknjunk PRIVATE
        "--shell-file" "${CMAKE_SOURCE_DIR}/resources/html/shell.html"
        "-sUSE_GLFW=3"
        "-sASYNCIFY"
        "-sALLOW_MEMORY_GROWTH=1"
        "--preload-file" "${CMAKE_SOURCE_DIR}/resources@/resources"
    )
endif()

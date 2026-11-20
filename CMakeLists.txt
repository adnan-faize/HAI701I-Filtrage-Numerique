cmake_minimum_required(VERSION 3.16)
project(FiltrageNumerique LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c")
add_executable(main ${SOURCES})
target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE m)

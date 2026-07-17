cmake_minimum_required(VERSION 3.15)
project(emtypyie-cli-c VERSION 2.2.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-parameter -Wno-format-truncation -Wno-missing-field-initializers")
endif()

add_executable(emtypyie
    src/main.c
    src/shell.c
    src/theme.c
    src/util.c
    src/fetch.c
    src/download.c
    src/project.c
    src/bakafetch.c
    src/runtime.c
    src/auth.c
    lib/cJSON.c
)

target_include_directories(emtypyie PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/lib
)

if(WIN32)
    target_link_libraries(emtypyie PRIVATE winhttp)
else()
    find_package(CURL REQUIRED)
    target_link_libraries(emtypyie PRIVATE CURL::libcurl)
endif()

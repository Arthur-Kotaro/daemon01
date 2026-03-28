cmake_minimum_required(VERSION 4.1.2 FATAL_ERROR)
#cmake_policy(VERSION 3.5)

project(daemon01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Options
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_SANITAZERS "Enable ASAN/UBSAN in debug" OFF)

if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()


# source and include dirs
add_subdirectory(src)


# Install
install(TARGETS daemon01
        RUNTIME DESTINATION /usr/bin
)

install(FILES resources/default.conf
        DESTINATION /etc/daemon01
)


# Tests
add_subdirectory(tests)


# Uninstall
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY)
add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

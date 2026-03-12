cmake_minimum_required(VERSION 4.1)
project(daemon01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Опции
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_SANITAZERS "Enable ASAN/UBSAN in debug" OFF)

if(ENABLE_SANITIZERS AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

find_package(Qt6 COMPONENTS Core Network REQUIRED) # при использовании Qt
find_package(Threads REQUIRED)

#find_package(nlohmann_json REQUIRED)
include(FetchContent)
FetchContent_Declare(nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2)
FetchContent_MakeAvailable(nlohmann_json)

# source and include dirs
add_subdirectory(src)



# install
install(TARGETS daemon01
        RUNTIME DESTINATION /usr/bin
)

install(FILES resources/default_config.json
        DESTINATION /etc/daemon01
)

add_subdirectory(tests)
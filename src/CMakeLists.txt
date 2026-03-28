set(SRC_FILES daemon.cpp config.cpp)

add_library(daemon_lib STATIC ${SRC_FILES})
target_include_directories(daemon_lib PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(daemon_lib PUBLIC cxx_std_17)

add_executable(daemon01 main.cpp)

target_link_libraries(daemon01 PRIVATE
        daemon_lib
)

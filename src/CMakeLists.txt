set(SRC_FILES daemon.cpp)

add_library(daemon_lib STATIC ${SRC_FILES})
target_include_directories(daemon_lib PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_compile_features(daemon_lib PUBLIC cxx_std_20)

add_executable(daemon01 main.cpp)

target_link_libraries(daemon01 PRIVATE
        daemon_lib
        Qt6::Core
        Qt6::Network
        Threads::Threads
        nlohmann_json::nlohmann_json
)
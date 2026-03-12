# Fetch GoogleTest
if (BUILD_TESTS)
    find_package(GTest QUIET)
    if (NOT GTest_FOUND)
        message(STATUS "GTest not found, fetching via FetchContent")
        include(${CMAKE_SOURCE_DIR}/cmake/FetchGTest.cmake)
    endif()

    enable_testing()

    add_executable(test_daemon test_daemon.cpp)
    target_link_libraries(test_daemon PRIVATE GTest::gtest_main daemon_lib)
    target_include_directories(test_daemon PRIVATE ${CMAKE_SOURCE_DIR}/inc)
    add_test(NAME test_daemon COMMAND test_daemon)
endif ()

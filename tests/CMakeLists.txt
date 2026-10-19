add_executable(csp_tests
    test_main.cpp
    test_calendar.cpp
    test_qimen.cpp
    test_snapshot.cpp
)

target_include_directories(csp_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(csp_tests PRIVATE csp_core)
target_compile_definitions(csp_tests PRIVATE CSP_SNAPSHOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/snapshot")

add_test(NAME csp_tests COMMAND csp_tests)

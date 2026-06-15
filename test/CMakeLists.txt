enable_testing()

# -----------------------
# Bitmap test
# -----------------------
add_executable(test_bitmap
    test_bitmap.c
)

target_link_libraries(test_bitmap
    PRIVATE led_matrix
)

add_test(
    NAME bitmap_tests
    COMMAND test_bitmap
)

# -----------------------
# Brightness test
# -----------------------
add_executable(test_brightness
    test_brightness.c
)

target_link_libraries(test_brightness
    PRIVATE led_matrix
)

add_test(
    NAME brightness_tests
    COMMAND test_brightness
)

# -----------------------
# Pattern test
# -----------------------
add_executable(test_pattern
    test_pattern.c
)

target_link_libraries(test_pattern
    PRIVATE led_matrix
)

add_test(
    NAME pattern_tests
    COMMAND test_pattern
)

# -----------------------
# Sleep / Wake test
# -----------------------
add_executable(test_sleep_wake
    test_sleep_wake.c
)

target_link_libraries(test_sleep_wake
    PRIVATE led_matrix
)

add_test(
    NAME sleep_wake_tests
    COMMAND test_sleep_wake
)

# -----------------------
# Column grayscale test
# -----------------------
add_executable(test_column_grayscale
    test_column_grayscale.c
)

target_link_libraries(test_column_grayscale
    PRIVATE led_matrix
)

add_test(
    NAME column_grayscale_tests
    COMMAND test_column_grayscale
)

# -----------------------
# Version test
# -----------------------
add_executable(test_version_display
    test_version_display.c
)

target_link_libraries(test_version_display
    PRIVATE led_matrix
)

add_test(
    NAME version_tests
    COMMAND test_version_display
)


# -----------------------
# CI / Unit Tests
# -----------------------

set(MOCK_SRCS
    mocks.c
)

function(add_ci_test TESTNAME)
    add_executable(${TESTNAME}
        ${TESTNAME}.c
        ${MOCK_SRCS}
    )

    target_link_libraries(${TESTNAME}
        PRIVATE led_matrix
    )

    target_link_options(${TESTNAME}
        PRIVATE
        -Wl,--wrap=open
        -Wl,--wrap=close
        -Wl,--wrap=read
        -Wl,--wrap=write
        -Wl,--wrap=tcgetattr
        -Wl,--wrap=tcsetattr
        -Wl,--wrap=tcflush
    )

    add_test(
        NAME ${TESTNAME}
        COMMAND ${TESTNAME}
    )
endfunction()

add_ci_test(test_bitmap_unit)
add_ci_test(test_send_cmd_unit)
add_ci_test(test_read_unit)
add_ci_test(test_init_unit)
add_ci_test(test_close_unit)
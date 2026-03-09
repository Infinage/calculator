if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_executable(calculatorFuzz calculator_fuzz.cc)
    target_compile_options(calculatorFuzz PRIVATE -fsanitize=fuzzer,undefined -O1)
    target_link_options(calculatorFuzz PRIVATE -fsanitize=fuzzer,undefined)
    target_link_libraries(calculatorFuzz PRIVATE calculator)
    cmake_host_system_information(RESULT nproc QUERY NUMBER_OF_LOGICAL_CORES)
    register_special_target(fuzz)
    add_custom_target(fuzz
        COMMAND ${CMAKE_COMMAND} -E echo "Cleaning old coverage data..."
        COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete

        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/calculatorFuzz
            ${CMAKE_CURRENT_BINARY_DIR}/corpus
            -dict=${CMAKE_CURRENT_SOURCE_DIR}/calculator.dict
            -rss_limit_mb=2048 -max_len=256 -timeout=5 -fork=${nproc}
            -max_total_time=60 -print_final_stats=1
        COMMENT "Running fuzztests on Calculator, will run for 60s"

        DEPENDS calculatorFuzz)
else()
    add_custom_target(fuzz COMMAND false COMMENT 
        "Fuzztests not supported with current compiler ${CMAKE_CXX_COMPILER_ID}")
endif()

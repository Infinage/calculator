include(FetchContent)
FetchContent_Declare(benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG 1a54956777ba672764db09a51960056ea042af7e)
set(BENCHMARK_ENABLE_TESTING OFF)
FetchContent_MakeAvailable(benchmark)

add_executable(calcBench calculator_bench.cc)
target_link_libraries(calcBench PRIVATE calculator benchmark::benchmark)

register_special_target(bench)
add_custom_target(bench DEPENDS calcBench
    COMMAND ${CMAKE_COMMAND} -E echo "Cleaning old coverage data..."
    COMMAND find ${CMAKE_BINARY_DIR} -name "*.gcda" -delete

    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/calcBench 
    --benchmark_out_format=json --benchmark_out=bench.json
    COMMENT "Running benchmarks...")

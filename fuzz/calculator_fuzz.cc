#include <cstdint>
#include <string_view>
#include "calc/calculator.h"

/**
 * Fuzz Target: feed arbitrary input to calculator.
 *
 * Goal:
 * Ensure compute() never crashes, hangs, or triggers UB.
 */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    static Calc::Calculator calc;
    std::string_view input(reinterpret_cast<const char*>(data), size);
    auto result = calc.compute(input);
    (void)result;
    return 0;
}

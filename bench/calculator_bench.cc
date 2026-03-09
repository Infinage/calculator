#include <benchmark/benchmark.h>
#include "calc/calculator.h"

using Calc::Calculator;

/*
------------------------------------------------------------
Helper: run compute() and prevent compiler optimizations
------------------------------------------------------------
*/
static inline void RunCompute(Calculator& calc, std::string_view expr)
{
    auto result = calc.compute(expr);
    benchmark::DoNotOptimize(result);
}

/*
------------------------------------------------------------
Benchmark: Minimal expression

Measures the absolute overhead of the pipeline:
Tokenizer → Parser → Evaluator

Expression is intentionally tiny so we measure
framework + algorithm overhead.
------------------------------------------------------------
*/
static void SimpleAddition(benchmark::State& state)
{
    Calculator calc;
    for (auto _ : state)
    {
        RunCompute(calc, "1+2");
    }
}

/*
------------------------------------------------------------
Benchmark: Typical expression

Represents a real-world calculator input with
mixed operators, parentheses, and floating numbers.
------------------------------------------------------------
*/
static void ComplexExpression(benchmark::State& state)
{
    Calculator calc;
    constexpr std::string_view expr =
        "(12.5 * 4) / (2 + 3.5) - 100.25 + (10 * 2)";
    for (auto _ : state)
    {
        RunCompute(calc, expr);
    }
}

/*
------------------------------------------------------------
Benchmark: Operator-heavy expression

Tests evaluator arithmetic throughput by reducing
tokenization complexity and focusing on operator
evaluation.
------------------------------------------------------------
*/
static void MixedOperators(benchmark::State& state)
{
    Calculator calc;

    constexpr std::string_view expr =
        "1+2*3+4*5+6*7+8*9+10";

    for (auto _ : state)
    {
        RunCompute(calc, expr);
    }
}

/*
------------------------------------------------------------
Benchmark: Long linear expression

Generates expressions like:

1 + 1 + 1 + 1 + ...

This stresses:
- tokenizer throughput
- parser linear performance
- evaluator stack operations
------------------------------------------------------------
*/
static void LongExpression(benchmark::State& state)
{
    Calculator calc;

    std::string expr = "1";

    for (int i = 0; i < state.range(0); ++i)
        expr += "+1";

    for (auto _ : state)
    {
        RunCompute(calc, expr);
    }

    state.SetComplexityN(state.range(0));
}

/*
------------------------------------------------------------
Benchmark: Deep parenthesis nesting

Tests parser behavior under heavy nesting which
stresses the operator stack in the shunting-yard
algorithm.
------------------------------------------------------------
*/
static void DeepNesting(benchmark::State& state)
{
    Calculator calc;
    const auto depth = state.range(0);
    std::string expr;
    expr.reserve(static_cast<std::size_t>(depth) * 2 + 3);

    for (int i = 0; i < depth; ++i)
        expr += "(";

    expr += "1+1";

    for (int i = 0; i < depth; ++i)
        expr += ")";

    for (auto _ : state)
    {
        RunCompute(calc, expr);
    }
}


/*
------------------------------------------------------------
Benchmark: Throughput measurement

Tracks how many operations per second the calculator
can process.

Useful for CI regression detection.
------------------------------------------------------------
*/
static void Throughput(benchmark::State& state)
{
    Calculator calc;

    constexpr std::string_view expr = "1.+2.*3/4";

    for (auto _ : state)
    {
        RunCompute(calc, expr);
    }

    // Expression contains 4 arithmetic operations
    state.counters["Ops/s"] =
        benchmark::Counter(
            static_cast<double>(state.iterations() * 4),
            benchmark::Counter::kIsRate);
}

/*
------------------------------------------------------------
Benchmark registration
------------------------------------------------------------
*/

BENCHMARK(SimpleAddition);
BENCHMARK(ComplexExpression);
BENCHMARK(MixedOperators);

BENCHMARK(DeepNesting)
    ->Range(1, 512);

BENCHMARK(Throughput);

BENCHMARK(LongExpression)
    ->RangeMultiplier(2)
    ->Range(8, 4096)
    ->Complexity();

BENCHMARK_MAIN();

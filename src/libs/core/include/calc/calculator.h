#pragma once

#include "evaluator.h"
#include "parser.h"
#include "tokenizer.h"

namespace Calc
{
/**
 * @brief High-level calculator interface.
 *
 * Coordinates the full evaluation pipeline:
 *
 *  1. Tokenization   : raw string → sequence of Tokens
 *  2. Parsing        : infix Tokens → postfix (Reverse Polish Notation)
 *  3. Evaluation     : postfix Tokens → final numeric result
 *
 * This class provides a single public entry point (`compute`)
 * that performs the entire process and returns either a result
 * or an error.
 */
class Calculator
{
  public:
    /**
     * @brief Computes the result of a mathematical expression.
     *
     * Internally performs tokenization, parsing, and evaluation.
     * Any error produced by those stages is propagated to the caller.
     *
     * @param expr Input expression string.
     * @return Computed value or error string.
     */
    std::expected<double, std::string> compute(std::string_view expr) const;

  private:
    Tokenizer tokenizer;
    Parser parser;
    Evaluator evaluator;
};

} // namespace Calc

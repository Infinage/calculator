#pragma once

#include <optional>

namespace Calc
{

/**
 * @brief Represents a lexical token in a mathematical expression.
 *
 * Tokens are produced by the Tokenizer and later transformed by the Parser.
 *
 * A Token may represent:
 * - A numeric literal
 * - A binary operator (+, -, *, /)
 * - A unary operator (+, -)
 * - A parenthesis '(' or ')'
 *
 * Notes:
 * - The Tokenizer initially emits '+' and '-' as Add/Sub.
 * - The Parser later converts them to UPlus/UMinus when they appear
 *   in a unary context (e.g. "-3" or "5 * -2").
 *
 * If the token type is Num, the `number` field contains the parsed value.
 * For all other token types, `number` is std::nullopt.
 */
struct Token
{
    /**
     * @brief Enumerates all supported token types.
     */
    enum class TokenType
    {
        Num,   ///< Numeric literal
        Open,  ///< '('
        Close, ///< ')'

        Mul, ///< Binary '*'
        Div, ///< Binary '/'

        Add, ///< Binary '+'
        Sub, ///< Binary '-'

        UPlus,  ///< Unary '+'
        UMinus, ///< Unary '-'
    } type;

    /// Holds numeric value if type == Num.
    std::optional<double> number{std::nullopt};
};

} // namespace Calc

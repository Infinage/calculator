#include "calc/evaluator.h"
#include <stack>

std::expected<double, std::string>
Calc::Evaluator::eval(std::span<Token> postfix) const
{
    using TT = Token::TokenType;

    std::stack<double> stack;
    for (const auto &token : postfix)
    {
        if (token.type == TT::Num)
        {
            if (!token.number) return std::unexpected{"Invalid numeric token"};
            stack.push(token.number.value());
        }
        else if (token.type == TT::UMinus || token.type == TT::UPlus)
        {
            if (stack.empty())
                return std::unexpected{
                    "Stack underflow: Missing operand for unary operator"};
            double val = stack.top();
            stack.pop();
            stack.push(token.type == TT::UMinus ? -val : val);
        }
        else
        {
            if (stack.size() < 2)
                return std::unexpected{
                    "Stack underflow: Missing operands for binary operator"};

            auto v2 = stack.top();
            stack.pop();
            auto v1 = stack.top();
            stack.pop();
            switch (token.type)
            {
            case TT::Add:
                stack.push(v1 + v2);
                break;
            case TT::Sub:
                stack.push(v1 - v2);
                break;
            case TT::Mul:
                stack.push(v1 * v2);
                break;
            case TT::Div:
                if (v2 == 0) return std::unexpected{"Divide by zero"};
                stack.push(v1 / v2);
                break;
            default:
                return std::unexpected{
                    "Encountered unexpected token type in evaluator"};
            }
        }
    }

    if (stack.size() != 1)
        return std::unexpected{"Invalid expression: stack ended with " +
                               std::to_string(stack.size()) + " elements"};

    return stack.top();
}

#include "calc/parser.h"
#include <stack>

using TT = Calc::Token::TokenType;

namespace
{
bool isOperator(TT type)
{
    return type == TT::UPlus || type == TT::UMinus || type == TT::Add ||
           type == TT::Sub || type == TT::Mul || type == TT::Div;
}
} // namespace

std::expected<std::vector<Calc::Token>, std::string>
Calc::Parser::parse(std::span<Token> tokens) const
{
    // Tracks the last token type encountered in the input stream.
    // Initialized to Open so that a leading '+' or '-' is treated as unary.
    auto lastSeen = TT::Open;

    std::vector<Token> result; // Output queue (postfix expression)
    std::stack<Token> stack;   // Operator stack used by Shunting Yard

    for (const auto &token : tokens)
    {
        // Numbers go directly to the output queue.
        if (token.type == TT::Num)
        {
            result.push_back(token);
        }

        // Closing parenthesis triggers unwinding of the operator stack
        // until the matching '(' is found.
        else if (token.type == TT::Close)
        {
            bool matched = false;

            while (!stack.empty() && !matched)
            {
                auto curr = stack.top();
                stack.pop();

                matched = curr.type == TT::Open;

                if (!matched) result.push_back(curr);
            }

            // No matching '(' found → parentheses are imbalanced.
            if (!matched) return std::unexpected{"Imbalanced parentheses"};
        }

        // All remaining tokens are operators or '('.
        else
        {
            Token effectiveToken = token;

            // Determine if '+' or '-' should be interpreted as unary.
            // A unary operator occurs when:
            //   - the expression begins
            //   - the previous token was another operator
            //   - the previous token was '('
            bool isUnary = (lastSeen == TT::Open || isOperator(lastSeen));

            if (isUnary && (token.type == TT::Add || token.type == TT::Sub))
            {
                effectiveToken.type =
                    (token.type == TT::Add) ? TT::UPlus : TT::UMinus;
            }

            // Apply Shunting Yard precedence and associativity rules.
            // While the operator stack contains an operator with higher
            // precedence (or equal precedence for left-associative ops),
            // pop it to the output queue.
            while (!stack.empty() && stack.top().type != TT::Open)
            {
                int p1 = oPriority.at(effectiveToken.type); // current operator
                int p2 = oPriority.at(stack.top().type);    // stack operator

                // Unary operators are right-associative.
                bool isRightAssoc = (effectiveToken.type == TT::UMinus ||
                                     effectiveToken.type == TT::UPlus);

                // For right-associative operators:
                //   pop while precedence(top) > precedence(current)
                //
                // For left-associative operators:
                //   pop while precedence(top) >= precedence(current)
                if ((isRightAssoc && p1 < p2) || (!isRightAssoc && p1 <= p2))
                {
                    result.push_back(stack.top());
                    stack.pop();
                }
                else
                {
                    break;
                }
            }

            // Push the current operator onto the stack.
            stack.push(effectiveToken);
        }

        // Update context for the next token.
        lastSeen = token.type;
    }

    // Drain the remaining operators from the stack.
    while (!stack.empty())
    {
        result.push_back(stack.top());
        stack.pop();

        // Any remaining '(' means parentheses were imbalanced.
        if (result.back().type == TT::Open)
            return std::unexpected{"Imbalanced parentheses"};
    }

    return result;
}

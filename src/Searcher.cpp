//
// Created by leoric on 25-2-1.
//

#include "Searcher.h"

namespace Jimu {
    std::set<std::string> BooleanSearch::search(const std::string &query) {
        auto tokens = tokenize(query);
        auto postfix = shunting_yard(tokens);
        return evaluate(postfix);
    }

    std::vector<std::string> BooleanSearch::tokenize(const std::string &query) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream iss(query);

        while (iss >> token) {
            if (operators_.count(token)) {
                std::transform(token.begin(), token.end(), token.begin(), ::toupper);
                tokens.push_back(token);
            } else if (token == "(" || token == ")") {
                tokens.push_back(token);
            } else {
                TextProcessor processor;
                auto processed = processor.process(token);
                if (!processed.empty()) {
                    tokens.push_back(processed[0]);
                }
            }
        }
        return tokens;
    }

    std::vector<std::string> BooleanSearch::shunting_yard(const std::vector<std::string> &tokens) {
        std::vector<std::string> output;
        std::stack<std::string> op_stack;

        for (const auto &token: tokens) {
            if (token == "(") {
                op_stack.push(token);
            } else if (token == ")") {
                while (!op_stack.empty() && op_stack.top() != "(") {
                    output.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.pop();
            } else if (operators_.count(token)) {
                while (!op_stack.empty() && op_stack.top() != "(") {
                    const auto &top_op = op_stack.top();
                    if (operators_.at(top_op).precedence >= operators_.at(token).precedence) {
                        output.push_back(top_op);
                        op_stack.pop();
                    } else {
                        break;
                    }
                }
                op_stack.push(token);
            } else {
                output.push_back(token);
            }
        }

        while (!op_stack.empty()) {
            output.push_back(op_stack.top());
            op_stack.pop();
        }

        return output;
    }

    std::set<std::string> BooleanSearch::evaluate(const std::vector<std::string> &postfix) {
        std::stack<std::set<std::string> > stack;

        for (const auto &token: postfix) {
            if (operators_.count(token)) {
                if (operators_.at(token).is_unary) {
                    auto operand = stack.top();
                    stack.pop();
                    stack.push(not_operation(operand));
                } else {
                    auto rhs = stack.top();
                    stack.pop();
                    auto lhs = stack.top();
                    stack.pop();
                    if (token == "AND") {
                        stack.push(intersection(lhs, rhs));
                    } else {
                        stack.push(union_(lhs, rhs));
                    }
                }
            } else {
                stack.push(get_docs(token));
            }
        }

        return stack.empty() ? std::set<std::string>{} : stack.top();
    }

    std::set<std::string> BooleanSearch::get_docs(const std::string &term) {
        return index_.count(term) ? index_.at(term) : std::set<std::string>{};
    }

    std::set<std::string> BooleanSearch::not_operation(const std::set<std::string> &docs) {
        std::set<std::string> result;
        std::set_difference(all_docs_.begin(), all_docs_.end(),
                            docs.begin(), docs.end(),
                            std::inserter(result, result.begin()));
        return result;
    }

    std::set<std::string> BooleanSearch::intersection(const std::set<std::string> &a, const std::set<std::string> &b) {
        std::set<std::string> result;
        std::set_intersection(a.begin(), a.end(),
                              b.begin(), b.end(),
                              std::inserter(result, result.begin()));
        return result;
    }

    std::set<std::string> BooleanSearch::union_(const std::set<std::string> &a, const std::set<std::string> &b) {
        std::set<std::string> result;
        std::set_union(a.begin(), a.end(),
                       b.begin(), b.end(),
                       std::inserter(result, result.begin()));
        return result;
    }
} // Jimu

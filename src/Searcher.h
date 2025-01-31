//
// Created by leoric on 25-2-1.
//

#ifndef SEARCH_H
#define SEARCH_H
#include <set>
#include <sstream>
#include <string>

#include "IndexBuilder.h"
#include "TextProcessor.h"

namespace Jimu {
    // 布尔搜索处理器
    class BooleanSearch {
    public:
        BooleanSearch(const IndexBuilder &index)
            : index_(index.index()), all_docs_(index.all_docs()) {
        }

        std::set<std::string> search(const std::string &query);

    private:
        const std::map<std::string, std::set<std::string> > &index_;
        const std::set<std::string> &all_docs_;

        struct Operator {
            int precedence;
            bool is_unary;
        };

        const std::map<std::string, Operator> operators_ = {
            {"NOT", {3, true}},
            {"AND", {2, false}},
            {"OR", {1, false}}
        };

        std::vector<std::string> tokenize(const std::string &query);

        std::vector<std::string> shunting_yard(const std::vector<std::string> &tokens);

        std::set<std::string> evaluate(const std::vector<std::string> &postfix);

        std::set<std::string> get_docs(const std::string &term);

        std::set<std::string> not_operation(const std::set<std::string> &docs);

        std::set<std::string> intersection(const std::set<std::string> &a,
                                           const std::set<std::string> &b);

        std::set<std::string> union_(const std::set<std::string> &a,
                                     const std::set<std::string> &b);
    };
} // Jimu

#endif //SEARCH_H

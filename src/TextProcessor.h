//
// Created by leoric on 25-2-1.
//

#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H
#include <sstream>
#include <unordered_set>
#include <vector>


namespace Jimu {
    // 增强文本处理器
    class TextProcessor {
    public:
        TextProcessor() {
            stopwords_ = {
                "the", "a", "an", "and", "or", "of", "to", "in", "for", "is",
                "on", "that", "by", "this", "with", "it", "as", "be", "are"
            };
        }

        std::vector<std::string> process(const std::string &text);

    private:
        std::unordered_set<std::string> stopwords_;

        std::string normalize(std::string text);

        std::vector<std::string> tokenize(const std::string &text);

        std::string stem(const std::string &word);
    };
} // Jimu

#endif //TEXTPROCESSOR_H

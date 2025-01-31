//
// Created by leoric on 25-2-1.
//

#include "TextProcessor.h"

namespace Jimu {
    std::vector<std::string> TextProcessor::process(const std::string &text) {
        std::string processed = normalize(text);
        return tokenize(processed);
    }

    std::string TextProcessor::normalize(std::string text) {
        std::transform(text.begin(), text.end(), text.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        std::string result;
        bool last_was_space = false;
        for (char c: text) {
            if (std::isalnum(c) || c == '\'' || c == '-') {
                result += c;
                last_was_space = false;
            } else if (!last_was_space) {
                result += ' ';
                last_was_space = true;
            }
        }
        return result;
    }

    std::vector<std::string> TextProcessor::tokenize(const std::string &text) {
        std::vector<std::string> tokens;
        std::istringstream iss(text);
        std::string token;

        while (iss >> token) {
            token = stem(token);
            if (token.length() > 1 && !stopwords_.count(token)) {
                tokens.push_back(token);
            }
        }
        return tokens;
    }

    std::string TextProcessor::stem(const std::string &word) {
        if (word.length() > 3 && word.back() == 's') {
            return word.substr(0, word.length() - 1);
        }
        return word;
    }
} // Jimu

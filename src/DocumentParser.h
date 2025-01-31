//
// Created by leoric on 25-2-1.
//

#ifndef DOCUMENTPARSER_H
#define DOCUMENTPARSER_H
#include <fstream>
#include <string>
#include <vector>

#include "TextProcessor.h"
namespace fs = std::filesystem;

namespace Jimu {
    // 元数据存储结构
    struct DocumentMeta {
        std::string id;
        std::string url;
        std::string title;
        std::string date;
        std::vector<std::string> links;
    };

    // 文档解析器
    class DocumentParser {
    public:
        struct ParseResult {
            DocumentMeta meta;
            std::vector<std::string> tokens;
        };

        ParseResult parse(const fs::path &file_path);

    private:
        TextProcessor processor_;

        void parse_metadata(const std::string &content, DocumentMeta &meta);

        std::string extract_content(const std::string &content);
    };
} // jimu

#endif //DOCUMENTPARSER_H

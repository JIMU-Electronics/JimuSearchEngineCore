//
// Created by leoric on 25-2-1.
//

#include "DocumentParser.h"

namespace Jimu {
    DocumentParser::ParseResult DocumentParser::parse(const fs::path &file_path) {
        ParseResult result;
        std::ifstream file(file_path);
        std::stringstream buffer;
        buffer << file.rdbuf();

        parse_metadata(buffer.str(), result.meta);
        result.tokens = processor_.process(extract_content(buffer.str()));
        result.meta.id = file_path.filename().string();
        return result;
    }

    void DocumentParser::parse_metadata(const std::string &content, DocumentMeta &meta) {
        std::istringstream iss(content);
        std::string line;
        bool in_header = true;

        while (std::getline(iss, line)) {
            if (line.empty()) {
                in_header = false;
                continue;
            }

            if (in_header) {
                if (line.starts_with("URL: ")) {
                    meta.url = line.substr(5);
                } else if (line.starts_with("Title: ")) {
                    meta.title = line.substr(7);
                } else if (line.starts_with("Date: ")) {
                    meta.date = line.substr(6);
                }
            } else {
                if (line.starts_with("- ")) {
                    meta.links.push_back(line.substr(2));
                }
            }
        }
    }

    std::string DocumentParser::extract_content(const std::string &content) {
        std::istringstream iss(content);
        std::string line;
        std::string body;
        bool in_body = false;

        while (std::getline(iss, line)) {
            if (line.empty()) {
                in_body = true;
                continue;
            }
            if (in_body && !line.starts_with("Related Links:")) {
                body += line + " ";
            }
        }
        return body;
    }
} // jimu

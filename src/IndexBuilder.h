//
// Created by leoric on 25-2-1.
//

#ifndef INDEXBUILDER_H
#define INDEXBUILDER_H
#include <iostream>
#include <map>
#include <set>

#include "DocumentParser.h"

namespace Jimu {
    // 索引构建器
    class IndexBuilder {
    public:
        void build(const fs::path &dataset_path);

        const auto &index() const { return index_; }
        const auto &metadata() const { return metadata_; }
        const auto &all_docs() const { return all_docs_; }

    private:
        std::map<std::string, std::set<std::string> > index_;
        std::map<std::string, DocumentMeta> metadata_;
        std::set<std::string> all_docs_;

        void add_to_index(const DocumentParser::ParseResult &result);
    };
} // Jimu

#endif //INDEXBUILDER_H

//
// Created by leoric on 25-2-1.
//

#include "IndexBuilder.h"

namespace Jimu {
    void IndexBuilder::build(const fs::path &dataset_path) {
        DocumentParser parser;
        size_t count = 0;

        for (const auto &entry: fs::recursive_directory_iterator(dataset_path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".txt") {
                auto result = parser.parse(entry.path());
                add_to_index(result);
                metadata_[result.meta.id] = result.meta;
                all_docs_.insert(result.meta.id);
                if (++count % 10000 == 0) {
                    std::cout << "Processed " << count << " documents\n";
                }
            }
        }
    }

    void IndexBuilder::add_to_index(const DocumentParser::ParseResult &result) {
        for (const auto &token: result.tokens) {
            index_[token].insert(result.meta.id);
        }
    }
} // Jimu

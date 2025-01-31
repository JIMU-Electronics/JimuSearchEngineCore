#include <iostream>
#include <string>
#include <map>
#include <set>
#include <filesystem>

#include "Searcher.h"
namespace fs = std::filesystem;

// 主程序
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <dataset_path>\n";
        return 1;
    }

    fs::path dataset_path(argv[1]);

    // 构建索引
    Jimu::IndexBuilder indexer;
    std::cout << "Building index...\n";
    indexer.build(dataset_path);
    std::cout << "Index built with " << indexer.index().size()
            << " unique terms\n";

    // 初始化搜索
    Jimu::BooleanSearch searcher(indexer);

    // 搜索交互
    std::string query;
    while (true) {
        std::cout << "\nEnter boolean query (AND/OR/NOT supported) or 'quit':\n> ";
        std::getline(std::cin, query);

        if (query == "quit") break;

        auto results = searcher.search(query);

        std::cout << "Found " << results.size() << " documents:\n";
        for (const auto &doc_id: results) {
            const auto &meta = indexer.metadata().at(doc_id);
            std::cout << "[" << doc_id << "] " << meta.title
                    << "\n  URL: " << meta.url << "\n";
        }
    }

    return 0;
}

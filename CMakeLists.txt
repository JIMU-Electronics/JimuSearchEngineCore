cmake_minimum_required(VERSION 3.30)
project(JimuSearchEngineCore)

set(CMAKE_CXX_STANDARD 20)

include_directories(src/*)

add_executable(JimuSearchEngineCore
        src/main.cpp
        src/TextProcessor.cpp
        src/DocumentParser.cpp
        src/IndexBuilder.cpp
        src/Searcher.cpp
)

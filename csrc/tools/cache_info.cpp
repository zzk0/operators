#include <iostream>
#include <fstream>
#include <memory>
#include <string>

#include "tabulate/markdown_exporter.hpp"
#include "tabulate/table.hpp"

using namespace std;

#define CACHE_INFO_PATH "/sys/devices/system/cpu/cpu0/cache/"
#define CACHE_INDEX "index"
#define CACHE_LEVEL "level"
#define CACHE_TYPE "type"
#define CACHE_SIZE "size"
#define NUMBER_OF_SETS "number_of_sets"
#define WAYS_OF_ASSOCIATIVIVITY "ways_of_associativity"
#define COHERENCY_LINE_SIZE "coherency_line_size"


std::string Read(const std::string& filename) {
    std::unique_ptr<std::istream> fin = std::make_unique<std::ifstream>(filename);
    std::string res;
    *fin >> res;
    return res;
}

int main() {
    tabulate::Table table;
    table.add_row({"Level", "Type", "Size", "NumberOfSets", "WaysOfAssociativity", "LineSize"});
    for (int i = 0; i < 4; ++i) {
        std::string path = std::string(CACHE_INFO_PATH) + CACHE_INDEX + std::to_string(i) + "/";
        table.add_row({
            Read(path + CACHE_LEVEL),
            Read(path + CACHE_TYPE),
            Read(path + CACHE_SIZE),
            Read(path + NUMBER_OF_SETS),
            Read(path + WAYS_OF_ASSOCIATIVIVITY),
            Read(path + COHERENCY_LINE_SIZE),
        });
    }

    tabulate::MarkdownExporter exporter;
    const auto& markdown = exporter.dump(table);
    std::cout << markdown << std::endl;
    return 0;
}

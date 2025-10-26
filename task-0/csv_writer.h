#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>

class CSVWriter {
public:
    CSVWriter();
    static bool writeCSV(const std::string& filename, 
                        const std::vector<std::pair<std::string, int>>& sortedWords,
                        int totalWords);
};

#endif
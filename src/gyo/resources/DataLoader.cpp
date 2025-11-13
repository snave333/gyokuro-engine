
#include <gyo/resources/DataLoader.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace gyo {

CSVData DataLoader::LoadCSV(const char* filePath) {
    CSVData data = {};

    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file " << filePath << std::endl;
        return data;
    }

    std::string line;

    // read each line from the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        std::vector<std::string> row;

        // parse each line using stringstream and the comma delimiter
        while (std::getline(ss, field, ',')) {
            row.push_back(field);
        }
        data.push_back(row);
    }

    file.close();
    
    return data;
}

} // namespace gyo

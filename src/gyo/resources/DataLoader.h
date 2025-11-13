#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include <vector>

namespace gyo {

typedef std::vector<std::vector<std::string>> CSVData;

class DataLoader {
public:
    static CSVData LoadCSV(const char* filePath);
};
  
} // namespace gyo

#endif // DATA_LOADER_H

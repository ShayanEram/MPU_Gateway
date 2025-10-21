#ifndef INC_DATAMANAGER_HPP
#define INC_DATAMANAGER_HPP

#include <vector>

class DataManager 
{
public:
    DataManager() = default;
    ~DataManager() = default;

    void DataManager::ingest(void);
    void DataManager::filter() const;
    std::vector<int> DataManager::batch(std::size_t n) const;

private:

};

#endif // INC_DATAMANAGER_HPP
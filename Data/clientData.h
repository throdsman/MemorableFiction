#pragma once

#include <vector>
#include <cstring>
#include <ostream>

#ifndef CLIENTDATA
#define CLIENTDATA

// Client Data, use to hold file indexes onto the HashMap(key: DNI, value: clientData)
struct clientData
{
    clientData()
    {
    }

    clientData(int minArchives, std::string n) : name(n)
    {
        this->fileList.reserve(minArchives);
    }

    void addFile(int indx)
    {
        this->fileList.push_back(indx);
    }

    const std::string& getName()
    {
        return name;
    }

    const std::vector<int> &getFl()
    {
        return this->fileList;
    }

    std::ostream &operator<<(std::ostream &os)
    {
        const int cant = this->fileList.size();
        for (size_t i{0u}; i < cant; i++)
        {
            os << std::endl
               << "Data: " << this->fileList[i] << std::endl;
        }

        return os;
    }

private:
    std::vector<int> fileList;
    std::string name;
};

#endif
#include <vector>
#include <cstring>
#include <ostream>

struct clientData
{
    clientData()
    {
    }

    clientData(int minArchives)
    {
        this->fileExtension.reserve(minArchives);
        this->fileList.reserve(minArchives);
        this->fileNames.reserve(minArchives);
    }

    void addFile(int name, const char *ext, const char *orinalName)
    {
        this->fileList.push_back(name);
        char dest[4];
        std::strcpy(dest, ext);
        this->fileExtension.push_back(dest);
        this->fileNames.push_back(orinalName);
    }

    const std::vector<int> &getFl()
    {
        return this->fileList;
    }

    const std::vector<const char *> getFileExt()
    {
        return this->fileExtension;
    }

    const std::vector<const char *> getFileNames()
    {
        return this->fileNames;
    }

    std::ostream &operator<<(std::ostream &os)
    {
        const int cant = this->fileNames.size();
        for (size_t i{0u}; i < cant; i++)
        {
            os << std::endl
               << "Data: " << this->fileList[i] << " " << this->fileNames[i] << " " << this->fileExtension[i] << std::endl;
        }

        return os;
    }

private:
    std::vector<int> fileList;
    std::vector<const char *> fileExtension;
    std::vector<const char *> fileNames;
};
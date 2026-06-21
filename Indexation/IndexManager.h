#pragma once

#ifndef INDEXMANAGER
#define INDEXMANAGER

#include <filesystem>
#include <fstream>
#include <ostream>
#include <format>
#include "../Data/PathsNDefines.h"
#include "../Data/ArchivoMultimedia.h"
#include "../FileHelper/FileHelper.h"
#include "../Data/clientData.h"
#include "../Data/DataEnums.h"

using namespace FileHelper;

class IndexManager
{
public:

    IndexManager()
    {

    }

    // Write on Info file, Relevant Data
    void createPatientFiles(std::vector<std::pair<int, clientData>>& outClients)
    {
        std::vector<std::string> names;
        {
            std::ifstream namesFiles(TEMPLATE "/" "NameList.txt");

            std::string line = "";
            while (getline(namesFiles, line))
            {
                names.push_back(line);
            }

            namesFiles.close();
        }
        
        this->archivoList.reserve(names.size());

        int counter = 1;
        for (const auto& name: names)
        {
            clientData cData(5);
            const long cDNI = (std::rand() % 29999999) + 70000000;
            auto fileList = typesByNumber(cDNI);
            for (const auto& f: fileList)
            {
                ArchivoMultimedia arch;
                arch.nombre = name;
                arch.dni = cDNI;
                arch.paciente = name;
                arch.c = name[0];
                arch.fecha = std::to_string(std::rand() % 24);
                createFile(arch, counter, f);
                this->archivoList.push_back(arch);
                cData.addFile(counter);
                counter += 1;
            }
            outClients.push_back(std::pair(cDNI, cData));
        }

        std::cout << "Files ready!" << std::endl;
    }

    //Fill list with files in path
    void readDirectory(const std::filesystem::path& inPath)
    {
        std::vector<std::filesystem::directory_entry> entries;
        FileHelper::getAllEntriesInPath(inPath, entries);

        this->archivoList.reserve(entries.size());
        for (const auto& entry: entries)
        {
            ArchivoMultimedia am;
            auto& path = entry.path();
            am.nombre = path.stem().generic_string();
            am.c = 'c';
            am.tipo = path.extension().generic_string();
            am.ruta = path.generic_string();
            am.tamano = entry.file_size();
            int a = std::rand()%24;
            am.fecha = std::printf("%i", a);

            this->archivoList.push_back(am);
        }
    }

    void addArchivo(ArchivoMultimedia a, int& indx)
    {
        indx = archivoList.size();
        archivoList.push_back(a);
    }

    ArchivoMultimedia& get(int indx)
    {
        return this->archivoList[indx];
    }
    
    const ArchivoMultimedia& get(int indx) const
    {
        return this->archivoList[indx];
    }

    int getCount()
    {
        return archivoList.size();
    }

    std::vector<int> checkIntegrity()
    {
        std::vector<int> ret;
        for (size_t i{0u}; i < archivoList.size(); i++)
        {
            if (!FileHelper::do_file_exist(archivoList[i].ruta))
            {
                ret.push_back(i);
            }
        }
        return ret;
    }

    void eliminateIndex(int indx)
    {
        archivoList.erase(archivoList.begin() + indx);
    }

private:
    void createFile(ArchivoMultimedia& arch, int num, FileType type)
    {
        std::filesystem::path dataPath(getPathByType(type));
        std::string aux =  getFileNameInZeros(num);
        std::filesystem::path outFile(PATH "/" + aux + ".abc");
        FileHelper::encriptFile(dataPath, outFile, arch, arch.nombre + dataPath.stem().generic_string());
    }

    std::filesystem::path getPathByType(FileType type)
    {
        switch (type)
        {
            case FileType::jpg:
            {
                return (TEMPLATE "/" "scan.jpg");
            }
            case FileType::png:
            {
                return (TEMPLATE "/" "img.png");
            }
            case FileType::mp3:
            {
                return (TEMPLATE "/" "aud.mp3");
            }
            default:
            case FileType::txt:
            {
                return (TEMPLATE "/" "info.txt");
            }
            case FileType::mp4:
            {
                return (TEMPLATE "/" "vid.mp4");
            }
        }
    }

    std::vector<FileType> typesByNumber(int number)
    {
        std::vector<FileType> ret;
        ret.reserve(5);
        ret.push_back(FileType::txt);

        if (number & 1)
        {
            ret.push_back(FileType::jpg);
        }

        if (number & 2)
        {
            ret.push_back(FileType::mp3);
        }

        if (number & 4)
        {
            ret.push_back(FileType::mp4);
        }

        if (number & 8)
        {
            ret.push_back(FileType::png);
        }

        return ret;
    }

private:
    std::vector<ArchivoMultimedia> archivoList;
};

#endif
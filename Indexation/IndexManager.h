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

using namespace FileHelper;

class IndexManager
{
public:

    IndexManager()
    {

    }

    // Test Function (NEED REFACTORING)
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
            std::filesystem::path dataPath(TEMPLATE "/" "info.txt");
            std::string aux =  getFileNameInZeros(counter);
            std::filesystem::path outFile(PATH "/" + aux + ".abc");

            ArchivoMultimedia arch;
            arch.paciente = name;
            arch.fecha = std::to_string(std::rand() % 24);
            std::string fileName = name + "Info";
            FileHelper::encriptFile(dataPath, outFile, arch, fileName);

            const int cDni = (std::rand() % 29999999) + 70000000;
            arch.dni = cDni;

            this->archivoList.push_back(arch);

            std::ofstream oFile(outFile, std::ios::binary);
            std::string basicInfo = "Paciente: " + name;
            oFile.write(basicInfo.c_str(), basicInfo.size());
            oFile.close();
            oFile.clear();

            clientData cData(2);
            cData.addFile(counter-1);
            counter += 1;

            if (counter%5 == 0)
            {
                std::filesystem::path templateImage(TEMPLATE "/" "scan.jpg");
                std::string imageaux = FileHelper::getFileNameInZeros(counter);
                std::filesystem::path imagePath(PATH "/" + imageaux + ".abc");

                ArchivoMultimedia imgData;
                imgData.paciente = name;
                imgData.fecha = std::to_string(std::rand() % 24);
                FileHelper::encriptFile(templateImage, imagePath, imgData, name + "Scan");
                this->archivoList.push_back(imgData);
                cData.addFile(counter-1);
                counter += 1;
            }

            outClients.push_back(std::pair(cDni, cData));
        }
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
            am.c = 'c';
            am.nombre = path.stem().generic_string();
            am.tipo = path.extension().generic_string();
            am.ruta = path.generic_string();
            am.tamano = entry.file_size();
            int a = std::rand()%24;
            am.fecha = std::printf("%i", a);

            this->archivoList.push_back(am);
        }
    }

    ArchivoMultimedia& get(int indx)
    {
        return this->archivoList[indx];
    }
    
    const ArchivoMultimedia& get(int indx) const
    {
        return this->archivoList[indx];
    }

private:
    std::vector<ArchivoMultimedia> archivoList;
};

#endif
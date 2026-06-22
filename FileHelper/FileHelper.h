#pragma once

#ifndef FILEHELPER
#define FILEHELPER

#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <filesystem>
#include "../Data/ArchivoMultimedia.h"
#include "../Data/PathsNDefines.h"

class IndexManager;

using namespace std;

namespace FileHelper
{
    bool encriptFile(const std::filesystem::path &inFile, const std::filesystem::path &outFile, ArchivoMultimedia& outInfo, std::string auxName = "")
    {
        if (!std::filesystem::exists(inFile))
        {
            return false;
        }

        if (!std::filesystem::is_regular_file(inFile))
        {
            return false;
        }

        if (std::filesystem::is_empty(inFile))
        {
            //return false;
        }

        std::string extension = inFile.extension().string();
        std::string name = (auxName.size() <= 0) ? inFile.stem().string() : auxName;

        outInfo.tipo = extension;
        outInfo.nombre = name;
        outInfo.ruta = outFile.generic_string();
        outInfo.tamano = std::filesystem::file_size(inFile);
        
        std::ifstream sourceFile(inFile, std::ios::binary);
        std::ofstream targetFile(outFile, std::ios::binary);

        char buffer = ' ';
        while (sourceFile.get(buffer) || sourceFile.gcount() > 0)
        {
           targetFile.put(buffer + outInfo.c);
        }

        sourceFile.close();
        targetFile.close();

        return true;
    }

    static bool getAllFilePaths(const std::filesystem::path &inFolder, std::vector<std::filesystem::path>& outVec, int reserved = 0)
    {
        if (!std::filesystem::exists(inFolder) || !std::filesystem::is_directory(inFolder))
        {
            return false;
        }

        try
        {
            if (reserved == 0)
            {
                uint8_t count = 0;
                for (const auto &entry : std::filesystem::directory_iterator(inFolder))
                {
                    count+=1;
                }

                const size_t vecSize{count};
                outVec.reserve(vecSize);
            }

            // Read file by file
            for (auto &entry : std::filesystem::directory_iterator(inFolder))
            {
                outVec.push_back(entry.path());
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return true;
    }

     static bool getAllEntriesInPath(const std::filesystem::path &inFolder, std::vector<std::filesystem::directory_entry>& outVec, int reserved = 0)
    {
        if (!std::filesystem::exists(inFolder) || !std::filesystem::is_directory(inFolder))
        {
            return false;
        }

        try
        {
            if (reserved == 0)
            {
                uint8_t count = 0;
                for (const auto &entry : std::filesystem::directory_iterator(inFolder))
                {
                    count+=1;
                }

                const size_t vecSize{count};
                outVec.reserve(vecSize);
            }

            // Read file by file
            for (auto &entry : std::filesystem::directory_iterator(inFolder))
            {
                outVec.push_back(entry);
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return true;
    }

    // Remember always to close the file after trying to delete it
    static bool eliminateFile(const std::filesystem::path &filePath)
    {
        if (!std::filesystem::exists(filePath))
        {
            return false;
        }

        if (!std::filesystem::is_regular_file(filePath))
        {
            return false;
        }
        
        return std::filesystem::remove(filePath);
    }

    // using the metadata saved to recreate original archive, next update will use magic number
    static bool recreateOriginalFile(const ArchivoMultimedia& inData, ArchivoMultimedia& outData, std::string dirLocation)
    {
        if (!std::filesystem::exists(dirLocation))
        {
            return false;
        }

        if (std::filesystem::is_regular_file(inData.ruta))
        {
            std::filesystem::path outP = dirLocation + "//" + inData.nombre + inData.tipo;
            std::ifstream sourceFile(inData.ruta, std::ios::binary);
            std::ofstream targetFile(outP, std::ios::binary);
            outData.ruta = outP.generic_string();
            outData.nombre = inData.nombre;
            outData.tipo = inData.tipo;
            outData.tamano = inData.tamano;

            char buffer = ' ';
            while (sourceFile.get(buffer) || sourceFile.gcount() > 0) {
                targetFile.put(buffer - inData.c);
            }


            sourceFile.close();
            sourceFile.clear();

            targetFile.close();
            targetFile.clear();

            return true;
        }

        return false;
    }
 
    static int numOfFilesInPath(const std::filesystem::path& inPath)
    {
        if (!std::filesystem::is_directory(inPath))
        {
            return 0;
        }

        try
        {
            int count = 0;
            // Read file by file
            for (auto &entry : std::filesystem::directory_iterator(inPath))
            {
                count += 1;
            }
            return count;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return  0;
    }

    // Returns string fillup with zeroes and number entered, length:NUMZEROS - digits of n
    string getFileNameInZeros(int n)
    {
        std::string a;
        int aux = n;
        int digits = 0;
        while (aux > 0)
        {
            digits++;
            aux /= 10;
        }
        a.resize(NUMZEROS - digits, '0');
        a += std::to_string(n);
        return a;
    }

    string path_to_aux()
    {
        return AUX;
    }

    bool do_file_exist(std::string path)
    {
        return std::filesystem::exists(path);
    }
}

#endif
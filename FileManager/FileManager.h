#pragma once

#ifndef FILEMANAGER
#define FILEMANAGER

#include <fstream>
#include <ostream>
#include <vector>
#include <string>
#include <filesystem>

using namespace std;

namespace FileManager
{

    const uint32_t MAGIC_NUMBER = 0x5a495050; // ZIPP on hex

    struct ArchivoMultimedia 
    {
        string sha256;
        string nombre;
        string paciente;
        string dni;
        string tipo;
        string ruta;
        string fecha;
        long long tamano;

        ArchivoMultimedia() {}
        ArchivoMultimedia(string n, string t, string r, string f, string p)
            : nombre(n), tipo(t), ruta(r), fecha(f), paciente(p){}
        ArchivoMultimedia(string s, string n, string p, string d,
                        string t, string r, string f, long long tam)
            : sha256(s), nombre(n), paciente(p), dni(d),
            tipo(t), ruta(r), fecha(f), tamano(tam) {}
    };


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
        
        std::ifstream sourceFile(inFile, std::ios::binary);
        std::ofstream targetFile(outFile, std::ios::binary);

        const size_t bufferSize(4096);
        char *buffer = new char[bufferSize];
        while (sourceFile.read(buffer, bufferSize) || sourceFile.gcount() > 0)
        {
           targetFile.write(buffer, sourceFile.gcount());
        }

        //delete buffer;
        sourceFile.close();
        targetFile.close();

        return true;
    }
    bool decriptFile(const std::filesystem::path &inFile, const std::filesystem::path &outFile, ArchivoMultimedia& outInfo, std::string auxName = "")
    {
        return encriptFile(inFile, outFile, outInfo, auxName);
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
            
            const size_t bufferSize = 4096;
            std::vector<char> buffer(bufferSize);
            while (sourceFile.read(buffer.data(), bufferSize) || sourceFile.gcount() > 0) {
                targetFile.write(buffer.data(), sourceFile.gcount());
            }


            sourceFile.close();
            sourceFile.clear();

            targetFile.close();
            targetFile.clear();

            return true;
        }

        return false;
    }
};

#endif
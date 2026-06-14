#pragma once
#include <fstream>
#include <ostream>
#include <vector>
#include <filesystem>

#ifndef FILEMANAGER
#define FILEMANAGER

namespace FileManager
{

    bool encriptFile(const std::filesystem::path &inFile, const std::filesystem::path &outFile)
    {
        if (!std::filesystem::exists(inFile) || !std::filesystem::exists(outFile))
        {
            return false;
        }

        if (!std::filesystem::is_regular_file(inFile) || !std::filesystem::is_regular_file(outFile))
        {
            return false;
        }

        if (std::filesystem::is_empty(inFile))
        {
            return false;
        }

        std::ifstream sourceFile(inFile, std::ios::binary);
        std::ofstream targetFile(outFile, std::ios::binary);

        const size_t bufferSize(4096);
        char *buffer = new char[bufferSize];
        while (sourceFile.read(buffer, bufferSize) || sourceFile.gcount() > 0)
        {
            targetFile.write(buffer, sourceFile.gcount());
        }

        delete buffer;
        sourceFile.close();
        targetFile.close();

        return true;
    }
    bool decriptFile(const std::filesystem::path &inFile, const std::filesystem::path &outFile)
    {
        return encriptFile(inFile, outFile);
    }

    static char *getExtension(const char *bins)
    {
        return nullptr;
    }

    static char *getFileName(const char *bins)
    {
        return nullptr;
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
};

#endif
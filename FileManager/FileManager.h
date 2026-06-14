#pragma once
#include <fstream>
#include <ostream>
#include <vector>
#include <filesystem>

#ifndef FILEMANAGER
#define FILEMANAGER

namespace FileManager
{

    const uint32_t MAGIC_NUMBER = 0x5a495050; // ZIPP on hex

    struct fileMetaData
    {
        fileMetaData()
        {
            
        }
    };

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

        std::string extension = inFile.extension().string();
        std::string name = inFile.stem().string();

        std::ifstream sourceFile(inFile, std::ios::binary);
        std::ofstream targetFile(outFile, std::ios::binary);

       // 1. Write Magic Number (4 bytes)
        targetFile.write(reinterpret_cast<const char*>(&MAGIC_NUMBER), sizeof(MAGIC_NUMBER));

        // 2. Write Extension Length (1 byte) and Extension String
        uint8_t extLen = static_cast<uint8_t>(extension.size());
        targetFile.write(reinterpret_cast<const char*>(&extLen), sizeof(extLen));
        targetFile.write(extension.c_str(), extLen);

        // 3. Write Name Length (1 byte) and Name String
        uint8_t nameLen = static_cast<uint8_t>(name.size());
        targetFile.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
        targetFile.write(name.c_str(), nameLen);

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
                int count = 0;
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

    static class fileMetaData getMData()
    {
        return fileMetaData();
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

    static bool recreateOriginalFile(const std::filesystem::path& inPath, std::filesystem::path& outPath, std::string dirLocation)
    {
        if (!std::filesystem::exists(dirLocation))
        {
            return false;
        }

        if (std::filesystem::is_regular_file(inPath))
        {
            std::ifstream sourceFile(inPath, std::ios::binary);

            // 1. Read and verify Magic Number
            uint32_t readMagic = 0;
            sourceFile.read(reinterpret_cast<char*>(&readMagic), sizeof(readMagic));
            if (readMagic != MAGIC_NUMBER)
            {
                throw std::runtime_error("Invalid file format! Magic number matching failed.");
            }

            // 2. Read Extension
            uint8_t extLen = 0;
            sourceFile.read(reinterpret_cast<char*>(&extLen), sizeof(extLen));
            std::vector<char> extBuf(extLen);
            sourceFile.read(extBuf.data(), extLen);
            std::string extension(extBuf.begin(), extBuf.end());

            // 3. Read Name
            uint8_t nameLen = 0;
            sourceFile.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
            std::vector<char> nameBuf(nameLen);
            sourceFile.read(nameBuf.data(), nameLen);
            std::string stem(nameBuf.begin(), nameBuf.end());

            // 4. Dynamically construct the output path using std::filesystem
            std::filesystem::path restoredFilename = stem + extension;
            outPath = dirLocation / restoredFilename;

            // 5. Stream out the remaining payload data
            std::ofstream outFile(outPath, std::ios::binary);
            if (!outFile) throw std::runtime_error("Failed to create restored output file.");

            const size_t bufferSize = 4096;
            std::vector<char> buffer(bufferSize);
            while (sourceFile.read(buffer.data(), bufferSize) || sourceFile.gcount() > 0) {
                outFile.write(buffer.data(), sourceFile.gcount());
            }


            sourceFile.close();
            sourceFile.clear();

            outFile.close();
            outFile.clear();

            return true;
        }
        
        // search magic number

        // get extension and name

        // create on aux folder

        // all over again

        return false;
    }
};

#endif
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <filesystem>
#include "clientData.h"
#include "dataRelation.h"
#include "dataStructures/cuckooHashing.h"
#include "FileManager/FileManager.h"

using namespace std;

#define PATH "./dataFolder"
#define NUMZEROS 10

void arc()
{
    // creating files
    for (size_t i = 1; i <= 20; i++)
    {
        std::string a;
        int aux = i;
        int digits = 0;
        while (aux > 0)
        {
            digits++;
            aux /= 10;
        }
        a.resize(NUMZEROS - digits, '0');
        a += std::to_string(i);
        std::ofstream newFile{PATH "/" + a + ".abc"};
        newFile.close();
    }

    try
    {
        if (std::filesystem::exists(PATH) && std::filesystem::is_directory(PATH))
        {
            // Read file by file
            for (const auto &entry : std::filesystem::directory_iterator(PATH))
            {
                std::cout << entry.path().filename() << std::endl;
            }
        }
        else
        {
            std::cout << "no directory by that name, error opening the path" << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // writing on files
    std::ofstream myFile(PATH "/000001.abc");

    int num = 4455;
    for (int i = 0; i < 10; i++)
    {
        myFile << num << std::endl;
    }

    myFile.close();

    // reading off files
    std::ifstream readFile(PATH "/000001.abc", std::ios::binary | std::ios::in);

    std::string line = "";
    std::cout << "Reading" << std::endl;
    while (getline(readFile, line))
    {
        std::cout << line << std::endl;
    }

    readFile.close();

    cHash<clientData *> *ck = new cHash<clientData *>();

    for (std::size_t i{0u}; i < 100; i++)
    {
        int DNI = (std::rand() % 29999999) + 70000000;

        clientData *data = new clientData(5);
        const int cantFile = std::rand() % 5;
        for (size_t o{0u}; i < cantFile; o++)
        {
            char ext[4] = "jpg";
            const char *name = "   ";
            data->addFile(std::rand() % 21, ext, name);
        }

        std::pair<int, clientData *> clientPair;
        clientPair.first = DNI;
        clientPair.second = data;
        ck->insertar(clientPair);
    }

    ck->mostrarTablas();

    delete (ck);

    
}

int main()
{
    std::ifstream image(PATH "/laberinto.jpg", std::ios::binary);
    std::ofstream receiverWrite(PATH "/000000.abc", std::ios::binary);
    std::ifstream receiverRead(PATH "/000000.abc", std::ios::binary);
    std::ofstream created(PATH "/created.jpg", std::ios::binary);

    FileManager::encriptFile(PATH "/laberinto.jpg", PATH "/000000.abc");
    FileManager::decriptFile(PATH "/000000.abc", PATH "/created.jpg");

    image.close();
    receiverWrite.close();
    receiverRead.close();
    created.close();

    return 0;
}
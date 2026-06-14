#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <filesystem>
#include "clientData.h"
#include "dataRelation.h"
#include "dataStructures/cuckooHashing.h"
#include "FileManager/FileManager.h"
#include "FileManager/GestorMultimedia.h"

using namespace std;

#define PATH "./dataFolder"
#define AUX "./auxFolder"
#define TEMPLATE "./Template"
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
            data->addFile(std::rand() % 21);
        }

        std::pair<int, clientData *> clientPair;
        clientPair.first = DNI;
        clientPair.second = data;
        ck->insertar(clientPair);
    }

    ck->mostrarTablas();

    delete (ck);
}

std::string getCurrentFileName(int n)
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

void setupPatientData(GestorMultimedia* gm)
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
    
    int counter = 1;
    for (const auto& name: names)
    {
        std::filesystem::path dataPath(TEMPLATE "/" "info.txt");
        std::string aux =  getCurrentFileName(counter);
        std::filesystem::path outFile(PATH "/" + aux + ".abc");

        Archivo arch;
        arch.paciente = name;
        arch.fecha = std::to_string(std::rand() % 24);
        std::string fileName = name + "Info";
        FileManager::encriptFile(dataPath, outFile, arch, fileName);

        gm->indexarArchivo(arch);

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
            std::string imageaux = getCurrentFileName(counter);
            std::filesystem::path imagePath(PATH "/" + imageaux + ".abc");

            Archivo imgData;
            imgData.paciente = name;
            imgData.fecha = std::to_string(std::rand() % 24);
            FileManager::encriptFile(templateImage, imagePath, imgData, name + "Scan");
            gm->indexarArchivo(imgData);
            cData.addFile(counter-1);
            counter += 1;
        }

        gm->guardarDataCliente((std::rand() % 29999999) + 70000000, cData);
    }
}

int main()
{
    GestorMultimedia gm;
    setupPatientData(&gm);

    bool flag = false;
    int dni = 0;
    while (!flag)
    {
        std::cout << "Ingresa dni a Buscar: "; 
        std::cin >> dni;

        flag = gm.searchDNIFiles(dni);
    }

    char currentLetter = ' ';
    std::string currentPreFix = "";
    while (currentLetter != '0')
    {
        std::cin >> currentLetter;
        currentPreFix += currentLetter;
        std::cout << "Busqueda: " << currentPreFix << std::endl;
        gm.autocompletar(currentPreFix);
    }

    return 0;
}
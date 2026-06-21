#pragma once

#ifndef FILEMANAGER
#define FILEMANAGER

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <map>
#include "../Data/ArchivoMultimedia.h"
#include "../Data/clientData.h"
#include "../Data/cuckooHashing.h"
#include "../Data/skewHeap.h"
#include "../Data/trieStruct.h"
#include "../Data/unionFind.h"
#include "../Data/DataEnums.h"
#include "../Data/PathsNDefines.h"
#include "../FileHelper/FileHelper.h"
#include "../Indexation/IndexManager.h"

class IndexManager;

using namespace std;

class FileManager 
{
private:
    IndexManager* indexManager; // indices de archivos
    TrieBusqueda indiceTrie;   // Motor de prefijos
    std::map<std::string, int> indiceHash;      // Motor de búsqueda exacta
    skewHeap<ArchivoMultimedia> heapFecha; // Motor de búsqueda por fechas
    skewHeap<ArchivoMultimedia> heapSize; // Motor de búsqueda por tamaño
    cHash<clientData> dniStorage; // Información relevante del cliente
    unionFind ufTypes; // Motor de búsqueda por tipo de archivo
    int currentDNISearched= 0; // Paciente actual en revisión
    const int Types;

public:

    FileManager(int maxTypes):Types(maxTypes)
    {
        indexManager = new IndexManager();
    }

// RF01 & RF02: Simulación de escaneo e indexación en RAM (RF03) + Simulación de volumenes de archivos
    void SearchFiles()
    {
        std::vector<std::pair<int, clientData>> outVec;
        indexManager->createPatientFiles(outVec);

        for (const auto& d: outVec)
        {
            guardarDataCliente(d.first, d.second);
        }
    }


    void guardarDataCliente(int dni, clientData d)
    {
        this->dniStorage.insertar({dni, d});
        std::cout << " - " << dni << std::endl;
    }

    void clearCurrentSearch()
    {
        this->currentDNISearched = 0;
        indiceTrie.clean();
        indiceHash.clear();
        ufTypes.clear(dniStorage.getSize(), Types); // complex crear, needs to expand
        heapFecha.empty();
        heapSize.empty();
    }

    void updateStructures(bool upscaling)
    {
        int upscaleValue = (upscaling) ? 1.2 : .8 ;
        dniStorage.reHash(dniStorage.getSize() * upscaleValue);

        int diff = (FileHelper::numOfFilesInPath(PATH)) - indexManager->getCount();
        if (diff < 0)
        {
            auto missingList = indexManager->checkIntegrity();
            for (const auto& m: missingList)
            {
                indexManager->eliminateIndex(m);
            }
        }

        auto aux = currentDNISearched;

        clearCurrentSearch();

        searchDNIFiles(aux);
    }

    bool searchDNIFiles(int DNI)
    {
        if (!this->dniStorage.buscar(DNI))
        {
            std::cout << printf("DNI: {}. Not on DB", DNI) << std::endl; 
            return false;
        }


        currentDNISearched = DNI;
        std::vector<int> indexes = dniStorage.getData(DNI).getFl(); // obtiene los indices de los files asociados del paciente
        ufTypes = unionFind(dniStorage.getSize(), Types);
        for (const auto& indx: indexes)
        {
            ArchivoMultimedia arch = indexManager->get(indx - 1);
            std::cout << indx << " : " << arch.nombre << std::endl;
            indiceHash[arch.nombre] = indx; // busqueda exacta
            indiceTrie.insertar(arch.nombre, indx); // parcial
            heapFecha.merge(new skewNode<ArchivoMultimedia>(std::stoi(arch.fecha), arch)); // fecha
            heapSize.merge(new skewNode<ArchivoMultimedia>(arch.tamano, arch)); // tamano
            agregarTipoArchivo(arch.tipo, indx + (Types));// tipo
        }

        return true;
    }

    void showDNI()
    {
        std::cout << "DNIs: " << std::endl;
        for (const auto& d: dniStorage.getKeys())
        {
            std::printf("- %i", d);
        }
        std::cout << "--------------------------------" << std::endl;
    }

    void agregarTipoArchivo(std::string tipo, int i)
    {
        int index = -1;
        if (tipo.compare(".jpg")) // 0 index on union find
        {
            index = 0;
        }
        else if (tipo.compare(".png"))
        {
            index = 1;
        }else if (tipo.compare(".mp3"))
        {
            index = 2;
        }else if (tipo.compare(".txt"))
        {
            index = 3;
        }else if (tipo.compare(".mp4"))
        {
            index = 4;
        }

        if (index == -1)
        {
            std::cout << "Error on adding type of archive" << std::endl;
            return;
        }

        ufTypes.unite(index, i);
    }

 // RF04: Búsqueda Exacta
    void buscarExacta(std::string nombre) 
    {
        if (currentDNISearched == 0)
        {
            return;
        }

        if (indiceHash.count(nombre)) {
            int i = indiceHash[nombre];
            cout << "Archivo Encontrado! Ruta: " << indexManager->get(i).ruta << endl;
        } else cout << "No hay coincidencia exacta." << endl;
    }
 

// RF05: Búsqueda por Prefijo usando el TRIE 
    void autocompletar(std::string prefijo) 
    { 
        if (currentDNISearched == 0)
        {
            std::cout << "\n Ingresar primer DNI!" << std::endl;
            return;
        }

        cout << "\n--- Resultados de búsqueda parcial para '" << prefijo << "' ---" << endl; 
        std::vector<int> resultados; 
        indiceTrie.buscarPrefijo(prefijo, resultados);
        
        for (const auto& res: resultados)
        {
            ImprimirArchivo(indexManager->get(res));
        }
    }

    void buscarPorNombre()
    {
        auto words = indiceTrie.obtenerPalabras();

        for (const auto& word: words)
        {
            std::cout << "- " << word << std::endl;
        }
    }

    // Busqueda por fecha usando heaps
    void busquedaPorFecha()
    {
        auto files = heapFecha.getList();

        for (const auto& f: files)
        {
            ImprimirArchivo(f.value);
        }
    }

    // Busqueda por tipo de archivo: usando union find
    void busquedaPorTipo(FileType type)
    {
        std::vector<int> archives;
        switch (type)
        {
            case FileType::jpg:
            {
                archives = ufTypes.get(0);
                break;
            }
            case FileType::png:
            {
                archives = ufTypes.get(1);
                break;
            }
            case FileType::mp3:
            {
                archives = ufTypes.get(2);
                break;
            }
            case FileType::txt:
            {
                archives = ufTypes.get(3);
                break;
            }
            case FileType::mp4:
            {
                archives = ufTypes.get(4);
                break;
            }
        }
        
        for (const auto& res: archives)
        {
            ImprimirArchivo(indexManager->get(res - (Types)));
        }
    }

    void busquedaPorSize()
    {
        auto files = heapSize.getList();

        for (const auto& f: files)
        {
            ImprimirArchivo(f.value);
        }
    }

    void ImprimirArchivo(ArchivoMultimedia a)
    {
        std::string show("Nombre: " + a.nombre + ", Tamano: " + std::to_string(a.tamano) + " bytes, Tipo: " + a.tipo + ", Ruta: " + a.ruta);
        std::cout << show  << std::endl;
    }

};

#endif
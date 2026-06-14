#pragma once

#ifndef GM
#define GM

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <map>
#include "../clientData.h"
#include "../dataStructures/cuckooHashing.h"
#include "../dataStructures/heapStruct.h"
#include "../dataStructures/trieStruct.h"
#include "FileManager.h"

using namespace std;
#define Archivo FileManager::ArchivoMultimedia


class GestorMultimedia {
private:
    std::vector<Archivo> db;
    TrieBusqueda indiceTrie;   // Motor de prefijos
    std::map<std::string, int> indiceHash;      // Motor de búsqueda exacta
    HeapFecha heapFecha;
    cHash<clientData> dniStorage;
    int currentDNISearched= 0;

public:

    GestorMultimedia()
    {

    }

// RF01 & RF02: Simulación de escaneo e indexación en RAM (RF03) 
    void indexarArchivo(Archivo arc) { 
        db.push_back(arc);
    } 

    void guardarDataCliente(int dni, clientData d)
    {
        this->dniStorage.insertar({dni, d});
        std::cout << dni << std::endl;
    }

    void clearCurrentSearch()
    {
        this->currentDNISearched = 0;
    }

    bool searchDNIFiles(int DNI)
    {
        if (!this->dniStorage.buscar(DNI))
        {
            std::cout << printf("DNI: {}. Not on DB", DNI) << std::endl; 
            return false;
        }

        currentDNISearched = DNI;
        std::vector<int> indexes = dniStorage.getData(DNI).getFl();
        for (const auto& indx: indexes)
        {
            auto arch = db[indx];
            std::cout << indx << " : " << arch.nombre << std::endl;
            indiceHash;
            indiceHash[arch.nombre] = indx;
            indiceTrie.insertar(arch.nombre, indx);
            heapFecha.insertar(arch);
        }

        return true;
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
            cout << "[EXACTA] Encontrado: " << db[i].ruta << endl;
        } else cout << "No hay coincidencia exacta." << endl;
    }
 

// RF05: Búsqueda por Prefijo usando el TRIE 
    void autocompletar(std::string prefijo) 
    { 
        if (currentDNISearched == 0)
        {
            return;
        }

        cout << "\n--- Resultados de búsqueda parcial para '" << prefijo << "' ---" << endl; 
        std::vector<int> resultados = indiceTrie.buscarPrefijo(prefijo); 
        
        for (const auto& res: resultados)
        {
            heapFecha.insertar(db[res]);
        }

        auto ordered = heapFecha.ordenados();
        for (const auto& o: ordered)
        {
            std::cout << o.nombre << std::endl;
        }

        heapFecha.vacio();
    } 
};
#endif
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
//#include "../Data/PathsNDefines.h"
#include "../FileHelper/FileHelper.h"
#include "../Indexation/IndexManager.h"

class IndexManager;

using namespace std;

// Clase encargada de Manejas las busquedas, se apoya de File Helper para el manejo de carpetas
// Principalmente, se encarga de realizar los ordenamientos y busquedas que le solicita Logic Manager
// Adempas de tratar directamente con IndexManager,el cual contiene los indices en memoria de todos los archivos médicos
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
    pair<long, vector<ArchivoMultimedia>> filesCreated;

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

    // Guarda la data de un nuevo cliente
    void guardarDataCliente(int dni, clientData d)
    {
        this->dniStorage.insertar({dni, d});
        std::cout << " - " << dni << std::endl;
    }

    // Limpia la busqueda, regresa el estado de los data structures a uno default, donde carecen de datos ingresado. Se espera que la capa lógica pregunte ->
    // -> el dni del paciente para funcionar.
    void clearCurrentSearch()
    {
        this->currentDNISearched = 0;
        indiceTrie.clean();
        indiceHash.clear();
        ufTypes.clear(dniStorage.getSize(), Types); // complex crear, needs to expand
        heapFecha.empty();
        heapSize.empty();
    }

    // Actualiza las estructuras dependiendo si encontro un file que estaba faltando, y procede a retirarlo de la lista de archivos
    void updateStructures(bool upscaling)
    {
        int upscaleValue = (upscaling) ? 1.2 : .8 ;
        dniStorage.reHash(dniStorage.getSize() * upscaleValue);

        int diff = (FileHelper::numOfFilesInPath(PATH)) - indexManager->getCount();
        if (diff > 0)
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

    inline bool dni_exist(long dni)
    {
        return this->dniStorage.buscar(dni);
    }

    // Función principal que, actualiza los data structures con los archivos relacionados al dni ingresado
    bool searchDNIFiles(int DNI)
    {
        if (!dni_exist(DNI))
        {
            std::cout << printf("DNI: {}. Not on DB", DNI) << std::endl; 
            return false;
        }

        currentDNISearched = DNI;
        std::vector<int> indexes = dniStorage.getData(DNI).getFl(); // obtiene los indices de los files asociados del paciente

        ufTypes = unionFind(indexes.size() + Types, Types, indexes);
        int counter = 0;
        for (const auto& indx: indexes)
        {
            ArchivoMultimedia arch = indexManager->get(indx);
            if (FileHelper::do_file_exist(arch.ruta))
            {
                indiceHash[arch.nombre] = indx; // busqueda exacta
                indiceTrie.insertar(arch.nombre, indx); // parcial
                heapFecha.merge(new skewNode<ArchivoMultimedia>(std::stoi(arch.fecha), arch)); // fecha
                heapSize.merge(new skewNode<ArchivoMultimedia>(arch.tamano, arch)); // tamano
                agregarTipoArchivo(arch.tipo, counter + (Types));// tipo
                counter += 1;
            }
        }

        return counter != 0;
    }

    // muestra los files relacionados al dni ingresado, en caso no se ingrese uno se toma el dni actualmente usado
    void showFiles(long dni = 0)
    {
        if (dni == 0)
        {
            dni = currentDNISearched;
        }

        std::cout << "-> Paciente : " << dniStorage.getData(dni).getName() << std::endl;
        std::cout << " Archivos Asociados (Indice en memoria : Nombre : Tipo) " << std::endl;
        std::vector<int> indexes = dniStorage.getData(dni).getFl();

        for (const auto& indx: indexes)
        {
            ArchivoMultimedia arch = indexManager->get(indx);
            std::cout << indx << " : " << arch.nombre << " : " << arch.tipo << std::endl;
        }
    }

    // muestra la lista de dnis en cache
    void showDNI()
    {
        std::cout << "DNIs: " << std::endl;
        for (const auto& d: dniStorage.getKeys())
        {
            std::printf("- %i", d);
        }
        std::cout << "--------------------------------" << std::endl;
    }

    // agrega los archivos por tipo al union find
    void agregarTipoArchivo(std::string tipo, int i)
    {
        int index = -1;
        if (tipo == (".jpg")) // 0 index on union find
        {
            index = 0;
        }
        else 
        if (tipo == (".png"))
        {
            index = 1;
        }else 
        if (tipo == (".mp3"))
        {
            index = 2;
        }else 
        if (tipo == (".txt"))
        {
            index = 3;
        }else 
        if (tipo == (".mp4"))
        {
            index = 4;
        }

        if (index == -1)
        {
            std::cout << "Error on adding type of archive" << std::endl;
            return;
        }

        ufTypes.unite(i, index);
    }

 // RF04: Búsqueda Exacta
    bool buscarExacta(std::string nombre) 
    {
        if (currentDNISearched == 0)
        {
            return false;
        }

        if (indiceHash.count(nombre)) 
        {
            int i = indiceHash[nombre];
            cout << "Archivo Encontrado! Ruta: " << indexManager->get(i).ruta << endl;
            return true;
        } 
        else
        {
            cout << " -- No hay coincidencia exacta --" << endl;
            return false;
        }
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

    // Ordenamiento por nombre (orden alfabético)
    void buscarPorNombre()
    {
        auto words = indiceTrie.obtenerPalabras();

        cout << "\n--- Resultados de Ordenamiento de archivos por Nombre ---" << endl; 

        for (const auto& word: words)
        {
            std::cout << "- " << word << std::endl;
        }
    }

    // Busqueda por fecha usando heaps
    void busquedaPorFecha()
    {
        auto files = heapFecha.getList();

        cout << "\n--- Resultados de Ordenamiento de archivos por Fecha ---" << endl; 

        for (const auto& f: files)
        {
            ImprimirArchivo(f);
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
        
        if (archives.size() <= 0)
        {
            cout << "\n--- No se ha encontrado archivos de ese tipo ---" << endl;
            return;
        }

        cout << "\n--- Resultados de Ordenamiento de archivos por Tipo ---" << endl; 
        for (const auto& res: archives)
        {
            ImprimirArchivo(indexManager->get(res));
        }
        cout << "\n-------------------------------------------------------" << endl; 
    }

    // Ordenamiento por tamano
    void busquedaPorSize()
    {
        auto files = heapSize.getList();

        for (const auto& f: files)
        {
            ImprimirArchivo(f);
        }
    }

    void reCreateDNIFilesRelated(int dni = 0)
    {
        if (dni == 0)
        {
            dni = currentDNISearched;
        }

        if (filesCreated.first != dni)
        {
            filesCreated.second.clear();
            clearFilesRelated();
        }

        filesCreated.first = dni;
        auto indexes = dniStorage.getData(dni).getFl();
        for (const auto& i: indexes)
        {
            ArchivoMultimedia arch = indexManager->get(i);
            ArchivoMultimedia out;
            FileHelper::recreateOriginalFile(arch, out, FileHelper::path_to_aux());
            filesCreated.second.push_back(out);
        }
    }

    void clearFilesRelated()
    {
        if (filesCreated.first == 0)
        {
            return;
        }

        if (filesCreated.second.size() <= 0)
        {
            return;
        }

        for (const auto& f: filesCreated.second)
        {
            FileHelper::eliminateFile(f.ruta);
        }
    }

    // función de apoyo para imprimir archivos
    void ImprimirArchivo(ArchivoMultimedia a)
    {
        std::string show("Nombre: " + a.nombre + ", Tamano: " + std::to_string(a.tamano) + " bytes, Tipo: " + a.tipo + ", Ruta: " + a.ruta);
        std::cout << show  << std::endl;
    }
};

#endif
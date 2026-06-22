#pragma once

#ifndef LOGICMANAGER
#define LOGICMANAGER

#include <vector>
#include "../AlgorithmicNucleus/FileManager.h"
#include "../Data/Query.h"
#include "../Data/DataEnums.h"
#include "../Data/ArchivoMultimedia.h"
#include "../Data/PathsNDefines.h"

// Clase encargada de recibir consultas(Query) y coordinar acciones con la clase File Manager
class LogicManager
{
public:
    LogicManager()
    {
        fileManager = new FileManager(CTYPES);
        Init(); 
    }

    // Initialize
    void Init()
    {
        fileManager->SearchFiles();
    }

    // Actualiza query y lo marca como activo, para iniciar el proceso de búsqueda/ordenamiento
    void generateQuery(Query query)
    {
        if (!query.bActive)
        {
            currentQuery.bActive = false;
            fileManager->clearCurrentSearch();
            return;
        }

        currentQuery = query;
        currentQuery.bActive = true;

        fileManager->clearCurrentSearch();
        fileManager->searchDNIFiles(currentQuery.DNI);
        fileManager->reCreateDNIFilesRelated(currentQuery.DNI);

        switch (query.queryType)
        {
            case QueryType::ordenation:
            {
                createOrdenation();
                break;
            }
            case QueryType::search:
            {
                // Insert if exist return archive name
                // If not, prefix tree search for prefix coincidence
                startSearch();
                break;
            }
            case QueryType::update: // update values: on missing files, rehash or adding files
            {
                updateStructures();
                break;
            }
        }
    }

    // Funciones que exponen comportamiento de File Manager, ya que el proyecto usa responsabilidad limitada entre clases para evitar código spaguetti

    bool do_DNI_Exist(long dni)
    {
        return fileManager->dni_exist(dni);
    }

    bool is_query_active()
    {
        return currentQuery.bActive;
    }

    bool show_files_related(long dni)
    {
        if (!fileManager->dni_exist(dni))
        {
            return false;
        }

        fileManager->showFiles(dni);
        return true;
    }

    void clean_managers()
    {
        fileManager->clearCurrentSearch();
        fileManager->clearFilesRelated();
    }

private:

    // función interna que coordina con file manager que clase de ordenamiento se va a realizar a los archivos actuales en cache
    void createOrdenation()
    {
        auto search = currentQuery.orderType;
        switch (search)
        {
            case OrderType::name:
            {
                fileManager->buscarPorNombre();
                break;
            }
            case OrderType::date:
            {
                fileManager->busquedaPorFecha();
                break;
            }
            case OrderType::type:
            {
                fileManager->busquedaPorTipo(currentQuery.fileType);
                break;
            }
            case OrderType::size:
            {
                fileManager->busquedaPorSize();
                break;
            }
        }
    }

    // Busqueda exacta, de no encontrar archivo directamente, pregunta por prefijo
    void startSearch()
    {
        if (!fileManager->buscarExacta(currentQuery.preFix))
        {
            fileManager->autocompletar(currentQuery.preFix);
        }
    }

    // Actualiza las estructuras de datos usadas para ordenamiento y storage de indices, principalmente re hash
    void updateStructures()
    {
        // Logic 
        // File manager call (Long process, check every DNI - key- so every file integrity is in good condition, then if at least one file is not intact) ->
        //                      -> its index is discarded from its proper DNI value list
        //                      - Other Case: Max Capacity - Current Capacity is close, need to rehash  and reAdd the hashed archives
        //                      
        // File Manager Structures Update
        // Structure Update

        switch (currentQuery.updateType)
        {
            case UpdateType::downscale:
            {
                fileManager->updateStructures(false);
                break;
            }
            case UpdateType::upscale:
            {
                fileManager->updateStructures(true);
                break;
            }
        }
    }

private:
    FileManager* fileManager;
    Query currentQuery;
};

#endif
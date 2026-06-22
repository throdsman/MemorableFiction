#pragma once

#ifndef LOGICMANAGER
#define LOGICMANAGER

#include <vector>
#include "../AlgorithmicNucleus/FileManager.h"
#include "../Data/Query.h"
#include "../Data/DataEnums.h"
#include "../Data/ArchivoMultimedia.h"
#include "../Data/PathsNDefines.h"

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

    // What kind of query is needed
    void generateQuery(Query type)
    {
        currentQuery = type;
        currentQuery.bActive = true;

        fileManager->clearCurrentSearch();
        fileManager->searchDNIFiles(currentQuery.DNI);

        switch (type.queryType)
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

private:

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

    void startSearch()
    {
        if (!fileManager->buscarExacta(currentQuery.preFix))
        {
            fileManager->autocompletar(currentQuery.preFix);
        }
    }

    void updateStructures()
    {
        // Logic needed
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
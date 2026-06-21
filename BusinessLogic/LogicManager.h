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
        
    }

    // What kind of query is needed
    void generateQuery(Query type)
    {
        currentQuery = type;
        switch (type.queryType)
        {
            case QueryType::ordenation:
            {
                createOrdenation();
                break;
            }
            case QueryType::search:
            {
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

private:

    void createOrdenation()
    {
        fileManager->clearCurrentSearch();
        fileManager->searchDNIFiles(currentQuery.DNI);

        auto search = currentQuery.orderType;
        switch (search)
        {
            case OrderType::name:
            {
                fileManager->autocompletar("");
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

    void loopOrdenation()
    {
        bool flag = false;
        while (!flag)
        {
            std::cout << "Ingrese DNI a buscar: " << std::endl;
            int dni = 0;
            std::cin >> dni;

            flag = fileManager->searchDNIFiles(dni);
        }
    }

    int getSearchOption()
    {
        int ret = -1;
        std::cout << "Elegir Opción de busqueda: \n  1) Name \n  2) Size \n  3) Type \n  4) Date \n  5) Return"<< std::endl;
        std::cin >> ret;
        return ret;
    }

    void resolveOption(int option)
    {
        if (option == 5) // return
        {
            return;
        }

        switch (option)
        {
            case 1: // name
            {
                std::string name = "";
                std::cin >> name;
                fileManager->buscarExacta(name);
                break;
            }
            case 2: // size
            {
                break;
            }
            case 3: // type
            {
                break;
            }
            case 4: // date
            {
                break;
            }
            default: // go again
            {
                std::cout << "Select a valid option!" << std::endl;
            }
        }
    }

    void startSearch()
    {
        fileManager->autocompletar(currentQuery.preFix);
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
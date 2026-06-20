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
                createOrdenation(type);
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

    void createOrdenation(Query t)
    {
        fileManager->clearCurrentSearch();
        fileManager->showDNI();
        loopOrdenation();
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

    void startSearch()
    {
        loopSearch();
    }

    void loopSearch()
    {
        bool flag = false;
        while (!flag)
        {



            if (flag)
            {
                exitSearch();
            }
        }
    }

    void exitSearch()
    {

    }

    void updateStructures()
    {

    }

private:
    FileManager* fileManager;
    Query currentQuery;
};

#endif
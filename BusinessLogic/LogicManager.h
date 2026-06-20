#pragma once

#ifndef LOGICMANAGER
#define LOGICMANAGER

#include <vector>
#include "../AlgorithmicNucleus/FileManager.h"
#include "../Data/Query.h"
#include "../Data/DataEnums.h"
#include "../Data/ArchivoMultimedia.h"

class LogicManager
{
public:
    LogicManager()
    {
        fileManager = new FileManager(5);
    }

    // Initialize
    void Init()
    {

    }

    // What kind of query is needed
    void generateQuery(QueryType type)
    {

    }

private:

    void createOrdenation(OrderType type = OrderType::none)
    {
        
    }


private:
    FileManager* fileManager;
    Query currentQuery;
};

#endif
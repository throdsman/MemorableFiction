#pragma once

#ifndef QUERY
#define QUERY

#include "../Data/ArchivoMultimedia.h"
#include "../Data/DataEnums.h"

class Query
{
public:
    Query()
    {

    }

    Query(OrderType t, FileType f, QueryType q, SortType s, UpdateType u)
    : orderType(t), fileType(f), queryType(q), sortType(s), updateType(u)
    {
        
    }
public:
    OrderType orderType;
    FileType fileType;
    QueryType queryType;
    SortType sortType;
    UpdateType updateType;
    long DNI = 0;
    std::string preFix;
    bool bActive = false;
};

#endif


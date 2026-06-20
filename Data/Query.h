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

    Query(OrderType t): type(t)
    {

    }
public:
    OrderType type;
};

#endif
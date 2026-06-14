#include <vector>
#include <cstring>

#ifndef RELATIONS
#define RELATION

struct dataRelation
{

    dataRelation()
    {
    }

    dataRelation(int d, char *id) : DNI(d)
    {
        std::strncpy(this->UID, id, 8);
    }

private:
    int DNI;
    char UID[9];
};

struct dataPostFix
{
    dataPostFix()
    {
    }

    dataPostFix(int d, int minfiles) : DNI(d)
    {
        this->postFixList.reserve(minfiles);
        this->fileName.reserve(minfiles);
    }

private:
    int DNI;
    std::vector<char *> postFixList; // 1 to 1 ClientData filelist
    std::vector<char *> fileName;
};

#endif

#include <iostream>
#include <string>
#include "BusinessLogic/LogicManager.h"


int main()
{
    // Muestra los DNIS
    LogicManager* lm = new LogicManager();

    // Seleccionar un DNI
    std::printf("Ingresar DNI de paciente: \n");
    Query consulta;
    std::cin >> consulta.DNI;

    std::printf("Ingresar Opcion : \n 1) Ordenar Archivos \n 2) Buqueda por prefijos \n");
    int option = 0;
    std::cin >> option;

    // Búsqueda
    if (option == 2)
    {
        consulta.queryType = QueryType::search;
    }
    // Preguntar Ordenamiento
    else if (option == 1)
    {
        consulta.queryType = QueryType::ordenation;
        std::printf("Ingresar Opcion de ordenamiento: \n 1) Nombre \n 2) Size \n 3) Fecha \n 4) Tipo \n");
        std::cin >> option;
        switch (option)
        {
            case 1:
            {
                consulta.orderType = OrderType::name;
                break;
            }
            case 2:
            {
                consulta.orderType = OrderType::size;
                break;
            }
            case 3:
            {
                consulta.orderType = OrderType::date;
                break;
            }
            case 4:
            {
                consulta.orderType = OrderType::type;
                break;
            }
        }
    }
    else
    {
        std::printf("Opción no valida \n");
    }

    lm->generateQuery(consulta);

    return 0;
}
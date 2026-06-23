
#include <iostream>
#include <string>
#include "BusinessLogic/LogicManager.h"

void askDNI(long& dni)
{
    // Seleccionar un DNI
    std::printf("Ingresar DNI de paciente: \n");
    std::cin >> dni;
}

void primaryLoop(Query& consulta, LogicManager* lm)
{ 
    do
    {
        int option = 0;
        std::printf("Ingresar Opcion : \n 1) Ordenar Archivos \n 2) Busqueda por prefijos \n %s \n", ((lm->is_query_active() ? "3) Finalizar Consulta" : "")));
        std::cin >> option;

        // Permite cancelas la solicitud
        if (lm->is_query_active() && option == 3)
        {
            consulta.bActive = false;
        }
        // Búsqueda exacta o prefijo
        else if (option == 2)
        {
            std::string prefix = "";

            if (consulta.preFix.size() > 0)
            {
                std::printf("Borrar Busqueda Anterior?: \n  -> %s <- (Y/N) \n", consulta.preFix);
                char y = ' ';
                std::cin >> y;
                y = tolower(y);
                if (y == 'y')
                {
                    consulta.preFix = "";
                }
            }

            consulta.queryType = QueryType::search;
            std::printf("Ingresar Nombre de Archivo a buscar: \n %s", consulta.preFix);
            std::cin >> prefix;

            consulta.preFix += prefix;
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
                    std::printf("Ingresar Opcion de ordenamiento por tipo de archivo: \n 1) JPG \n 2) MP3 \n 3) MP4 \n 4) PNG \n 5) TXT \n");
                    std::cin >> option;
                    switch (option)
                    {
                        case 1:
                        {
                            consulta.fileType = FileType::jpg;
                            break;
                        }
                        case 2:
                        {
                            consulta.fileType = FileType::mp3;
                            break;
                        }
                        case 3:
                        {
                            consulta.fileType = FileType::mp4;
                            break;
                        }
                        case 4:
                        {
                            consulta.fileType = FileType::png;
                            break;
                        }
                        case 5:
                        {
                            consulta.fileType = FileType::txt;
                            break;
                        }
                        default:
                        {
                            std::printf("Opción no valida \n");
                        }
                    }
                    break;
                }
            }
        }
        else
        {
            std::printf("Opción no valida \n");
        }

        // Envia el Query creado y actualiza para su procesamiento
        lm->generateQuery(consulta);
    
    } while (lm->is_query_active());
}

int main()
{
    LogicManager* lm = new LogicManager();

    bool flag = false;
    while(!flag)
    {
        Query consulta;
        int option;
        std::printf("Ingresar Opcion : \n 1) Empezar Busqueda \n 2) Verificar Integridad de Archivo \n 3) Salir \n");
        std::cin >> option;
        switch (option)
        {
            case 1:
            {
                consulta.bActive = true;
                            
                do 
                {
                    askDNI(consulta.DNI);

                } while(!lm->do_DNI_Exist(consulta.DNI));

                // Muestras los files relacionados al dni ingresado, de manera que se puede hacer una comparativa con las busquedas y ordenamientos a continuación sugeridos...
                lm->show_files_related(consulta.DNI);

                primaryLoop(consulta, lm);

                break;
            }
            case 2:
            {
                consulta.bActive = true;
                consulta.queryType = QueryType::update;
                consulta.updateType = UpdateType::upscale;

                // Envia el Query creado y actualiza para su procesamiento
                lm->generateQuery(consulta);
                break;
            }
            case 3:
            {
                flag = true;
                break;
            }
            default:
            {
                std::printf("Ingresar Opcion valida:");
                break;
            }
        }
    }
    return 0;
}
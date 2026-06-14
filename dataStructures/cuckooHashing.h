#include <iostream>
#include <vector>
#include <utility>

#define TAM 1000
#define MAX_REUBICACIONES 16

template <typename T>
class cHash
{
public:
    cHash()
    {
        tabla1.resize(TAM, std::pair<int, T>());
        tabla2.resize(TAM, std::pair<int, T>());
    }

    bool buscar(int clave)
    {
        int posicion1 = this->hash1(clave);
        if (tabla1[posicion1].first == clave)
        {
            return true;
        }
        int posicion2 = this->hash2(clave);
        if (tabla2[posicion2].first == clave)
        {
            return true;
        }
        return false;
    }
    void eliminar(int clave)
    {
        int posicion1 = this->hash1(clave);
        if (tabla1[posicion1].first == clave)
        {
            tabla1[posicion1].first = -1;
            tabla1[posicion1].second = T();
        }
        int posicion2 = this->hash2(clave);
        if (tabla2[posicion2].first == clave)
        {
            tabla2[posicion2].first = -1;
            tabla2[posicion2].second = T();
        }
        return;
    }
    void mostrarTablas()
    {
        std::cout << "Mostrar Tabla1" << std::endl;
        int ind = 0;
        for (const auto &a : this->tabla1)
        {
            if (a.first != -1)
            {
                std::cout << a.first << " : " << a.second << std::endl;
            }
        }
        std::cout << "--------------------------" << std::endl;
        std::cout << "Mostrar Tabla2" << std::endl;
        ind = 0;
        for (const auto &a : this->tabla2)
        {
            if (a.first != -1)
            {
                std::cout << a.first << " : " << a.second << std::endl;
            }
        }
    }

    void insertar(std::pair<int, T> clave)
    {
        if (this->buscar(clave.first))
        {
            std::cout << "Esa clave ya existe" << std::endl;
            return;
        }
        int tablaActual = 1;
        auto claveActual = clave;
        for (int i = 0; i < MAX_REUBICACIONES; i++)
        {
            if (tablaActual == 1)
            {
                int posicion = this->hash1(claveActual.first);
                if (tabla1[posicion].first == -1)
                {
                    tabla1[posicion] = claveActual;
                    std::cout << "Clave Actual ingresada en Tabla 1" << std::endl;
                    return;
                }
                else
                {
                    auto claveDesplazada = tabla1[posicion];
                    tabla1[posicion] = claveActual;
                    claveActual = claveDesplazada;
                    tablaActual = 2;
                }
            }
            else
            {
                int posicion = this->hash2(claveActual.first);
                if (tabla2[posicion].first == -1)
                {
                    tabla2[posicion] = claveActual;
                    std::cout << "Clave Actual ingresada en Tabla 1" << std::endl;
                    return;
                }
                else
                {
                    auto claveDesplazada = tabla2[posicion];
                    tabla2[posicion] = claveActual;
                    claveActual = claveDesplazada;
                    tablaActual = 1;
                }
            }
        }
        std::cout << "Se Detecto un ciclo, es necesario hacer reHashing" << std::endl;
    }
    void llenarTabla(int minVal, int items)
    {
        static int rnd;
        for (int i = 0; i < items; i++)
        {
            this->insertar(i + (std::rand() % minVal) + (rnd++));
        }
    }

private:
    int hash1(int clave)
    {
        return clave % TAM;
    }
    int hash2(int clave)
    {
        return (clave / TAM) % TAM;
    }

private:
    std::vector<std::pair<int, T>> tabla1;
    std::vector<std::pair<int, T>> tabla2;
};
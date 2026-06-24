#include <iostream>
#include <vector>
#include <utility>

#define TAM 100000
#define MAX_REUBICACIONES 16

#ifndef CHASH
#define CHASH

template <typename T>
class cHash
{
public:
    cHash()
    {
        size = TAM;
        tabla1.resize(size, std::pair<int, T>());
        tabla2.resize(size, std::pair<int, T>());
    }

    cHash(int s): size(s)
    {
        tabla1.resize(s, std::pair<int, T>());
        tabla2.resize(s, std::pair<int, T>());
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
    T getData(int clave)
    {
        int posicion1 = this->hash1(clave);
        if (tabla1[posicion1].first == clave)
        {
            return tabla1[posicion1].second;
        }
        int posicion2 = this->hash2(clave);
        if (tabla2[posicion2].first == clave)
        {
            return tabla2[posicion2].second;
        }
        return T();
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
    }
    void llenarTabla(int minVal, int items)
    {
        static int rnd;
        for (int i = 0; i < items; i++)
        {
            this->insertar(i + (std::rand() % minVal) + (rnd++));
        }
    }
    std::vector<int> getKeys()
    {
        std::vector<int> ret;
        for (size_t i{0u}; i < tabla1.size();i++)
        {
            if (tabla1[i].first != -1)
            {
                ret.push_back(tabla1[i].first);
            }

            if (tabla2[i].first != -1)
            {
                ret.push_back(tabla2[i].first);
            }
        }

        return ret;
    }

    std::vector<T> getValues()
    {
        std::vector<T> ret;
        for (size_t i{0u}; i < tabla1;i++)
        {
            if (tabla1[i].first != -1)
            {
                ret.push_back(tabla1[i].second);
            }

            if (tabla2[i].first != -1)
            {
                ret.push_back(tabla2[i].second);
            }
        }

        return ret;
    }

    std::vector<std::pair<int, T>> getList()
    {
        std::vector<std::pair<int, T>> ret;
        for (size_t i{0u}; i < tabla1.size();i++)
        {
            if (tabla1[i].first != -1)
            {
                ret.push_back(tabla1[i]);
            }

            if (tabla2[i].first != -1)
            {
                ret.push_back(tabla2[i]);
            }
        }

        return ret;
    }

    void reHash(int newSize)
    {
        size = newSize;
        auto aux = getList();

        tabla1.clear();
        tabla2.clear();

        tabla1.resize(size, std::pair<int, T>());
        tabla2.resize(size, std::pair<int, T>());

        for (const auto& v: aux)
        {
            if (v.first != 0)
            {
                insertar(v);
            }
        }
    }

    int getSize()
    {
        return size;
    }

private:
    int hash1(int clave)
    {
        return clave % size;
    }
    int hash2(int clave)
    {
        return (clave / size) % size;
    }

private:
    std::vector<std::pair<int, T>> tabla1;
    std::vector<std::pair<int, T>> tabla2;
    int size;
};

#endif
#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <string>
#include <stdexcept>
#include "ArchivoMultimedia.h"

using namespace std;

//Urgent Change
class HeapFecha {
private:
    vector<ArchivoMultimedia> datos;

    void subir(int i) {
        while (i > 0) {
            int padre = (i - 1) / 2;
            if (datos[i].fecha > datos[padre].fecha) {
                swap(datos[i], datos[padre]);
                i = padre;
            } else break;
        }
    }

    void bajar(int i) {
        int n = datos.size();
        while (true) {
            int izq   = 2 * i + 1;
            int der   = 2 * i + 2;
            int mayor = i;

            if (izq < n && datos[izq].fecha > datos[mayor].fecha)
                mayor = izq;
            if (der < n && datos[der].fecha > datos[mayor].fecha) 
                mayor = der;

            if (mayor != i) {
                swap(datos[i], datos[mayor]);
                i = mayor;
            } else break;
        }
    }

public:
    void insertar(ArchivoMultimedia a) {
        datos.push_back(a);
        subir(datos.size() - 1);
    }

    ArchivoMultimedia top() {
        if (datos.empty()) throw runtime_error("Heap vacio");
        return datos[0];
    }

    ArchivoMultimedia extraer() {
        if (datos.empty()) throw runtime_error("Heap vacio");
        ArchivoMultimedia resultado = datos[0];
        datos[0] = datos.back();
        datos.pop_back();
        if (!datos.empty()) bajar(0);
        return resultado;
    }

    vector<ArchivoMultimedia> ordenados() {
        HeapFecha copia = *this;
        vector<ArchivoMultimedia> resultado;
        while (!copia.vacio())
            resultado.push_back(copia.extraer());
        return resultado;
    }

    ArchivoMultimedia masAntiguo() {
        if (datos.empty()) throw runtime_error("Heap vacio");
        ArchivoMultimedia minimo = datos[0];
        for (auto& a : datos)
            if (a.fecha < minimo.fecha) minimo = a;
        return minimo;
    }

    bool vacio() { return datos.empty(); }
    int  size()  { return datos.size();  }
};

#endif

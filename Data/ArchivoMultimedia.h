#pragma once

#include <string>

using namespace std;

struct ArchivoMultimedia 
{
    string nombre;
    string paciente;
    long dni;
    string tipo;
    string ruta;
    string fecha;
    long long tamano;
    char c;

    ArchivoMultimedia() {}
    ArchivoMultimedia(string n, string t, string r, string f, string p)
        : nombre(n), tipo(t), ruta(r), fecha(f), paciente(p){}
    ArchivoMultimedia(char s, string n, string p, long d,
                    string t, string r, string f, long long tam)
        : c(s), nombre(n), paciente(p), dni(d),
        tipo(t), ruta(r), fecha(f), tamano(tam) {}
};

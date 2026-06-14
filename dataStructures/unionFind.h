#include <iostream> 
#include <vector> 
#include <string> 

using namespace std; 
 
class UnionFind 
{ 

private: 

    vector<int> parent; 
    vector<int> rango; 

public: 

    UnionFind(int n) { 

        parent.resize(n); 

        rango.resize(n, 0); 

 

        for (int i = 0; i < n; i++) { 

            parent[i] = i; 

        } 

    } 

    int find(int x) { 

        if (parent[x] == x) 
            return x; 

        parent[x] = find(parent[x]); 

        return parent[x]; 

    } 

    void unite(int a, int b) { 

        int raizA = find(a); 
        int raizB = find(b); 

        if (raizA == raizB) 
            return; 


        if (rango[raizA] < rango[raizB]) { 

            parent[raizA] = raizB; 

        } 

        else if (rango[raizA] > rango[raizB]) { 

            parent[raizB] = raizA; 

        } 

        else { 

            parent[raizB] = raizA; 

            rango[raizA]++; 

        } 

    } 

    bool mismoGrupo(int a, int b) { 

        return find(a) == find(b); 

    } 

    void mostrarPadres() { 

        cout << "\nTabla de representantes:\n"; 

 

        for (size_t i = 0; i < parent.size(); i++) { 

            cout << "Archivo " << i 

                 << " -> Grupo " 

                 << find(i) << endl; 

        } 

    } 

};
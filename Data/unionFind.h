#include <iostream> 
#include <vector> 
#include <string> 

using namespace std; 
 
// ADAPT!!!!!!
class unionFind 
{ 

private: 

    vector<int> parent; 
    vector<int> rango; 

public: 

    unionFind()
    {
        
    }

    unionFind(int n, int types) { 

        clear(n, types);

    } 

    int find(int x) 
    { 
        int root = parent[x];

        if (this->parent[root] != root)
        {
            return find(root);
        }

        return root;
    } 

    std::vector<int> get(int x)
    {
        std::vector<int> ret;
        int val = parent[x];
        while (val != x)
        {
            ret.push_back(val);
            x = val;
            val = parent[val];
        }
        return ret;
    }

    void unite(int a, int b) { 

        int raizA = find(a); 
        int raizB = find(b); 

        if (raizA == raizB) 
            return; 


        if (rango[raizA] < rango[raizB]) { 
            parent[raizA] = raizB; 
            rango[raizB] += rango[raizA];
        } 
        else if (rango[raizA] > rango[raizB]) { 
            parent[raizB] = raizA; 
            rango[raizA] += rango[raizB];
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

                 << parent[i] << endl; 

        } 

    } 

    void clear(int n, int types)
    {
        parent.resize(n); 

        rango.resize(n, 0); 

        for (int i = 0; i < n; i++) 
        { 
            if (i < types)
            {
                rango[i] = -1;
            }
            parent[i] = i;
        } 
    }
};
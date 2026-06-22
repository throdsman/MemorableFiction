#include <iostream> 
#include <vector> 
#include <string> 

using namespace std; 
 
// ADAPT!!!!!!
class unionFind 
{ 

private: 

    // <Parent Indx, Ram Index>
    vector<std::pair<int, int>> parent; 
    vector<int> rango; 

public: 

    unionFind()
    {
        
    }

    unionFind(int n, int types, std::vector<int> indexes) 
    { 
        clear(n, types);

        for (size_t i{0u}; i < indexes.size(); i++)
        {
            parent[i + types].second = indexes[i];
        }
    } 

    int find(int x) 
    { 
        int root = parent[x].first;

        if (this->parent[root].first != root)
        {
            return find(root);
        }

        return root;
    } 

    std::vector<int> get(int x)
    {
        std::vector<int> ret;
        int val = parent[x].first;
        while (val != x)
        {
            ret.push_back(parent[val].second);
            x = val;
            val = parent[val].first;
        }
        return ret;
    }

    void unite(int a, int b) { 

        int raizA = find(a); 
        int raizB = find(b); 

        if (raizA == raizB) 
            return; 


        if (rango[raizA] < rango[raizB]) { 
            parent[raizA].first = raizB; 
            rango[raizB] += rango[raizA];
        } 
        else if (rango[raizA] > rango[raizB]) { 
            parent[raizB].first = raizA; 
            rango[raizA] += rango[raizB];
        } 
        else { 
            parent[raizB].first = raizA; 
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

                 << parent[i].first << endl; 

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
            parent[i].first = i;
        } 
    }
};
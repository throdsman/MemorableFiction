#include <string>
#include <vector>
#include <unordered_map>

struct TrieNode { 
    std::unordered_map<char, TrieNode*> hijos; 
    std::vector<int> indicesArchivos; // Guarda índices del vector principal 
}; 

class TrieBusqueda { 
private: 
    TrieNode* root; 
 
public: 
    TrieBusqueda() { root = new TrieNode(); } 
 
    // Inserta nombres para búsqueda parcial (RF05) 
    void insertar(std::string nombre, int index) { 
        TrieNode* curr = root; 
        for (char c : nombre) { 
            char lowC = tolower(c); 
            if (curr->hijos.find(lowC) == curr->hijos.end()) { 
                curr->hijos[lowC] = new TrieNode(); 
            } 
            curr = curr->hijos[lowC]; 
            // Registra el archivo en este nodo de prefijo 
            curr->indicesArchivos.push_back(index); 
        } 
    } 
 
    // Retorna IDs de archivos que coinciden con el prefijo 
    std::vector<int> buscarPrefijo(std::string prefijo) { 
        TrieNode* curr = root; 
        for (char c : prefijo) { 
            char lowC = tolower(c); 
            if (curr->hijos.find(lowC) == curr->hijos.end()) return {}; 
            curr = curr->hijos[lowC]; 
        } 
        return curr->indicesArchivos; 
    }

    void clean()
    {
        delete root;
        root = new TrieNode();
    }
}; 
#include <string>
#include <vector>
#include <unordered_map>

struct TrieNode 
{ 
    TrieNode()
    {
        hijos.resize(26, nullptr);
        bLeaf = false;
    }

    std::vector<TrieNode*> hijos; 
    int index; // Guarda índices del vector principal 
    bool bLeaf;
}; 

class TrieBusqueda { 
private: 
    TrieNode* root; 
 
public: 
    TrieBusqueda() { root = new TrieNode(); } 
 
    // Inserta nombres para búsqueda parcial (RF05) 
    void insertar(std::string nombre, int index)
    {
        if (nombre.empty())
        {
            return;
        }

        this->to_Lower(nombre);

        TrieNode* curr = root;
        for (const auto& c: nombre)
        {
            if (curr->hijos[c - 'a'] == nullptr)
            {
                curr->hijos[c - 'a'] = new TrieNode();
            }

            curr = curr->hijos[c - 'a'];

            if (c == nombre.back())
            {
                curr->index = index;
                curr->bLeaf = true;
            }
        }
    }

    // Retorna IDs de archivos que coinciden con el prefijo 
    void buscarPrefijo(std::string prefijo, std::vector<int>& ret)
    {
        this->to_Lower(prefijo);
        TrieNode* curr = root;
        for (const auto& c: prefijo)
        {
            if (curr->hijos[c - 'a'] == nullptr)
            {
                return;
            }

            curr = curr->hijos[c - 'a'];
        } 
        
        for (const auto h: curr->hijos)
        {
            if (h != nullptr)
            {
                auxBuscarPrefix(h, ret);
            }
        }
    }

    std::vector<std::string> obtenerPalabras()
    {
        std::vector<std::string> ret;
        auxObtenerPalabras(root, "", ret);
        return ret;
    }

    void clean()
    {
        delete root;
        root = new TrieNode();
    }

private:

    void auxBuscarPrefix(TrieNode* node, std::vector<int>& ret)
    {
        if (node->bLeaf)
        {
            ret.push_back(node->index);
        }

        for (const auto h: node->hijos)
        {
            if (h != nullptr)
            {
                auxBuscarPrefix(h, ret);
            }
        }
    }

    void auxObtenerPalabras(TrieNode* node, std::string currentString, std::vector<std::string>& out)
    {
        if (node->bLeaf)
        {
            out.push_back(currentString);
        }

        int count = 0;
        for (const auto h: node->hijos)
        {
            if (h != nullptr)
            {
                auxObtenerPalabras(h, currentString + (char)(count + 'a'), out);
            }
            count += 1;
        }
    }

    void to_Lower(std::string& out)
    {
        for (auto& c: out)
        {
            c = tolower(c);
        }
    }
}; 
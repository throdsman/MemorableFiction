#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

template <typename T>
struct skewNode
{
    skewNode()
    {
        left = nullptr;
        right = nullptr;
    }

    skewNode(int k, T v): key(k), value(v)
    {
        left = nullptr;
        right = nullptr;
    }

    int key;
    T value;
    skewNode<T> *left;
    skewNode<T> *right;
};

template <typename T>
struct skewHeap
{
    skewNode<T>* root;

    skewHeap()
    {
        root = nullptr;
    }

    skewheap(std::vector<skewNode<T>> inArr)
    {
        skewNode<T> *temp;
        for (size_t i = 0; i < inArr.size(); i++)
        {
            temp = new skewNode<T>(inArr[i]);
            root = this->merge(root, temp);
        }
    }

    
    skewNode<T> *merge(skewNode<T> *h1)
    {
        root = merge(root, h1);
        return root;
    }

    skewNode<T> *merge(skewNode<T> *h1, skewNode<T> *h2)
    {
        if (h1 == nullptr)
        {
            return h2;
        }
        if (h2 == nullptr)
        {
            return h1;
        }

        if (h1->key > h2->key)
        {
            swap(h1, h2);
        }

        swap(h1->left, h1->right);

        h1->left = this->merge(h2, h1->left);

        return h1;
    }

    void inorder(skewNode<T> *root)
    {
        if (root == nullptr)
        {
            return;
        }
        else
        {
            inorder(root->left);
            std::cout << root->key << " ";
            inorder(root->right);
        }

        return;
    }

    skewNode<T>* getRoot()
    {
        if (root == nullptr)
        {
            return nullptr;
        }

        skewNode<T> aux = *root;
        
        delete root;
        root = nullptr;

        root = merge(aux.right, aux.left);

        return root;
    }

    // WARNING: Heap gets destroyed after
    std::vector<skewNode<T>> getList()
    {
        std::vector<skewNode<T>> ret;
        while (root != nullptr)
        {
            if (auto aux = getRoot())
            {
                ret.push_back(*aux);
            }
        }

        return ret;
    }

    void empty()
    {
        delete root;
        root = nullptr;
    }
};

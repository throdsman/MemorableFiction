#include <algorithm>
#include <iostream>

using namespace std;

struct skewHeap
{
    int key;
    skewHeap *left;
    skewHeap *right;

    skewHeap(int k) : key(k)
    {
        this->left = nullptr;
        this->right = nullptr;
    }

    skewHeap *merge(skewHeap *h1, skewHeap *h2)
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

    skewHeap *constructor(skewHeap *root, int heap[], int n)
    {
        skewHeap *temp;
        for (size_t i = 0; i < n; i++)
        {
            temp = new skewHeap(heap[i]);
            root = this->merge(root, temp);
        }
        return root;
    }

    void inorder(skewHeap *root)
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
};

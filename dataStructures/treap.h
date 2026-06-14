#include <cstdlib>
#include <iostream>

struct Nodo
{
    Nodo(int k, int p) : key(k), priority(p)
    {
        this->left = nullptr;
        this->right = nullptr;
    }

    Nodo(int k, int p, std::string n) : key(k), priority(p), data(n)
    {
        this->left = nullptr;
        this->right = nullptr;
    }

    int key;
    int priority;
    std::string data;
    Nodo *left;
    Nodo *right;
};

Nodo *rotateRight(Nodo *y)
{
    Nodo *x = y->left;
    Nodo *aux = x->right;

    x->right = y;
    y->left = aux;

    return x;
}

Nodo *rotateLeft(Nodo *y)
{
    Nodo *x = y->right;
    Nodo *aux = x->left;

    x->left = y;
    y->right = aux;

    return x;
}

Nodo *insert(Nodo *root, int key, std::string data = " ")
{
    if (root == nullptr)
    {
        return new Nodo(key, std::rand() % 1000, data);
    }

    if (key < root->key)
    {
        root->left = insert(root->left, key, data);

        if (root->left->priority > root->priority)
        {
            root = rotateRight(root);
        }
    }
    else if (key > root->key)
    {
        root->right = insert(root->right, key, data);

        if (root->right->priority > root->priority)
        {
            root = rotateLeft(root);
        }
    }

    return root;
}

bool search(Nodo *root, int key)
{
    if (root == nullptr)
    {
        return false;
    }

    if (root->key == key)
    {
        return true;
    }

    if (key < root->key)
    {
        return search(root->left, key);
    }
    else
    {
        return search(root->right, key);
    }
}

Nodo *eliminate(Nodo *root, int key)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    if (key < root->key)
    {
        root->left = eliminate(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = eliminate(root->right, key);
    }
    else
    {
        if (root->left == nullptr)
        {
            Nodo *temp = root->right;
            delete root;
            root = temp;
        }
        else if (root->right == nullptr)
        {
            Nodo *temp = root->left;
            delete root;
            root = temp;
        }
        else
        {
            if (root->left->priority > root->right->priority)
            {
                root = rotateRight(root);
                root->right = eliminate(root->right, key);
            }
            else
            {
                root = rotateLeft(root);
                root->left = eliminate(root->left, key);
            }
        }
    }

    return root;
}

void inorder(Nodo *root)
{
    if (root != nullptr)
    {
        inorder(root->left);

        std::cout << printf("%i : %i : ", root->key, root->priority) << root->data << std::endl;

        inorder(root->right);
    }
}

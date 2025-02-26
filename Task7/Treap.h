#pragma once

template<class T>
class Treap
{
public:
    void insert(T key);
    void show_treap();

private:
    struct node
    {
        T key;
        int prio;
        node* left = 0;
        node* right = 0;
        node(T key) : key(key), prio(rand()) {}
    };
    node* root = 0;
    static mt19937_64 generator;
    function<bool(T a, T b)> compar;

    void show_treap(node* ver, int level);
    node* merge(node* a, node* b)
    {
        if (!a || !b)
            return a ? a : b;

        if (a->prio > b->prio) {
            a->right = merge(a->right, b);
            return a;
        }
        else {
            b->left = merge(a, b->left);
            return b;
        }
    }
    void split(node* ver, T key, node*& a, node*& b);
};

template<class T>
void Treap<T>::split(node* ver, T key, node*& a, node*& b)
{
    if (!ver) {
        a = 0;
        b = 0;
        return;
    }
    if (ver->key < key) {
        split(ver->right, key, ver->right, b);
        a = ver;
    }
    else {
        split(ver->left, key, a, ver->left);
        b = ver;
    }
}

template<class T>
void Treap<T>::insert(T key)
{
    node* greater;
    node* less;
    split(root, key, less, greater);
    less = merge(less, new node(key));
    root = merge(less, greater);
}

template<class T>
void Treap<T>::show_treap()
{
    show_treap(root, 0);
}

template<class T>
void Treap<T>::show_treap(node* ver, int level)
{
    if (ver->left) {
        show_treap(ver->left, level + 1);
    }
    for (int i = 0; i < level; ++i) {
        cout << "   ";
    }
    cout << ver->key << '\n';
    if (ver->right) {
        show_treap(ver->right, level + 1);
    }
}
#include "bst.h"
#include "vector"
using std::vector;

template <class K, class D>
myBST<K, D>::myBST()
{
    this->size = 0;
    this->root = nullptr;
}

template <class K, class D>
typename myBST<K, D>::Node* myBST<K, D>::insertHelp(Node* t, const K& key, const D& data)
{
    ops++;

    if (t == nullptr)
    {
        Node* temp = new Node(key, data);
        this->size++;
        return temp;
    }

    if (key < t->key)
        t->left = insertHelp(t->left, key, data);
    else if (key > t->key)
        t->right = insertHelp(t->right, key, data);
    else if (key == t->key)
        throw std::runtime_error("This key already exists");
    return t;
}

template <class K, class D>
void myBST<K, D>::insert(const K& key, const D& data)
{
    try
    {
        ops = 0;
        this->root = insertHelp(this->root, key, data);
    }
    catch (std::runtime_error e)
    {
        //std::cout << e.what() << std::endl;
    }
}

template <class K, class D>
void myBST<K, D>::deleteHelp(Node*& t)
{
    this->size = 0;
    if (t != nullptr)
    {
        ops++;
        deleteHelp(t->left);
        deleteHelp(t->right);
        delete t;
        t = nullptr;
    }
}

template <class K, class D>
myBST<K, D>::~myBST()
{
    deleteHelp(this->root);
}

template <class K, class D>
void myBST<K, D>::clear()
{
    deleteHelp(this->root);
    ops = 0;
}

template <class K, class D>
myBST<K, D>::myBST(const myBST<K, D>& src)
{
    this->root = src.root;
    this->size = src.size;
    this->nodes = src.nodes;
}

template <class K, class D>
void myBST<K, D>::showHelp(Node* t, bool withValues)
{
    if (t == nullptr)
    {
        return;
    }
    else
    {
        showHelp(t->left, withValues);
        showHelp(t->right, withValues);
        withValues == true ? std::cout << "Key: " << t->key << " Value: " << t->data << std::endl : std::cout << "Key: " << t->key << std::endl;
    }
}

template <class K, class D>
void myBST<K, D>::show()
{
    if (this->size <= 0)
    {
        std::cout << "Tree is emptry" << std::endl;
        std::cout << this->size << std::endl;
        return;
    }
    showHelp(this->root, true);
    std::cout << "Size: " << std::endl;
    std::cout << this->size << std::endl;
}

template <class K, class D>
void myBST<K, D>::showKeys()
{
    showHelp(this->root, false);
}

template <class K, class D>
typename myBST<K, D>::Node* myBST<K, D>::searchHelp(Node*& t, const K& key)
{
    ops++;
    if (t == nullptr || t->key == key)
        return t;
    if (key < t->key)
        return searchHelp(t->left, key);
    else
        return searchHelp(t->right, key);
}

template <class K, class D>
typename myBST<K, D>::Node* myBST<K, D>::search(const K& key)
{
    Node* temp = searchHelp(this->root, key);
    if (temp == nullptr)
        throw std::runtime_error("No such key");
    return temp;
}

template <class K, class D>
D myBST<K, D>::getData(const K& key)
{
    ops = 0;
    const Node* temp = search(key);

    return temp->data;
}

template <class K, class D>
void myBST<K, D>::setData(const K& key, const D& data)
{
    try
    {
        Node* temp = searchHelp(this->root, key);
        if (temp != nullptr)
        {
            temp->data = data;
        }
        else
            throw std::runtime_error("No such key");
        ;
    }
    catch (std::runtime_error e)
    {
        // std::cout << e.what() << std::endl;
        return;
    }
}

template <class K, class D>
void myBST<K, D>::remove(const K& key)
{
    try
    {
        this->ops = 0;
        this->root = removeHelp(this->root, key);
        if (this->size <= 0)
            this->root = nullptr;
    }
    catch (std::runtime_error e)
    {
        //  std::cout << e.what() << std::endl;
    }
}

template <class K, class D>
typename myBST<K, D>::Node* myBST<K, D>::removeHelp(Node* t, const K& key)
{
    ops++;
    if (t == nullptr)
        throw std::runtime_error("No such key");
    if (key < t->key)
        t->left = removeHelp(t->left, key);
    else if (key > t->key)
        t->right = removeHelp(t->right, key);
    else if (t->left != nullptr && t->right != nullptr)
    {
        Node* temp = t->right;
        while (temp->left != nullptr)
            temp = temp->left;
        t->key = temp->key;
        t->data = temp->data;
        t->right = removeHelp(t->right, t->key);
    }
    else
    {
        if (t->left != nullptr)
        {
            Node* temp = t->left;
            t->key = temp->key;
            t->data = temp->data;
            t->left = temp->left;
            t->right = temp->right;
            temp->left = nullptr;
            temp->right = nullptr;
            temp = nullptr;
            delete temp;
            size--;
        }
        else if (t->right != nullptr)
        {
            Node* temp = t->right;
            t->key = temp->key;
            t->data = temp->data;
            t->left = temp->left;
            t->right = temp->right;
            temp->left = nullptr;
            temp->right = nullptr;
            temp = nullptr;
            delete temp;
            size--;
        }
        else
        {
            delete t;
            size--;
            t = nullptr;
        }
    }

    return t;
}

template <class K, class D>
void myBST<K, D>::print()
{
    printHelper(this->root, 0);
}

template <class K, class D>
void myBST<K, D>::printHelper(Node* t, int lvl)
{
    if (t == nullptr)
        return;
    printHelper(t->right, lvl + 1);
    for (int i = 0; i < lvl * 3; i++)
    {
        std::cout << "-";
    }
    //std::cout << std::endl;
    std::cout << t->key << " " << std::endl;
    printHelper(t->left, lvl + 1);
    return;
}

template <class K, class D>
myBST<K, D>::it::it(const myBST<K, D>& srcBst)
{
    if (srcBst.size <= 0)
    {
        std::cout << "Tree is empty" << std::endl;
    }
    else
    {
        this->setNode(srcBst);
    }
}

template <class K, class D>
myBST<K, D>::it::~it()
{
}

template <class K, class D>
myBST<K, D>::rIt::rIt(const myBST<K, D>& srcBst)
{
    if (srcBst.size <= 0)
    {
        std::cout << "Tree is empty" << std::endl;
    }
    else
    {
        this->setNode(srcBst);
    }
}

template <class K, class D>
myBST<K, D>::rIt::~rIt()
{
}

template <class K, class D>
myBST<K, D>::baseIT::baseIT()
{
}

template <class K, class D>
myBST<K, D>::baseIT::~baseIT()
{
}

template <class K, class D>
void myBST<K, D>::baseIT::setNodeHelp(Node* t)
{
}

template <class K, class D>
void myBST<K, D>::it::setNode(const myBST<K, D>& srcBst)
{
    setNodeHelp(srcBst.root);
    if (nodes.size() != 0)
        this->cur = nodes[0];
    else
        std::cout << "Iterator is not set" << std::endl;
}

template <class K, class D>
void myBST<K, D>::it::setNodeHelp(Node* t)
{
    if (t != nullptr)
    {
        setNodeHelp(t->left);
        this->nodes.push_back(t);
        setNodeHelp(t->right);
    }
}

template <class K, class D>
void myBST<K, D>::rIt::setNode(const myBST<K, D>& srcBst)
{
    setNodeHelp(srcBst.root);
}

template <class K, class D>
void myBST<K, D>::rIt::setNodeHelp(Node* t)
{
    if (t != nullptr)
    {
        setNodeHelp(t->right);
        this->nodes.push_back(t);
        setNodeHelp(t->left);
    }
}

template <class K, class D>
typename myBST<K, D>::it& ::myBST<K, D>::it::operator++()
{
    if (nodes.size() <= 0)
        throw std::runtime_error("Tree is empty");
    
    if (this->idx + 1 >= 0 && this->idx + 1 < nodes.size())
    {
        cur = nodes[idx + 1];
        idx++;
    }
    else
        std::cout << "Beyond the edge" << std::endl;

    return *this;
}

template <class K, class D>
typename myBST<K, D>::it& ::myBST<K, D>::it::operator--()
{
    if (nodes.size() <= 0)
        throw std::runtime_error("Tree is empty");

    if (this->idx - 1 < nodes.size() && this->idx - 1 >= 0) {
        cur = nodes[idx - 1];
        idx--;
    }
    else
        std::cout << "Beyond the edge" << std::endl;

    return *this;
}

template <class K, class D>
void myBST<K, D>::it::begin()
{
    if (nodes.size() != 0) {
        this->cur = nodes[0];
        this->idx = 0;
    }
    else
        std::cout << "Iterator is not set" << std::endl;
}

template <class K, class D>
void myBST<K, D>::it::end()
{
    idx = nodes.size();
    std::cout << "Iterator is not set(like it is supposed to be)" << std::endl;
}

template <class K, class D>
bool myBST<K, D>::it::operator==(const it& src)
{
    {
        if (src == nullptr)
        {
            if (cur == nullptr)
                return true;
            else
                return false;
        }
        else
        {
            if (cur == nullptr)
                return true;
            else
                return cur->key != src->key;
        }
    }
}

template <class K, class D>
bool myBST<K, D>::it::operator!=(const it& src)
{
    return !(*this == src);
}

template <class K, class D>
typename myBST<K, D>::Node* ::myBST<K, D>::it::operator*()
{
    return cur;
}

template <class K, class D>
typename myBST<K, D>::rIt& ::myBST<K, D>::rIt::operator++()
{
    if (nodes.size() <= 0)
        throw std::runtime_error("Tree is empty");

    if (this->idx + 1 >= 0 && this->idx + 1 < nodes.size())
    {
        cur = nodes[idx + 1];
        idx++;
    }
    else
        std::cout << "Beyond the edge" << std::endl;

    return *this;
}

template <class K, class D>
typename myBST<K, D>::rIt& ::myBST<K, D>::rIt::operator--()
{
    if (nodes.size() <= 0)
        throw std::runtime_error("Tree is empty");

    if (this->idx - 1 < nodes.size() && this->idx - 1 >= 0) {
        cur = nodes[idx - 1];
        idx--;
    }
    else
        std::cout << "Beyond the edge" << std::endl;

    return *this;
}

template <class K, class D>
void myBST<K, D>::rIt::rBegin()
{
    if (nodes.size() != 0) {
        this->cur = nodes[0];
        this->idx = 0;
    }
    else
        std::cout << "Iterator is not set" << std::endl;
}

template <class K, class D>
void myBST<K, D>::rIt::rEnd()
{
    this->idx = nodes.size();
    std::cout << "Iterator is not set(like it is supposed to be)" << std::endl;
}

template <class K, class D>
bool myBST<K, D>::rIt::operator==(const rIt& src)
{
    {
        if (src == nullptr)
        {
            if (cur == nullptr)
                return true;
            else
                return false;
        }
        else
        {
            if (cur == nullptr)
                return true;
            else
                return cur->key != src->key;
        }
    }
}

template <class K, class D>
bool myBST<K, D>::rIt::operator!=(const rIt& src)
{
    return !(*this == src);
}

template <class K, class D>
typename myBST<K, D>::Node* ::myBST<K, D>::rIt::operator*()
{
    if(cur == nullptr)
        std::cout << "Beyond the edge" << std::endl;

    return cur;
}
#pragma once
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

enum class Exception
{
    NOT_IN_TREE,            // �������� ��� � ������. ����� ���������� ��� ������ ��� �������� ��������, � ����� ��� ������������� ������� ��� ��������� ���������� ����� ����
    ALREADY_IN_TREE,        // ������� ��� ������������ � ������. ����� ���������� ��� ���������� �������� � ������
    ITERATOR_IS_NOT_INSTALL // �������� �� ����������. ����� ���������� ��� �������������, ������������� ��� ������������� ������� ��� ��������� ���������� 
};

template <class Key_T, class Data_T>
class Tree23
{

    // �����-������������ ����� ����� 2-3 ������
    enum class Node_Type
    {
        LEAF,           // ����
        INTERNAL_NODE   // ���������� ����
    };

    // ����������� ����� - ���� 2-3-������
    class Node
    {
    public:
        // �������� ��� ����
        virtual Node_Type getNodeType() const = 0;

        // ����������� ���������� ��� ����������� ������������ ������ �� �������� �����
        virtual ~Node() {};

    };

    // ����� �����
    class Leaf : public Node
    {
    public:
        Key_T key;      // ����
        Data_T data;    // ������

        // ����������� ����� � �����������
        Leaf(const Key_T& key, const Data_T& data) : key(key), data(data) {}

        // �������� ��� ����
        virtual Node_Type getNodeType() const override { return Node_Type::LEAF; }

    };

    // ����� ����������� ����
    class InternalNode : public Node
    {
    public:
        Key_T key2, key3;           // ��������� ����������� ������ � 2 � 3 �����������
        Node* son1, * son2, * son3; // ��������� �� ��� �������: �� �������� � ��������

        // ����������� ����������� ���� �� ���������
        InternalNode() : key2(0), key3(0), son1(nullptr), son2(nullptr), son3(nullptr) {}

        // �������� ��� ����
        virtual Node_Type getNodeType() const override { return Node_Type::INTERNAL_NODE; }

    };

    int size;           // ���������� ��������� � ������
    InternalNode* root; // ��������� �� ������ ������
    int last_op;        // ���������� ��������� �� ��������� ��������

    // ���������� ��������� ������ ���� � 2-3 ������
    Leaf* _search(const Key_T& key, const InternalNode* cur);

    // ���������� ��������� ����������� 2-3 ������
    InternalNode* _copy(const InternalNode* src);

    // ���������� ��������� ������� 2-3 ������
    void _clear(InternalNode* root);

    // ���������� ��������� ������� �������� � 2-3 ������
    bool _insert(const Key_T& key, const Data_T& data);

    // ��������������� ������� ������� ��� �������� - _insert
    bool _insert1(Node* cur, Leaf* lf, Node*& nroot, Key_T& kroot);

    // ���������� ��������� �������� �������� �� 2-3 ������
    bool _delete(const Key_T& key);

    // ��������������� ������� �������� ��� �������� - _delete
    bool _delete1(InternalNode* cur, const Key_T& key, Leaf*& low1, bool& one_son);

    // ���������� ��������� ������ 2-3 ������ � ������� � ����������� �����
    void _print(const Node* root, int level = 0) const;

    // �������� ���������� ������ �������� cur. ����� ��������� ����� ���� ����� ������� ����
    InternalNode* _getLeftParent(const Leaf* lf) const;

    // �������� ���������� ������� �������� cur. ������ ��������� ����� ���� ����� ������� ����
    InternalNode* _getRightParent(const Leaf* lf) const;

    // �������� ���� � ����������� ������ � ������ � ������ root
    Leaf* _getMin(Node* root) const;

    // �������� ���� � ������������ ������ � ������ � ������ root
    Leaf* _getMax(Node* root) const;

public:

    // ����� ������� ���������
    class Iterator
    {

        Tree23* tree;   // ��������� �� ������� ������
        Leaf* cur;      // ������� ����, �� ������� ���������� ��������

        // private-����������� � �����������
        Iterator(Tree23& tree, Leaf* cur);

    public:

        friend class Tree23;

        // ����������� ������� ��������� �� ���������
        Iterator() : tree(nullptr), cur(nullptr) {}

        // ����������� ������� ��������� � ���������� tree - ������� �� 2-3 ������
        Iterator(Tree23& tree) : tree(&tree), cur(nullptr) {}

        // ����������� ������� ��������� � �����������: tree - ������ �� 2-3 ������, key - ����
        Iterator(Tree23& tree, const Key_T& key);

        // ����������� ����������� ������� ���������
        Iterator(const Iterator& iter) : tree(iter.tree), cur(iter.cur) {}

        // �������� "���������������" - ��������� �������� �������� ���� ������ �� ������
        Key_T& operator*() const;

        // �������� ����������� ����������������� ������� ���������
        Iterator& operator++();

        // �������� ����������� ����������������� ������� ���������
        Iterator& operator--();

        // �������� ������������ ����������������� ������� ���������
        Iterator operator++(int) { Iterator temp = *this; ++(*this); return temp; }

        // �������� ������������ ����������������� ������� ���������
        Iterator operator--(int) { Iterator temp = *this; --(*this); return temp; }

        // �������� ������������ ������� ���������
        Iterator& operator=(const Iterator& iter) { this->tree = iter.tree; this->cur = iter.cur; return *this; }

        // �������� ��������� ������ ����������
        bool operator==(const Iterator& iter) const { return this->tree == iter.tree && this->cur == iter.cur; }

        // �������� ����������� ������ ����������
        bool operator!=(const Iterator& iter) const { return !(*this == iter); }

        // ���������� �� ��������
        bool is_established() const { return this->cur != nullptr; }

    };

    // ����� ��������� ���������
    class RIterator
    {

        Tree23* tree;   // ��������� �� ������� ������
        Leaf* cur;      // ������� ����, �� ������� ���������� ��������

        // private-����������� � �����������
        RIterator(Tree23& tree, Leaf* cur);

    public:

        friend class Tree23;

        // ����������� ��������� ��������� �� ���������
        RIterator() : tree(nullptr), cur(nullptr) {}

        // ����������� ��������� ��������� � ���������� tree - ������� �� BST-������
        RIterator(Tree23& tree) : tree(&tree), cur(nullptr) {}

        // ����������� ��������� ��������� � �����������: tree - ������ �� BST-������, key - ����
        RIterator(Tree23& tree, const Key_T& key);

        // ����������� ����������� ��������� ���������
        RIterator(const RIterator& riter) : tree(riter.tree), cur(riter.cur) {}

        // �������� "���������������" - ��������� �������� �������� ���� ������ �� ������
        Key_T& operator*() const;

        // �������� ����������� ����������������� ��������� ���������
        RIterator& operator++();

        // �������� ����������� ����������������� ��������� ���������
        RIterator& operator--();

        // �������� ������������ ����������������� ��������� ���������
        RIterator operator++(int) { RIterator temp = *this; ++(*this); return temp; }

        // �������� ������������ ����������������� ��������� ���������
        RIterator operator--(int) { RIterator temp = *this; --(*this); return temp; }

        // �������� ������������ ��������� ���������
        RIterator& operator=(const RIterator& riter) { this->tree = riter.tree; this->cur = riter.cur; return *this; }

        // �������� ��������� �������� ����������
        bool operator==(const RIterator& riter) const { return this->tree == riter.tree && this->cur == riter.cur; }

        // �������� ����������� �������� ����������
        bool operator!=(const RIterator& riter) const { return !(*this == riter); }

        // ���������� �� ��������
        bool is_established() const { return this->cur != nullptr; }

    };

    // ����������� 2-3 ������ �� ���������
    Tree23() : root(nullptr), size(0), last_op(0) {}

    // ����������� ����������� 2-3 ������
    Tree23(const Tree23& tree) : root(nullptr), size(tree.size), last_op(0) { this->root = _copy(tree.root); }

    // ���������� 2-3 ������
    ~Tree23() { clear(); }

    // �������� ������� � �������� �� �����
    Data_T& operator[](const Key_T& key) { return getItemByKey(key); }

    // �������� ������������ ��������
    Tree23& operator=(const Tree23& tree) { clear(); this->root = _copy(tree.root); this->size = tree.size; return *this; }

    // �������� ������ ������
    int getSize() const { return this->size; }

    // �������� ���������� ��������� �� ��������� ��������
    int getLastOp() const { return this->last_op; }

    // ���������� ���������� ��������� �� ��������� ��������
    void setLastOp(int value) { this->last_op = value; }

    // ������ �������� ������ �� �����
    Data_T& getItemByKey(const Key_T& key);

    // ������ �������� �� ��������� �����
    void setItemByKey(const Key_T& key, const Data_T& data) { getItemByKey(key) = data; }

    // �������� ������ �� �������
    bool isEmpty() const { return this->size == 0; }

    // ������� ������
    void clear() { _clear(this->root); this->root = nullptr; this->size = 0; }

    // �������� ������� � �������� ������ � ������
    void append(const Key_T& key, const Data_T& data);

    // ������� ������� � �������� ������
    void del(const Key_T& key);

    // ������� ������ � ����������� �����
    void print() { _print(this->root); }

    // ������� ������ � ����� �������: ����-��������
    void printAsDict();

    // ��������� ��������� �� ��������� ������� ������
    Iterator begin() { return Iterator(*this, _getMin(this->root)); }

    // ��������� ��������� �� ������� ����� ��������� ��������
    Iterator end() { return Iterator(*this); }

    // ��������� ��������� �������� � ����� ������
    RIterator rbegin() { return RIterator(*this, _getMax(this->root)); }

    // ��������� ��������� ��������� ����� ������ ��������� ������
    RIterator rend() { return RIterator(*this); }

};

//���������� ������� ������� ���������
template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::Iterator::Iterator(Tree23& tree, Leaf* lf)
{
    this->tree = &tree;
    this->cur = lf;
    if (!is_established())
        throw Exception::NOT_IN_TREE;
}

template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::Iterator::Iterator(Tree23& tree, const Key_T& key)
{
    this->tree = &tree;
    if ((this->cur = this->tree->_search(key, this->tree->root)) == nullptr)
        throw Exception::NOT_IN_TREE;
}

template <class Key_T, class Data_T> typename Key_T& Tree23<Key_T, Data_T>::Iterator::operator*() const
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    return this->cur->key;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator++()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // ���� ���������� ������ �� �������� � ������� ���� ���������� �� �� ������ �����
    InternalNode* par = this->tree->_getLeftParent(this->cur);

    if (par == nullptr) // ���� ������ ������ �� �������, ������ ���� ������� ����� ����������
        this->cur = nullptr;
    else // ����� ��������� � ���������� ��������
    {
        // ���� ���� � ������ ���� ��������, �� ��������� � ������������ �������� ������� ����
        if (this->cur->key < par->key2)
            this->cur = this->tree->_getMin(par->son2);
        // ���� �� ���� � ���������� ����, �� ��������� � ������������ �������� �������� ����
        else
            this->cur = this->tree->_getMin(par->son3);
    }

    return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator--()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // ���� ���������� ������ �� �������� � ������� ���� ���������� �� �� ����� �����
    InternalNode* par = this->tree->_getRightParent(this->cur);

    if (par == nullptr) // ���� ������ ������ �� �������, ������ ���� ������� ����� ����������
        this->cur = nullptr;
    else // ����� ��������� � ����������� ��������
    {
        // ���� ���� � ������� ���� ��������, �� ��������� � ������������� �������� ������� ����
        if (par->son3 != nullptr && this->cur->key >= par->key3)
            this->cur = this->tree->_getMax(par->son2);
        // ���� ���� �� ������ ���� ��������, �� ��������� � ������������� �������� ������� ����
        else
            this->cur = this->tree->_getMax(par->son1);
    }

    return *this;
}

//���������� ������� ��������� ���������
template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::RIterator::RIterator(Tree23& tree, Leaf* lf)
{
    this->tree = &tree;
    this->cur = lf;
    if (!is_established())
        throw Exception::NOT_IN_TREE;
}

template <class Key_T, class Data_T> Tree23<Key_T, Data_T>::RIterator::RIterator(Tree23& tree, const Key_T& key)
{
    this->tree = &tree;
    if ((this->cur = this->tree->_search(key, this->tree->root)) == nullptr)
        throw Exception::NOT_IN_TREE;
}

template <class Key_T, class Data_T> typename Key_T& Tree23<Key_T, Data_T>::RIterator::operator*() const
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    return this->cur->key;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator++()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // ���� ���������� ������ �� �������� � ������� ���� ���������� �� �� ����� �����
    InternalNode* par = this->tree->_getRightParent(this->cur);

    if (par == nullptr) // ���� ������ ������ �� �������, ������ ���� ������� ����� ����������
        this->cur = nullptr;
    else // ����� ��������� � ����������� ��������
    {
        // ���� ���� � ������� ���� ��������, �� ��������� � ������������� �������� ������� ����
        if (par->son3 != nullptr && this->cur->key >= par->key3)
            this->cur = this->tree->_getMax(par->son2);
        // ���� ���� �� ������ ���� ��������, �� ��������� � ������������� �������� ������� ����
        else
            this->cur = this->tree->_getMax(par->son1);
    }

    return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator--()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // ���� ���������� ������ �� �������� � ������� ���� ���������� �� �� ������ �����
    InternalNode* par = this->tree->_getLeftParent(this->cur);

    if (par == nullptr) // ���� ������ ������ �� �������, ������ ���� ������� ����� ����������
        this->cur = nullptr;
    else // ����� ��������� � ���������� ��������
    {
        // ���� ���� � ������ ���� ��������, �� ��������� � ������������ �������� ������� ����
        if (this->cur->key < par->key2)
            this->cur = this->tree->_getMin(par->son2);
        // ���� �� ���� � ���������� ����, �� ��������� � ������������ �������� �������� ����
        else
            this->cur = this->tree->_getMin(par->son3);
    }

    return *this;
}

// private-������:
template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_search(const Key_T& key, const InternalNode* cur)
{
    this->last_op++;

    // ������, ���� ��������� �����

    if (cur == nullptr)
        return nullptr;

    // ������, ���� ������� ��������� - �����

    if (cur->son1->getNodeType() == Node_Type::LEAF)
    {
        // ��������� ���� ������� �� ���������� ����� � key

        if (dynamic_cast<Leaf*>(cur->son1)->key == key)
            return dynamic_cast<Leaf*>(cur->son1);
        else if (cur->son2 != nullptr && dynamic_cast<Leaf*>(cur->son2)->key == key)
            return dynamic_cast<Leaf*>(cur->son2);
        else if (cur->son3 != nullptr && dynamic_cast<Leaf*>(cur->son3)->key == key)
            return dynamic_cast<Leaf*>(cur->son3);
        else
            return nullptr;
    }

    // ������, ���� ������� ��������� - ���������� ����

    if (key < cur->key2) // ������ �������� � ������� ����
        return _search(key, dynamic_cast<InternalNode*>(cur->son1));
    else if (cur->son3 != nullptr && key >= cur->key3) // ������ �������� � �������� ����
        return _search(key, dynamic_cast<InternalNode*>(cur->son3));
    else // ������ �������� � ������� ����
        return _search(key, dynamic_cast<InternalNode*>(cur->son2));
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_copy(const InternalNode* src)
{
    // ������, ���� ����� ��������� src ���
    if (src == nullptr)
        return nullptr;

    // ������, ���� ������ ��������� src ����

    InternalNode* inode = new InternalNode();
    inode->key2 = src->key2;
    inode->key3 = src->key3;

    // ������, ���� ������� ����� src - �����
    if (src->son1->getNodeType() == Node_Type::LEAF)
    {
        inode->son1 = new Leaf(dynamic_cast<Leaf*>(src->son1)->key, dynamic_cast<Leaf*>(src->son1)->data);
        if (src->son2 != nullptr)
            inode->son2 = new Leaf(dynamic_cast<Leaf*>(src->son2)->key, dynamic_cast<Leaf*>(src->son2)->data);
        if (src->son3 != nullptr)
            inode->son3 = new Leaf(dynamic_cast<Leaf*>(src->son3)->key, dynamic_cast<Leaf*>(src->son3)->data);
    }
    // ������, ���� ������� ����� src - ���������� ����
    else
    {
        inode->son1 = _copy(dynamic_cast<InternalNode*>(src->son1));
        inode->son2 = _copy(dynamic_cast<InternalNode*>(src->son2));
        inode->son3 = _copy(dynamic_cast<InternalNode*>(src->son3));
    }

    return inode;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::_clear(InternalNode* root)
{
    // ���� ���������/������ � ��� ������, �� ������ �� ������
    if (root == nullptr)
        return;

    // ������, ���� ������� ��������� - �����
    if (root->son1->getNodeType() == Node_Type::LEAF)
    {
        delete root->son1;
        if (root->son2 != nullptr)
            delete root->son2;
        if (root->son3 != nullptr)
            delete root->son3;
        delete root;
    }
    // ������, ���� ������� ��������� - ���������� ����
    else
    {
        _clear(dynamic_cast<InternalNode*>(root->son1));
        _clear(dynamic_cast<InternalNode*>(root->son2));
        _clear(dynamic_cast<InternalNode*>(root->son3));
        delete root;
    }
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_insert(const Key_T& key, const Data_T& data)
{
    Leaf* lf = new Leaf(key, data); // ����� ������ ����, ���� ���������� �� ���������, �� �� ����� �����

    if (this->root == nullptr) // ������, ���� ��������� ��� ���
    {
        this->root = new InternalNode();
        this->root->son1 = lf;
        this->size++;
        this->last_op++;

        return true;
    }

    if (this->root->son2 == nullptr) // ������, ���� ��� ������� ���� �������
    {
        Leaf* son1 = dynamic_cast<Leaf*>(this->root->son1);

        this->last_op++;

        // � ����������� �� �������� ����� key ��������� ��� �� ��������� ����� �������
        if (key < son1->key)
        {
            this->root->son2 = son1;
            this->root->son1 = lf;
            this->root->key2 = son1->key;
            this->size++;

            return true;
        }
        else if (key == son1->key)
        {
            delete lf;

            return false;
        }
        else
        {
            this->root->son2 = lf;
            this->root->key2 = lf->key;
            this->size++;

            return true;
        }
    }

    Node* nroot = nullptr;  // ��������� �� ��������� ����� ������
    Key_T kroot;            // ����������� ���� � ��������� ����� �����
    bool is_inserted;

    is_inserted = _insert1(this->root, lf, nroot, kroot); // ��������� ������ ������� � ������

    if (is_inserted == false)
    {
        delete lf;

        return false;
    }

    this->size++;

    if (nroot != nullptr) // ���� ��� ������ �������� ������ ������������ ��������� �����
    {
        // �� ������ ������ ����� �������� �������, ��������� �������� ����� ������ ��������� ������(������)
        // � ����� �������� ��� ������(������ ���)
        InternalNode* temp = this->root;

        this->root = new InternalNode();
        this->root->son1 = temp;
        this->root->son2 = nroot;
        this->root->key2 = kroot;

        this->last_op++;
    }

    return true;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_insert1(Node* cur, Leaf* lf, Node*& nroot, Key_T& kroot)
{
    // cur - ������� ������ 2-3 ������/���������
    // lf - ����� ��������� ����
    // nroot - �������� ��������. ������ �� ����� �������� ������ ������������ cur
    // kroot - �������� ��������. ����������� ���� ��������� � ������ nroot

    nroot = nullptr;
    this->last_op++;

    // ������, ���� cur - ����
    if (cur->getNodeType() == Node_Type::LEAF)
    {
        if (dynamic_cast<Leaf*>(cur)->key == lf->key) // ������, ���� ������� � ����� ������ ��� ���� � ������
            return false;
        else
        {
            nroot = lf; // � ���� �������, ��������� � nroot ����� ���� lf
            if (dynamic_cast<Leaf*>(cur)->key < lf->key)
                kroot = lf->key; // ���� ���� ������������ �������� ������ ��� � cur, �� � kroot ��������� ��� ����
            else
            {
                // ����� ������������ ��� ����: � cur ���������� ����������� �������, � � lf - ������� ������� cur

                Key_T key_temp = dynamic_cast<Leaf*>(cur)->key;
                Data_T data_temp = dynamic_cast<Leaf*>(cur)->data;

                kroot = key_temp;
                dynamic_cast<Leaf*>(cur)->key = lf->key;
                dynamic_cast<Leaf*>(cur)->data = lf->data;
                lf->key = key_temp;
                lf->data = data_temp;
            }

            return true;
        }
    }

    // ������, ���� cur - ���������� ����

    unsigned short child = 0;                           // ����� ����������� ����
    bool is_inserted = false;                           // ���� �� �������
    Node* tbk = nullptr;                                // �������� ���� ��� ���������� ���� ���� cur
    Key_T lbk = dynamic_cast<InternalNode*>(cur)->key2; // ����������� ���� � ��������� � ������ tbk

    // ������������ ������� � ��������� cur

    // ������ ������� � ������ ������� ����
    if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
    {
        child = 1;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son1, lf, tbk, lbk);
    }
    // ������ ������� � ������ �������� ����
    else if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr && lf->key >= dynamic_cast<InternalNode*>(cur)->key3)
    {
        child = 3;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son3, lf, tbk, lbk);
    }
    // ������ ������� � ������ ������� ����
    else
    {
        child = 2;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son2, lf, tbk, lbk);
    }

    // ������, ���� ������� �������� ������ ������� 
    // � ��� ������ �������� ���� ��� ���� cur, � ��������� �������� ��� �������� �������
    if (is_inserted && tbk != nullptr)
    {
        if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr) // ������, ���� � ���� ������ ��� ����
        {
            if (child == 2) // ������, ���� ������� �������������� �� ������ �����
            {
                dynamic_cast<InternalNode*>(cur)->son3 = tbk;
                dynamic_cast<InternalNode*>(cur)->key3 = lbk;
            }
            else // ������, ���� ������� �������������� � ������ �����
            {
                dynamic_cast<InternalNode*>(cur)->son3 = dynamic_cast<InternalNode*>(cur)->son2;
                dynamic_cast<InternalNode*>(cur)->key3 = dynamic_cast<InternalNode*>(cur)->key2;
                dynamic_cast<InternalNode*>(cur)->son2 = tbk;
                dynamic_cast<InternalNode*>(cur)->key2 = lbk;
            }
        }
        else // ������, ���� � ���� ���� ��� ������ ���
        {
            nroot = new InternalNode(); // ������ �������� ���� ��� cur
            this->last_op++;

            if (child == 3) // ������, ���� ������� �������������� � ������ �����
            {
                dynamic_cast<InternalNode*>(nroot)->son1 = dynamic_cast<InternalNode*>(cur)->son3;
                dynamic_cast<InternalNode*>(nroot)->son2 = tbk;
                dynamic_cast<InternalNode*>(nroot)->key2 = lbk;
                kroot = dynamic_cast<InternalNode*>(cur)->key3;
            }
            else // ������, ���� ������� �������������� � ������ ��� ������ �����
            {
                dynamic_cast<InternalNode*>(nroot)->son2 = dynamic_cast<InternalNode*>(cur)->son3;
                dynamic_cast<InternalNode*>(nroot)->key2 = dynamic_cast<InternalNode*>(cur)->key3;

                if (child == 2) // ������, ���� ������� �������������� �� ������ �����
                {
                    dynamic_cast<InternalNode*>(nroot)->son1 = tbk;
                    kroot = lbk;
                }
                else // ������, ���� ������� �������������� � ������ �����
                {
                    dynamic_cast<InternalNode*>(nroot)->son1 = dynamic_cast<InternalNode*>(cur)->son2;
                    dynamic_cast<InternalNode*>(cur)->son2 = tbk;
                    kroot = dynamic_cast<InternalNode*>(cur)->key2;
                    dynamic_cast<InternalNode*>(cur)->key2 = lbk;
                }
            }

            dynamic_cast<InternalNode*>(cur)->son3 = nullptr;
        }
    }

    return is_inserted;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_delete(const Key_T& key)
{
    Leaf* tmin;
    bool is_one;
    bool is_deleted;

    // ������, ���� ������ ������
    if (this->root == nullptr)
    {
        this->last_op++;

        return false;
    }

    // ������, ���� � ������ ���� �������
    if (this->root->son2 == nullptr)
    {
        this->last_op++;

        if (dynamic_cast<Leaf*>(this->root->son1)->key == key)
        {
            delete this->root->son1;
            delete this->root;
            this->root = nullptr;
            this->size--;

            return true;
        }
        else
            return false;
    }

    // ���� ������
    is_deleted = _delete1(this->root, key, tmin, is_one);

    if (is_deleted == true)
    {
        this->size--;

        // ���� � ������, ����� �������� ������� ���� �������, � ��� ���� ������ ��� ����� - ���������� ����
        if (is_one == true && this->root->son1->getNodeType() == Node_Type::INTERNAL_NODE)
        {
            InternalNode* temp = dynamic_cast<InternalNode*>(this->root->son1);

            delete this->root;
            this->root = temp;
            this->last_op++;
        }
    }

    return is_deleted;
}

template <class Key_T, class Data_T> typename bool Tree23<Key_T, Data_T>::_delete1(InternalNode* cur, const Key_T& key, Leaf*& low1, bool& one_son)
{
    // cur - ������� ������ ������/���������
    // key - ���� ���������� ��������
    // low1 - ������������ ����� ���� � ����������� ������ � ������ ���������
    // one_son - ������������ ������� ���� � ����� �����

    low1 = nullptr;
    one_son = false;
    this->last_op++;

    // ������, ���� ������� �������� ���� - �����
    if (cur->son1->getNodeType() == Node_Type::LEAF)
    {
        // � ����� ������ ���� ����� �������, ���, � �������� ��������� ���� � key
        // ���� ��� ������, �� ������� ��� � ������� ������� ������� � ����
        // ���� �� ������, �� ���������� false

        if (dynamic_cast<Leaf*>(cur->son1)->key == key)
        {
            delete cur->son1;
            cur->son1 = cur->son2;
            cur->son2 = cur->son3;
            cur->son3 = nullptr;
            cur->key2 = cur->key3;
        }
        else if (dynamic_cast<Leaf*>(cur->son2)->key == key)
        {
            delete cur->son2;
            cur->son2 = cur->son3;
            cur->son3 = nullptr;
            cur->key2 = cur->key3;
        }
        else if (cur->son3 != nullptr && dynamic_cast<Leaf*>(cur->son3)->key == key)
        {
            delete cur->son3;
            cur->son3 = nullptr;
        }
        else
            return false;

        // ����� ��������� �������� ���� � low1 ���������� ����� �������� ���� � ��������� � ������ cur
        // � �����, ���� ������� ������ ���� ���, �� ���������� ���� ������� ����� one_son
        low1 = dynamic_cast<Leaf*>(cur->son1);
        if (cur->son2 == nullptr)
            one_son = true;

        return true;
    }

    // ���� ������� �������� ����������� ������

    InternalNode* w = nullptr;  // ���������� ���, � ������� ����� ����������� ��������
    Leaf* low1_bk = nullptr;    // ���� � ����������� ������ � ������ ��������� ���������� ����
    unsigned short child = 0;   // ����� ���������� ����: 1, 2 ��� 3
    bool one_son_bk = false;    // ������� ����, ��� ��� �������� � ����� �� ���������� ����� 
                                // ������� ������ ���� ���-����

    // � ����������� �� ����� key, ��������� � ������-���� ����
    if (key < cur->key2)
    {
        child = 1;
        w = dynamic_cast<InternalNode*>(cur->son1);
    }
    else if (cur->son3 != nullptr && key >= cur->key3)
    {
        child = 3;
        w = dynamic_cast<InternalNode*>(cur->son3);
    }
    else
    {
        child = 2;
        w = dynamic_cast<InternalNode*>(cur->son2);
    }

    // ������� � ������-���� ����, �������� ������� ������� � ������ key � ��� ���������
    // ���� �������� � ������ key ��� � ������, �� ������� �� ������� � ���������� false
    if (_delete1(w, key, low1_bk, one_son_bk) == false)
        return false;

    // ������, ���� �������� ��������� �������

    // ������� � ������� ���� ������ �����, �.�. ���� �������� ������� �� ������ �����,
    // �� ���� ������������ �������� ���� �����, ���������� � ���� key2 �������� ����������� ���� cur
    // ���� �������� ������� �� ������� �����, �� ���������� � ���� key3
    low1 = low1_bk;
    if (low1_bk != nullptr)
    {
        if (child == 2)
        {
            cur->key2 = low1_bk->key;
            low1 = nullptr;
        }
        else if (child == 3)
        {
            cur->key3 = low1_bk->key;
            low1 = nullptr;
        }
    }

    // ���� ����� �������� �������� �� �������� ��������, ����� ���������� ���� ����� ����� ���� �������,
    // �� ������� �� ������� � ���������� true
    if (one_son_bk == false)
        return true;

    // ������, ���� � �����-���� �������� ���������� ���� cur ������� ������ ���� ���

    InternalNode* y = nullptr;
    InternalNode* z = nullptr;

    // ������������ ����� �������� cur � ����������� �� ����, ����� ���� � ��� �������

    // ������, ���� �������� ���� � ������� ���� cur � � ���� ������� ���� ���
    if (child == 1)
    {
        // w - ��� ������ ��� cur: w == cur->son1
        y = dynamic_cast<InternalNode*>(cur->son2); // y - ��� ������ ��� cur: y == cur->son2

        // ������, ���� � ������� ���� cur ���� ����������� ������ ���
        if (y->son3 != nullptr)
        {
            w->son2 = y->son1;
            w->key2 = cur->key2;
            cur->key2 = y->key2;
            y->son1 = y->son2;
            y->son2 = y->son3;
            y->key2 = y->key3;
            y->son3 = nullptr;
        }
        // ������, ���� � ������� ���� cur ��� ������������ �������� ����
        else
        {
            y->son3 = y->son2;
            y->key3 = y->key2;
            y->son2 = y->son1;
            y->key2 = cur->key2;
            y->son1 = w->son1;
            delete w;
            cur->son1 = cur->son2;
            cur->son2 = cur->son3;
            cur->key2 = cur->key3;
            cur->son3 = nullptr;
            if (cur->son2 == nullptr)
                one_son = true;
        }
    }
    // ������, ���� �������� ���� � ������� ���� cur, � � ���� ������� ���� ���
    else if (child == 2)
    {
        // w - ������ ��� cur: w == cur->son2
        y = dynamic_cast<InternalNode*>(cur->son1); // y - ��� ������ ��� cur: y == cur->son1

        // ������, ���� � ������� ���� cur ���� ����������� ������ ���
        if (y->son3 != nullptr)
        {
            w->son2 = w->son1;
            w->key2 = cur->key2;
            w->son1 = y->son3;
            y->son3 = nullptr;
            cur->key2 = y->key3;
        }
        // ������, ���� � ������� ���� cur ��� ������������ �������� ����
        else
        {
            z = dynamic_cast<InternalNode*>(cur->son3); // z - ������ ��� cur: z == cur->son3

            // ���� � cur ���� ������ ���, � � ���� ���� ����������� ������ ���
            if (z != nullptr && z->son3 != nullptr)
            {
                w->son2 = z->son1;
                w->key2 = cur->key3;
                cur->key3 = z->key2;
                z->son1 = z->son2;
                z->son2 = z->son3;
                z->key2 = z->key3;
                z->son3 = nullptr;
            }
            // ���� � cur ��� �������� ����, ��� �� ����, �� � ���� ��� ������������ �������� ����
            else
            {
                y->son3 = w->son1;
                y->key3 = cur->key2;
                delete w;
                cur->son2 = cur->son3;
                cur->key2 = cur->key3;
                cur->son3 = nullptr;

                if (cur->son2 == nullptr)
                    one_son = true;
            }
        }
    }
    // ������, ���� �������� ���� � �������� ���� cur, � � ���� ������� ���� ���
    else
    {
        // w - ������ ��� cur: w == cur->son3
        y = dynamic_cast<InternalNode*>(cur->son2); // ������ y - ������ ��� cur: y == cur->son2

        // ������, ���� � ������� ���� cur ���� ����������� ������ ��� 
        if (y->son3 != nullptr)
        {
            w->son2 = w->son1;
            w->key2 = cur->key3;
            w->son1 = y->son3;
            cur->key3 = y->key3;
            y->son3 = nullptr;
        }
        // ������, ���� � ������� ���� cur ��� ������������ �������� ����
        else
        {
            y->son3 = w->son1;
            y->key3 = cur->key3;
            cur->son3 = nullptr;
            delete w;
        }
    }

    return true;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::_print(const Node* root, int level) const
{
    // ���� ���������/������ ������, �� ������ �� �������
    if (root == nullptr)
        return;

    // ���� ������� ���� - ����, �� ������� ��� ����
    if (root->getNodeType() == Node_Type::LEAF)
    {
        for (int i = 0; i < level - 1; i++)
            cout << "  ";
        for (int i = 0; i < 6; i++)
            cout << "--";

        cout << dynamic_cast<const Leaf*>(root)->key << endl << endl;
    }
    // ���� ������� ���� - ���������� ����, �� ������� ������� ��������� �������� ����, ����� �������,
    // ����� ����� ������� ����������� ����, �, �������, ������� ����
    else
    {
        if (dynamic_cast<const InternalNode*>(root)->son3 != nullptr)
            _print(dynamic_cast<const InternalNode*>(root)->son3, level + 1);

        if (dynamic_cast<const InternalNode*>(root)->son2 != nullptr)
            _print(dynamic_cast<const InternalNode*>(root)->son2, level + 1);

        for (int i = 0; i < level - 1; i++)
            cout << "  ";
        if (level > 0)
            cout << "--";
        if (dynamic_cast<const InternalNode*>(root)->son2 != nullptr)
            cout << dynamic_cast<const InternalNode*>(root)->key2;
        else
            cout << "None";
        cout << "-";
        if (dynamic_cast<const InternalNode*>(root)->son3 != nullptr)
            cout << dynamic_cast<const InternalNode*>(root)->key3;
        else
            cout << "None";
        cout << endl << endl;

        _print(dynamic_cast<const InternalNode*>(root)->son1, level + 1);
    }
}


template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_getLeftParent(const Leaf* lf) const
{
    InternalNode* par = nullptr;    // ������������ ������������ ����
    Node* cur = this->root;         // ������� ����

    // ���� ���� lf � ��������� �� ��� ���������� ������ ��� ����������� �������� ��������� � par
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
    {
        // ������� � �������, �.�. ������ ����
        if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
        {
            par = dynamic_cast<InternalNode*>(cur);
            cur = dynamic_cast<InternalNode*>(cur)->son1;
        }
        // ������� � ������� ����
        else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || lf->key < dynamic_cast<InternalNode*>(cur)->key3)
        {
            // � par ������� cur � ��� ������, ���� ������ ��� - ����������
            if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr)
                par = dynamic_cast<InternalNode*>(cur);

            cur = dynamic_cast<InternalNode*>(cur)->son2;
        }
        // ������� � ��������, �.�. ������� ����
        else
            cur = dynamic_cast<InternalNode*>(cur)->son3;
    }

    return par;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_getRightParent(const Leaf* lf) const
{
    InternalNode* par = nullptr;    // ������������ ������������ ����
    Node* cur = this->root;         // ������� ����

    // ���� ���� lf � ��������� �� ��� ���������� ������� ��� ����������� �������� ��������� � par
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
    {
        // ������� � �������, �.�. ������ ����
        if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
            cur = dynamic_cast<InternalNode*>(cur)->son1;
        // ������� � ������� ����
        else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || lf->key < dynamic_cast<InternalNode*>(cur)->key3)
        {
            par = dynamic_cast<InternalNode*>(cur);
            cur = dynamic_cast<InternalNode*>(cur)->son2;
        }
        // ������� � ��������, �.�. ������� ����
        else
        {
            par = dynamic_cast<InternalNode*>(cur);
            cur = dynamic_cast<InternalNode*>(cur)->son3;
        }
    }

    return par;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_getMin(Node* root) const
{
    Node* cur = root; // ������� ���� ������

    // ��������� �� ����� ����� �� �����
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
        cur = dynamic_cast<InternalNode*>(cur)->son1;

    return dynamic_cast<Leaf*>(cur);
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_getMax(Node* root) const
{
    Node* cur = root; // ������� ���� ������

    // ��������� �� ������ ����� �� �����
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
    {
        if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr)
            cur = dynamic_cast<InternalNode*>(cur)->son3;
        else if (dynamic_cast<InternalNode*>(cur)->son2 != nullptr)
            cur = dynamic_cast<InternalNode*>(cur)->son2;
        else
            cur = dynamic_cast<InternalNode*>(cur)->son1;
    }

    return dynamic_cast<Leaf*>(cur);
}

// public-������:

template <class Key_T, class Data_T> typename Data_T& Tree23<Key_T, Data_T>::getItemByKey(const Key_T& key)
{
    this->last_op = 0;

    Leaf* temp = _search(key, this->root);

    if (temp == nullptr)
        throw Exception::NOT_IN_TREE;
    else
        return temp->data;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::append(const Key_T& key, const Data_T& data)
{
    this->last_op = 0;

    if (!_insert(key, data))
        throw Exception::ALREADY_IN_TREE;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::del(const Key_T& key)
{
    this->last_op = 0;

    if (!_delete(key))
        throw Exception::NOT_IN_TREE;
}

template <class Key_T, class Data_T> typename void Tree23<Key_T, Data_T>::printAsDict()
{
    if (this->size == 0)
        cout << "{}" << endl;
    else
    {
        Iterator iter = begin();

        cout << "{ ";
        for (int i = 0; i < this->size - 1; iter++, i++)
            cout << *iter << ": " << getItemByKey(*iter) << ", ";
        cout << *iter << ": " << getItemByKey(*iter) << " }" << endl;
    }
}
#pragma once
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

enum class Exception
{
    NOT_IN_TREE,            // Элемента нет в дереве. Может возникнуть при поиске или удалении элемента, а также при инициализации прямого или обратного итераторов через ключ
    ALREADY_IN_TREE,        // Элемент уже присутствует в дереве. Может возникнуть при добавлении элемента в дерево
    ITERATOR_IS_NOT_INSTALL // Итератор не установлен. Может возникнуть при разыменовании, инкрементации или декрементации прямого или обратного итераторов 
};

template <class Key_T, class Data_T>
class Tree23
{

    // Класс-перечисление типов узлов 2-3 дерева
    enum class Node_Type
    {
        LEAF,           // Лист
        INTERNAL_NODE   // Внутренний узел
    };

    // Абстрактный класс - узел 2-3-дерева
    class Node
    {
    public:
        // Получить тип узла
        virtual Node_Type getNodeType() const = 0;

        // Виртуальный деструктор для корректного освобождения памяти от дочерних узлов
        virtual ~Node() {};

    };

    // Класс листа
    class Leaf : public Node
    {
    public:
        Key_T key;      // Ключ
        Data_T data;    // Данные

        // Конструктор листа с параметрами
        Leaf(const Key_T& key, const Data_T& data) : key(key), data(data) {}

        // Получить тип узла
        virtual Node_Type getNodeType() const override { return Node_Type::LEAF; }

    };

    // Класс внутреннего узла
    class InternalNode : public Node
    {
    public:
        Key_T key2, key3;           // Дубликаты минимальных ключей в 2 и 3 поддеревьях
        Node* son1, * son2, * son3; // Указатели на трёх сыновей: от младшего к старшему

        // Конструктор внутреннего узла по умолчанию
        InternalNode() : key2(0), key3(0), son1(nullptr), son2(nullptr), son3(nullptr) {}

        // Получить тип узла
        virtual Node_Type getNodeType() const override { return Node_Type::INTERNAL_NODE; }

    };

    int size;           // Количество элементов в дереве
    InternalNode* root; // Указатель на корень дерева
    int last_op;        // Количество переборов на последней операции

    // Реализация алгоритма поиска узла в 2-3 дереве
    Leaf* _search(const Key_T& key, const InternalNode* cur);

    // Реализация алгоритма копирования 2-3 дерева
    InternalNode* _copy(const InternalNode* src);

    // Реализация алгоритма очистки 2-3 дерева
    void _clear(InternalNode* root);

    // Реализация алгоритма вставки элемента в 2-3 дерево
    bool _insert(const Key_T& key, const Data_T& data);

    // Вспомогательная функция вставки для основной - _insert
    bool _insert1(Node* cur, Leaf* lf, Node*& nroot, Key_T& kroot);

    // Реализация алгоритма удаления элемента из 2-3 дерева
    bool _delete(const Key_T& key);

    // Вспомогательная функция удаления для основной - _delete
    bool _delete1(InternalNode* cur, const Key_T& key, Leaf*& low1, bool& one_son);

    // Реализация алгоритма вывода 2-3 дерева в консоль в древовидной форме
    void _print(const Node* root, int level = 0) const;

    // Получить ближайшего левого родителя cur. Левым родителем может быть также средний узел
    InternalNode* _getLeftParent(const Leaf* lf) const;

    // Получить ближайшего правого родителя cur. Правым родителем может быть также средний узел
    InternalNode* _getRightParent(const Leaf* lf) const;

    // Получить лист с минимальным ключом в дереве с корнем root
    Leaf* _getMin(Node* root) const;

    // Получить лист с максимальным ключом в дереве с корнем root
    Leaf* _getMax(Node* root) const;

public:

    // Класс прямого итератора
    class Iterator
    {

        Tree23* tree;   // Указатель на текущее дерево
        Leaf* cur;      // Текущий лист, на который установлен итератор

        // private-конструктор с параметрами
        Iterator(Tree23& tree, Leaf* cur);

    public:

        friend class Tree23;

        // Конструктор прямого итератора по умолчанию
        Iterator() : tree(nullptr), cur(nullptr) {}

        // Конструктор прямого итератора с параметром tree - ссылкой на 2-3 дерево
        Iterator(Tree23& tree) : tree(&tree), cur(nullptr) {}

        // Конструктор прямого итератора с параметрами: tree - ссылка на 2-3 дерево, key - ключ
        Iterator(Tree23& tree, const Key_T& key);

        // Конструктор копирования прямого итератора
        Iterator(const Iterator& iter) : tree(iter.tree), cur(iter.cur) {}

        // Оператор "разыменовывания" - получение текущего элемента узла дерева по ссылке
        Key_T& operator*() const;

        // Оператор префиксного инкрементирования прямого итератора
        Iterator& operator++();

        // Оператор префиксного декрементирования прямого итератора
        Iterator& operator--();

        // Оператор постфиксного инкрементирования прямого итератора
        Iterator operator++(int) { Iterator temp = *this; ++(*this); return temp; }

        // Оператор постфиксного декрементирования прямого итератора
        Iterator operator--(int) { Iterator temp = *this; --(*this); return temp; }

        // Оператор присваивания прямого итератора
        Iterator& operator=(const Iterator& iter) { this->tree = iter.tree; this->cur = iter.cur; return *this; }

        // Оператор равенства прямых итераторов
        bool operator==(const Iterator& iter) const { return this->tree == iter.tree && this->cur == iter.cur; }

        // Оператор неравенства прямых итераторов
        bool operator!=(const Iterator& iter) const { return !(*this == iter); }

        // Установлен ли итератор
        bool is_established() const { return this->cur != nullptr; }

    };

    // Класс обратного итератора
    class RIterator
    {

        Tree23* tree;   // Указатель на текущее дерево
        Leaf* cur;      // Текущий лист, на который установлен итератор

        // private-конструктор с параметрами
        RIterator(Tree23& tree, Leaf* cur);

    public:

        friend class Tree23;

        // Конструктор обратного итератора по умолчанию
        RIterator() : tree(nullptr), cur(nullptr) {}

        // Конструктор обратного итератора с параметром tree - ссылкой на BST-дерево
        RIterator(Tree23& tree) : tree(&tree), cur(nullptr) {}

        // Конструктор обратного итератора с параметрами: tree - ссылка на BST-дерево, key - ключ
        RIterator(Tree23& tree, const Key_T& key);

        // Конструктор копирования обратного итератора
        RIterator(const RIterator& riter) : tree(riter.tree), cur(riter.cur) {}

        // Оператор "разыменовывания" - получение текущего элемента узла дерева по ссылке
        Key_T& operator*() const;

        // Оператор префиксного инкрементирования обратного итератора
        RIterator& operator++();

        // Оператор префиксного декрементирования обратного итератора
        RIterator& operator--();

        // Оператор постфиксного инкрементирования обратного итератора
        RIterator operator++(int) { RIterator temp = *this; ++(*this); return temp; }

        // Оператор постфиксного декрементирования обратного итератора
        RIterator operator--(int) { RIterator temp = *this; --(*this); return temp; }

        // Оператор присваивания обратного итератора
        RIterator& operator=(const RIterator& riter) { this->tree = riter.tree; this->cur = riter.cur; return *this; }

        // Оператор равенства обратных итераторов
        bool operator==(const RIterator& riter) const { return this->tree == riter.tree && this->cur == riter.cur; }

        // Оператор неравенства обратных итераторов
        bool operator!=(const RIterator& riter) const { return !(*this == riter); }

        // Установлен ли итератор
        bool is_established() const { return this->cur != nullptr; }

    };

    // Конструктор 2-3 дерева по умолчанию
    Tree23() : root(nullptr), size(0), last_op(0) {}

    // Конструктор копирования 2-3 дерева
    Tree23(const Tree23& tree) : root(nullptr), size(tree.size), last_op(0) { this->root = _copy(tree.root); }

    // Деструктор 2-3 дерева
    ~Tree23() { clear(); }

    // Оператор доступа к элементу по ключу
    Data_T& operator[](const Key_T& key) { return getItemByKey(key); }

    // Оператор присваивания деревьев
    Tree23& operator=(const Tree23& tree) { clear(); this->root = _copy(tree.root); this->size = tree.size; return *this; }

    // Получить размер дерева
    int getSize() const { return this->size; }

    // Получить количество переборов на последней операции
    int getLastOp() const { return this->last_op; }

    // Установить количество переборов на последней операции
    void setLastOp(int value) { this->last_op = value; }

    // Чтение элемента дерева по ключу
    Data_T& getItemByKey(const Key_T& key);

    // Запись элемента по заданному ключу
    void setItemByKey(const Key_T& key, const Data_T& data) { getItemByKey(key) = data; }

    // Проверка дерева на пустоту
    bool isEmpty() const { return this->size == 0; }

    // Очистка дерева
    void clear() { _clear(this->root); this->root = nullptr; this->size = 0; }

    // Добавить элемент с заданным ключом в дерево
    void append(const Key_T& key, const Data_T& data);

    // Удалить элемент с заданным ключом
    void del(const Key_T& key);

    // Вывести дерево в древовидной форме
    void print() { _print(this->root); }

    // Вывести дерево в форме словаря: ключ-значение
    void printAsDict();

    // Установка итератора на начальный элемент списка
    Iterator begin() { return Iterator(*this, _getMin(this->root)); }

    // Установка итератора на элемент после конечного элемента
    Iterator end() { return Iterator(*this); }

    // Установка обратного итератор в конец списка
    RIterator rbegin() { return RIterator(*this, _getMax(this->root)); }

    // Установка обратного итератора перед первым элементом списка
    RIterator rend() { return RIterator(*this); }

};

//Реализация методов прямого итератора
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

    // Ищем ближайшего предка от которого к данному узлу переходили НЕ по правой ветви
    InternalNode* par = this->tree->_getLeftParent(this->cur);

    if (par == nullptr) // Если такого предка не нашлось, значит ключ данного листа наибольший
        this->cur = nullptr;
    else // Иначе переходим к следующему элементу
    {
        // Если были в первом сыне родителя, то переходим к минимальному элементу второго сына
        if (this->cur->key < par->key2)
            this->cur = this->tree->_getMin(par->son2);
        // Если же были в серединном узле, то переходим к минимальному элементу третьего сына
        else
            this->cur = this->tree->_getMin(par->son3);
    }

    return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Iterator& Tree23<Key_T, Data_T>::Iterator::operator--()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // Ищем ближайшего предка от которого к данному узлу переходили НЕ по левой ветви
    InternalNode* par = this->tree->_getRightParent(this->cur);

    if (par == nullptr) // Если такого предка не нашлось, значит ключ данного листа наименьший
        this->cur = nullptr;
    else // Иначе переходим к предыдущему элементу
    {
        // Если были в третьем сыне родителя, то переходим к максимальному элементу второго сына
        if (par->son3 != nullptr && this->cur->key >= par->key3)
            this->cur = this->tree->_getMax(par->son2);
        // Если были во втором сыне родителя, то переходим к максимальному элементу первого сына
        else
            this->cur = this->tree->_getMax(par->son1);
    }

    return *this;
}

//Реализация методов обратного итератора
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

    // Ищем ближайшего предка от которого к данному узлу переходили НЕ по левой ветви
    InternalNode* par = this->tree->_getRightParent(this->cur);

    if (par == nullptr) // Если такого предка не нашлось, значит ключ данного листа наименьший
        this->cur = nullptr;
    else // Иначе переходим к предыдущему элементу
    {
        // Если были в третьем сыне родителя, то переходим к максимальному элементу второго сына
        if (par->son3 != nullptr && this->cur->key >= par->key3)
            this->cur = this->tree->_getMax(par->son2);
        // Если были во втором сыне родителя, то переходим к максимальному элементу первого сына
        else
            this->cur = this->tree->_getMax(par->son1);
    }

    return *this;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::RIterator& Tree23<Key_T, Data_T>::RIterator::operator--()
{
    if (!is_established())
        throw Exception::ITERATOR_IS_NOT_INSTALL;

    // Ищем ближайшего предка от которого к данному узлу переходили НЕ по правой ветви
    InternalNode* par = this->tree->_getLeftParent(this->cur);

    if (par == nullptr) // Если такого предка не нашлось, значит ключ данного листа наибольший
        this->cur = nullptr;
    else // Иначе переходим к следующему элементу
    {
        // Если были в первом сыне родителя, то переходим к минимальному элементу второго сына
        if (this->cur->key < par->key2)
            this->cur = this->tree->_getMin(par->son2);
        // Если же были в серединном узле, то переходим к минимальному элементу третьего сына
        else
            this->cur = this->tree->_getMin(par->son3);
    }

    return *this;
}

// private-методы:
template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_search(const Key_T& key, const InternalNode* cur)
{
    this->last_op++;

    // Случай, если поддерево пусто

    if (cur == nullptr)
        return nullptr;

    // Случай, если сыновья поддерева - листы

    if (cur->son1->getNodeType() == Node_Type::LEAF)
    {
        // Проверяем всех сыновей на совпадения ключа с key

        if (dynamic_cast<Leaf*>(cur->son1)->key == key)
            return dynamic_cast<Leaf*>(cur->son1);
        else if (cur->son2 != nullptr && dynamic_cast<Leaf*>(cur->son2)->key == key)
            return dynamic_cast<Leaf*>(cur->son2);
        else if (cur->son3 != nullptr && dynamic_cast<Leaf*>(cur->son3)->key == key)
            return dynamic_cast<Leaf*>(cur->son3);
        else
            return nullptr;
    }

    // Случай, если сыновья поддерева - внутренние узлы

    if (key < cur->key2) // Случай перехода к первому сыну
        return _search(key, dynamic_cast<InternalNode*>(cur->son1));
    else if (cur->son3 != nullptr && key >= cur->key3) // Случай перехода к третьему сыну
        return _search(key, dynamic_cast<InternalNode*>(cur->son3));
    else // Случай перехода к второму сыну
        return _search(key, dynamic_cast<InternalNode*>(cur->son2));
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_copy(const InternalNode* src)
{
    // Случай, если корня поддерева src нет
    if (src == nullptr)
        return nullptr;

    // Случай, если корень поддерева src есть

    InternalNode* inode = new InternalNode();
    inode->key2 = src->key2;
    inode->key3 = src->key3;

    // Случай, если сыновья корня src - листы
    if (src->son1->getNodeType() == Node_Type::LEAF)
    {
        inode->son1 = new Leaf(dynamic_cast<Leaf*>(src->son1)->key, dynamic_cast<Leaf*>(src->son1)->data);
        if (src->son2 != nullptr)
            inode->son2 = new Leaf(dynamic_cast<Leaf*>(src->son2)->key, dynamic_cast<Leaf*>(src->son2)->data);
        if (src->son3 != nullptr)
            inode->son3 = new Leaf(dynamic_cast<Leaf*>(src->son3)->key, dynamic_cast<Leaf*>(src->son3)->data);
    }
    // Случай, если сыновья корня src - внутренние узлы
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
    // Если поддерево/дерево и так пустое, то ничего не делаем
    if (root == nullptr)
        return;

    // Случай, если сыновья поддерева - листы
    if (root->son1->getNodeType() == Node_Type::LEAF)
    {
        delete root->son1;
        if (root->son2 != nullptr)
            delete root->son2;
        if (root->son3 != nullptr)
            delete root->son3;
        delete root;
    }
    // Случай, если сыновья поддерева - внутренние узлы
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
    Leaf* lf = new Leaf(key, data); // Сразу создаём лист, если добавление не произойдёт, то он будет удалён

    if (this->root == nullptr) // Случай, если элементов ещё нет
    {
        this->root = new InternalNode();
        this->root->son1 = lf;
        this->size++;
        this->last_op++;

        return true;
    }

    if (this->root->son2 == nullptr) // Случай, если уже имеется один элемент
    {
        Leaf* son1 = dynamic_cast<Leaf*>(this->root->son1);

        this->last_op++;

        // В зависимости от значения ключа key вставляем или не вставляем новый элемент
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

    Node* nroot = nullptr;  // Указатель на возможный новый корень
    Key_T kroot;            // Минимальный ключ в поддереве этого корня
    bool is_inserted;

    is_inserted = _insert1(this->root, lf, nroot, kroot); // Остальные случаи вставки в дерево

    if (is_inserted == false)
    {
        delete lf;

        return false;
    }

    this->size++;

    if (nroot != nullptr) // Если был создан братский корень относительно исходного корня
    {
        // То создаём корень более высокого порядка, сыновьями которого будут корень исходного дерева(первый)
        // и новый братский ему корень(второй сын)
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
    // cur - текущий корень 2-3 дерева/поддерева
    // lf - ранее созданный лист
    // nroot - выходной параметр. Ссылка на новый братский корень относительно cur
    // kroot - выходной параметр. Минимальный ключ поддерева с корнем nroot

    nroot = nullptr;
    this->last_op++;

    // Случай, если cur - лист
    if (cur->getNodeType() == Node_Type::LEAF)
    {
        if (dynamic_cast<Leaf*>(cur)->key == lf->key) // Случай, если элемент с таким ключом уже есть в дереве
            return false;
        else
        {
            nroot = lf; // В иных случаях, сохраняем в nroot новый узел lf
            if (dynamic_cast<Leaf*>(cur)->key < lf->key)
                kroot = lf->key; // Если ключ вставляемого элемента больше чем у cur, то в kroot сохраняем его ключ
            else
            {
                // Иначе переставляем эти узлы: в cur записываем вставляемый элемент, а в lf - текущий элемент cur

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

    // Случай, если cur - внутренний узел

    unsigned short child = 0;                           // Номер выбираемого сына
    bool is_inserted = false;                           // Была ли вставка
    Node* tbk = nullptr;                                // Братский узел для выбранного ниже сына cur
    Key_T lbk = dynamic_cast<InternalNode*>(cur)->key2; // Минимальный ключ в поддереве с корнем tbk

    // Осуществляем вставку в поддерево cur

    // Случай вставки в дерево первого сына
    if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
    {
        child = 1;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son1, lf, tbk, lbk);
    }
    // Случай вставки в дерево третьего сына
    else if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr && lf->key >= dynamic_cast<InternalNode*>(cur)->key3)
    {
        child = 3;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son3, lf, tbk, lbk);
    }
    // Случай вставки в корень второго сына
    else
    {
        child = 2;
        is_inserted = _insert1(dynamic_cast<InternalNode*>(cur)->son2, lf, tbk, lbk);
    }

    // Случай, если вставка элемента прошла успешно 
    // и был создан братский узел для сына cur, в поддерево которого был вставлен элемент
    if (is_inserted && tbk != nullptr)
    {
        if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr) // Случай, если у узла только два сына
        {
            if (child == 2) // Случай, если вставки осуществлялась во вторую ветвь
            {
                dynamic_cast<InternalNode*>(cur)->son3 = tbk;
                dynamic_cast<InternalNode*>(cur)->key3 = lbk;
            }
            else // Случай, если вставка осуществлялась в первую ветвь
            {
                dynamic_cast<InternalNode*>(cur)->son3 = dynamic_cast<InternalNode*>(cur)->son2;
                dynamic_cast<InternalNode*>(cur)->key3 = dynamic_cast<InternalNode*>(cur)->key2;
                dynamic_cast<InternalNode*>(cur)->son2 = tbk;
                dynamic_cast<InternalNode*>(cur)->key2 = lbk;
            }
        }
        else // Случай, если у узла есть ещё третий сын
        {
            nroot = new InternalNode(); // Создаём братский узел для cur
            this->last_op++;

            if (child == 3) // Случай, если вставка осуществлялась в третью ветвь
            {
                dynamic_cast<InternalNode*>(nroot)->son1 = dynamic_cast<InternalNode*>(cur)->son3;
                dynamic_cast<InternalNode*>(nroot)->son2 = tbk;
                dynamic_cast<InternalNode*>(nroot)->key2 = lbk;
                kroot = dynamic_cast<InternalNode*>(cur)->key3;
            }
            else // Случай, если вставка осуществлялась в первую или вторую ветвь
            {
                dynamic_cast<InternalNode*>(nroot)->son2 = dynamic_cast<InternalNode*>(cur)->son3;
                dynamic_cast<InternalNode*>(nroot)->key2 = dynamic_cast<InternalNode*>(cur)->key3;

                if (child == 2) // Случай, если вставка осуществлялась во вторую ветвь
                {
                    dynamic_cast<InternalNode*>(nroot)->son1 = tbk;
                    kroot = lbk;
                }
                else // Случай, если вставка осуществлялась в первую ветвь
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

    // Случай, если дерево пустое
    if (this->root == nullptr)
    {
        this->last_op++;

        return false;
    }

    // Случай, если в дереве один элемент
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

    // Иные случаи
    is_deleted = _delete1(this->root, key, tmin, is_one);

    if (is_deleted == true)
    {
        this->size--;

        // Если в дереве, после удаления остался один элемент, и при этом первый сын корня - внутренний узел
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
    // cur - текущий корень дерева/поддерева
    // key - ключ удаляемого элемента
    // low1 - возвращаемый адрес узла с минимальным ключом в первом поддереве
    // one_son - возвращаемый признак узла с одним сыном

    low1 = nullptr;
    one_son = false;
    this->last_op++;

    // Случай, если сыновья текущего узла - листы
    if (cur->son1->getNodeType() == Node_Type::LEAF)
    {
        // В таком случае ищем среди сыновей, тот, у которого совпадает ключ с key
        // Если сын найден, то удаляем его и смещаем старших сыновей к нему
        // Если не найден, то возвращаем false

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

        // После успешного удаления сына в low1 записываем адрес младшего сына в поддерево с корнем cur
        // И также, если остался только один сын, то возвращаем этот признак через one_son
        low1 = dynamic_cast<Leaf*>(cur->son1);
        if (cur->son2 == nullptr)
            one_son = true;

        return true;
    }

    // Если сыновья являются внутренними узлами

    InternalNode* w = nullptr;  // Выбираемый сын, в котором будет происходить удаление
    Leaf* low1_bk = nullptr;    // Узел с минимальный ключом в первом поддереве выбранного узла
    unsigned short child = 0;   // Номер выбранного сына: 1, 2 или 3
    bool one_son_bk = false;    // Признак того, что при удалении в одном из внутренних узлов 
                                // остался только один сын-лист

    // В зависимости от ключа key, переходим к какому-либо сыну
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

    // Перейдя к какому-либо сыну, пытаемся удалить элемент с ключом key в его поддереве
    // Если элемента с ключом key нет в дереве, то выходим из функции и возвращаем false
    if (_delete1(w, key, low1_bk, one_son_bk) == false)
        return false;

    // Случай, если удаление произошло успешно

    // Сначала в текущем узле меняем ключи, т.н. если удалялся элемент из второй ветви,
    // то ключ минимального элемента этой ветви, записываем в поле key2 текущего внутреннего узла cur
    // Если удалялся элемент из третьей ветви, то записываем в поле key3
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

    // Если после удаления элемента не возникло ситуации, когда внутренний узел имеет менее двух сыновей,
    // то выходим из функции и возвращаем true
    if (one_son_bk == false)
        return true;

    // Случай, если в каком-либо сыновьем внутреннем узле cur остался только один сын

    InternalNode* y = nullptr;
    InternalNode* z = nullptr;

    // Переставляем ветви потомков cur в зависимости от того, какой узел и где удаляли

    // Случай, если удаление было у первого сына cur и у него остался один сын
    if (child == 1)
    {
        // w - это первый сын cur: w == cur->son1
        y = dynamic_cast<InternalNode*>(cur->son2); // y - это второй сын cur: y == cur->son2

        // Случай, если у второго сына cur есть собственный третий сын
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
        // Случай, если у второго сына cur нет собственного третьего сына
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
    // Случай, если удаление было у второго сына cur, и у него остался один сын
    else if (child == 2)
    {
        // w - второй сын cur: w == cur->son2
        y = dynamic_cast<InternalNode*>(cur->son1); // y - это первый сын cur: y == cur->son1

        // Случай, если у первого сына cur есть собственный третий сын
        if (y->son3 != nullptr)
        {
            w->son2 = w->son1;
            w->key2 = cur->key2;
            w->son1 = y->son3;
            y->son3 = nullptr;
            cur->key2 = y->key3;
        }
        // Случай, если у первого сына cur нет собственного третьего сына
        else
        {
            z = dynamic_cast<InternalNode*>(cur->son3); // z - третий сын cur: z == cur->son3

            // Если у cur есть третий сын, и у него есть собственный третий сын
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
            // Если у cur нет третьего сына, или он есть, но у него нет собственного третьего сына
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
    // Случай, если удаление было у третьего сына cur, и у него остался один сын
    else
    {
        // w - третий сын cur: w == cur->son3
        y = dynamic_cast<InternalNode*>(cur->son2); // Теперь y - второй сын cur: y == cur->son2

        // Случай, если у второго сына cur есть собственный третий сын 
        if (y->son3 != nullptr)
        {
            w->son2 = w->son1;
            w->key2 = cur->key3;
            w->son1 = y->son3;
            cur->key3 = y->key3;
            y->son3 = nullptr;
        }
        // Случай, если у второго сына cur нет собственного третьего сына
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
    // Если поддерево/дерево пустое, то ничего не выводим
    if (root == nullptr)
        return;

    // Если текущий узел - лист, то выводим его ключ
    if (root->getNodeType() == Node_Type::LEAF)
    {
        for (int i = 0; i < level - 1; i++)
            cout << "  ";
        for (int i = 0; i < 6; i++)
            cout << "--";

        cout << dynamic_cast<const Leaf*>(root)->key << endl << endl;
    }
    // Если текущий узел - внутренний узел, то выводим сначала поддерево третьего сына, затем второго,
    // затем ключи данного внутреннего узла, и, наконец, первого сына
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
    InternalNode* par = nullptr;    // Возвращаемый родительский узел
    Node* cur = this->root;         // Текущий узел

    // Ищем лист lf и указатель на его ближайшего левого или серидинного родителя сохраняем в par
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
    {
        // Переход к первому, т.н. левому сыну
        if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
        {
            par = dynamic_cast<InternalNode*>(cur);
            cur = dynamic_cast<InternalNode*>(cur)->son1;
        }
        // Переход к второму сыну
        else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || lf->key < dynamic_cast<InternalNode*>(cur)->key3)
        {
            // В par заносим cur в том случае, если второй сын - серединный
            if (dynamic_cast<InternalNode*>(cur)->son3 != nullptr)
                par = dynamic_cast<InternalNode*>(cur);

            cur = dynamic_cast<InternalNode*>(cur)->son2;
        }
        // Переход к третьему, т.н. правому сыну
        else
            cur = dynamic_cast<InternalNode*>(cur)->son3;
    }

    return par;
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::InternalNode* Tree23<Key_T, Data_T>::_getRightParent(const Leaf* lf) const
{
    InternalNode* par = nullptr;    // Возвращаемый родительский узел
    Node* cur = this->root;         // Текущий узел

    // Ищем лист lf и указатель на его ближайшего правого или серединного родителя сохраняем в par
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
    {
        // Переход к первому, т.н. левому сыну
        if (lf->key < dynamic_cast<InternalNode*>(cur)->key2)
            cur = dynamic_cast<InternalNode*>(cur)->son1;
        // Переход к второму сыну
        else if (dynamic_cast<InternalNode*>(cur)->son3 == nullptr || lf->key < dynamic_cast<InternalNode*>(cur)->key3)
        {
            par = dynamic_cast<InternalNode*>(cur);
            cur = dynamic_cast<InternalNode*>(cur)->son2;
        }
        // Переход к третьему, т.н. правому сыну
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
    Node* cur = root; // Текущий узел дерева

    // Переходим по левой ветви до листа
    while (cur != nullptr && cur->getNodeType() != Node_Type::LEAF)
        cur = dynamic_cast<InternalNode*>(cur)->son1;

    return dynamic_cast<Leaf*>(cur);
}

template <class Key_T, class Data_T> typename Tree23<Key_T, Data_T>::Leaf* Tree23<Key_T, Data_T>::_getMax(Node* root) const
{
    Node* cur = root; // Текущий узел дерева

    // Переходим по правой ветви до листа
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

// public-методы:

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
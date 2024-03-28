#include <iostream>
#include<Windows.h>
#include "vector"
using std::vector;
template <class K, class D>
class myBST
{
public:
    class Node
    {
    public:
        K key;
        D data;
        Node* left;
        Node* right;
        Node(K _key, D _data) : key(_key), data(_data), left(nullptr), right(nullptr) {};
        Node(const Node& src) : key(src.key), data(src.data), left(src.left), right(src.right) {};

        int getBal()
        {
            return getBalHelp(this->right) - getBalHelp(this->left);
        }

        static int getLevel(Node* t)
        {
            return getBalHelp(t);
        }

    private:
        static int getBalHelp(Node* t)
        {
            int hr = 0;
            int hl = 0;
            if (t == nullptr)
                return 0;
            if (t->left)
            {
                hl = getBalHelp(t->left);
            }
            if (t->right)
            {
                hr = getBalHelp(t->right);
            }
            return max(hr, hl) + 1;
        }
    };

    myBST();                                                   // done
    myBST(const myBST& src);                                   //done
    ~myBST();                                                  // done
    inline int getSize() { return size; }                      // done
    inline bool isEmpty() { return size == 0 ? true : false; } // done
    void insert(const K& key, const D& data);                  // done
    void remove(const K& key);                                 //done
    void clear();                                              //done
    D getData(const K& key);                                   //done
    void setData(const K& key, const D& data);                 //done
    Node* search(const K& key);                                // done
    void showKeys();                                           //done
    void show();                                               // done
    void print();
    inline int getOps() { return ops; }

private:
    Node* root;
    int size = 0;
    int ops = 0;
    void showHelp(Node* t, bool withValues);
    Node* removeHelp(Node* t, const K& key);
    Node* insertHelp(Node* t, const K& key, const D& value);
    void deleteHelp(Node*& t);
    Node* searchHelp(Node*& t, const K& key);
    void printHelper(Node* t, int lvl);

public:

    class baseIT
    {
    private:
        Node* cur;
        int idx;
        std::vector<Node*> nodes;
        void setNode(const myBST<K, D>& srcBst);
        virtual void setNodeHelp(Node* t);

    public:
        baseIT();
        ~baseIT();
    };

    class it : baseIT
    {
    public:
        Node* cur;
        int idx;
        std::vector<Node*> nodes;
        void setNode(const myBST<K, D>& srcBst);
        void setNodeHelp(Node* t);

    public:
        it() {};
        it(const myBST<K, D>& srcBst);
        ~it();
        void begin();
        void end();
        it& operator++();
        it& operator--();
        bool operator==(const it& src);
        bool operator!=(const it& src);
        Node* operator*();
    };

    class rIt : baseIT
    {
    public:
        Node* cur;
        int idx;
        std::vector<Node*> nodes;
        void setNode(const myBST<K, D>& srcBst);
        void setNodeHelp(Node* t);

    public:
        rIt() {};
        rIt(const myBST<K, D>& srcBst);
        ~rIt();
        void rBegin();
        void rEnd();
        rIt& operator++();
        rIt& operator--();
        bool operator==(const rIt& src);
        bool operator!=(const rIt& src);
        Node* operator*();
    };
};
#include <iostream>
#include<windows.h>
#include "bst.cpp"
#define MIN_INT -2147483647
#define MAX_INT 2147483647
using namespace std;

void test_rand(int n)
{
    //создание дерева для 64 – разрядных ключей типа INT_64
    myBST< int64_t, int > tree;
    //массив для ключей, которые присутствуют в дереве 
    int64_t* m = new int64_t[n];
    //установка первого случайного числа
    srand(time(0));
    //заполнение дерева и массива элементами со случайными ключами
    for (int i = 0; i < n; i++)
    {
        m[i] = rand();
        tree.insert(m[i], 1);
    }
    //вывод размера дерева до теста
    std::cout << "items count before test:" << tree.getSize() << std::endl;
    //обнуление счётчиков трудоёмкости вставки, удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;
    //генерация потока операций, 10% - промахи операций
    for (int i = 0; i < n / 2; i++)
        if (i % 10 == 0)        //10% промахов
        {
            tree.remove(rand());
            D += tree.getOps();

            tree.insert(m[rand() % n], 1);
            I += tree.getOps();

            try {
                tree.getData(rand());
                S += tree.getOps();
            }
            //обработка исключения при ошибке операции поиска
            catch (const runtime_error& error) { S += tree.getOps(); }
        }
        else  //90% успешных операций
        {
            int ind = rand() % n;
            tree.remove(m[ind]);
            D += tree.getOps();

            int64_t key = rand();
            tree.insert(key, 1);
            I += tree.getOps();
            m[ind] = key;

            try {
                tree.getData(m[rand() % n]);
                S += tree.getOps();
            }
            //обработка исключения при ошибке операции поиска
            catch (const runtime_error& error) { S += tree.getOps(); }
        }    //конец теста

     //вывод результатов:
     //вывод размера дерева после теста
    std::cout << "items count after test:" << tree.getSize() << std::endl;
    //теоретической оценки трудоёмкости операций BST
    std::cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << std::endl;
    //экспериментальной оценки трудоёмкости вставки
    std::cout << "Count insert: " << I / (n / 2) << std::endl;
    //экспериментальной оценки трудоёмкости удаления
    std::cout << "Count delete: " << D / (n / 2) << std::endl;
    //экспериментальной оценки трудоёмкости поиска
    std::cout << "Count search: " << S / (n / 2) << std::endl << std::endl;
    //освобождение памяти массива m[]
    delete[] m;
    //tree.print();
}


void test_ord(int n)
{
    //создание дерева для 64 – разрядных ключей типа INT_64
    myBST< int64_t, int > tree;
    //массив для ключей, которые присутствуют в дереве 
    int64_t* m = new int64_t[n];

    //заполнение дерева и массива элементами с возрастающими чётными //ключами на интервале [0, 10000, 20000, ... ,10000*n]
    for (int i = 0; i < n; i++) {
        m[i] = i * (int64_t)10000;
        tree.insert(m[i], 1);
    }
    //вывод размера дерева до теста 
    std::cout << "items count before test:" << tree.getSize() << std::endl;
    //обнуление счётчиков трудоёмкости вставки, удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;
    //установка первого случайного числа 
    srand(time(0));
    //генерация потока операций, 10% - промахи операций
    for (int i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0)        //10% промахов
        {
            int k = rand() % (10000 * n);
            k = k + !(k % 2);    //случайный нечётный ключ
            tree.remove(k);
            D += tree.getOps();
            tree.insert(m[rand() % n], 1);
            I += tree.getOps();
            k = rand() % (10000 * n);
            k = k + !(k % 2);    // случайный нечётный ключ
            try {
                tree.getData(k);
                S += tree.getOps();
            }
            //обработка исключения при ошибке операции поиска
            catch (const runtime_error& error) { S += tree.getOps(); }
        }
        else  //90% успешных операций
        {
            int ind = rand() % n;
            tree.remove(m[ind]);
            D += tree.getOps();;
            int k = rand() % (10000 * n);
            k = k + k % 2;        // случайный чётный ключ
            tree.insert(k, 1);
            I += tree.getOps();; 
            m[ind] = k;
            try {
                tree.getData(m[rand() % n]);
                S += tree.getOps();;
            }
            //обработка исключения при ошибке операции поиска
            catch (const runtime_error& error) { S += tree.getOps(); }
        }
    }

    //вывод результатов:
    // вывод размера дерева после теста
    std::cout << "items count after test:" << tree.getSize() << std::endl;
    //теоретической оценки трудоёмкости операций BST
    std::cout << "n/2 =" << n / 2 << std::endl;
    //экспериментальной оценки трудоёмкости вставки
    std::cout << "Count insert: " << I / (n / 2) << std::endl;
    //экспериментальной оценки трудоёмкости удаления
    std::cout << "Count delete: " << D / (n / 2) << std::endl;
    //экспериментальной оценки трудоёмкости поиска
    std::cout << "Count search: " << S / (n / 2) << std::endl << std::endl;
    //освобождение памяти массива m[]
    delete[] m;
    //tree.print();
}    //конец теста

int Menu();
int getNumber(int min, int max);

int main() {

    for (int i = 2000; i <= 2000; i += 1000) {
        test_ord(2000);
        test_rand(2000);
    }

    /*HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);

    setlocale(LC_ALL, "Russian");
    system("title BSTree");
    system("mode con cols=77 lines=35");

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;

    wcscpy_s(cfi.FaceName, L"Lucida Console");

    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    SetConsoleTextAttribute(hOUTPUT, FOREGROUND_GREEN);
    COORD c = { 77, 900 };
    SetConsoleScreenBufferSize(hOUTPUT, c);

    bool done = false;
    bool allowedToGetBal = false;

    myBST<int, int> tree;
    myBST<int, int>::it it;
    myBST<int, int>::rIt rIt;

    while (!done) {

        switch (Menu()) {

        case 1: {
            std::cout << "Insert a key: " << std::endl;
            int a = getNumber(MIN_INT, MAX_INT);
            std::cout << "Insert a data: " << std::endl;
            int b = getNumber(MIN_INT, MAX_INT);
            tree.insert(a, b);
            tree.print();
        }
              break;
        case 2: {
            std::cout << "Size: " << tree.getSize() << std::endl;
            tree.print();

        }
              break;
        case 3: {
            if (tree.isEmpty()) {
                std::cout << "Tree is clear" << std::endl;
                tree.print();
            }
            else {
                std::cout << "Tree is not clear" << std::endl;
                tree.print();
            }

        }
              break;

        case 4:
        {
            tree.clear();
            std::cout << "Cleared" << std::endl;
            tree.print();
        }
        break;
        case 5:
        {   try {
            int k;
            std::cout << "Enter a key:" << std::endl;
            k = getNumber(MIN_INT, MAX_INT);
            std::cout << tree.getData(k) << std::endl;
            tree.print();
        }
        catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
        }

        }
        break;
        case 6:
        {
            int k, l;
            std::cout << "Enter a key: " << std::endl;
            k = getNumber(MIN_INT, MAX_INT);
            std::cout << "Enter a data: " << std::endl;
            l = getNumber(MIN_INT, MAX_INT);
            tree.setData(k, l);
            tree.print();

        }
        break;
        case 7:
        {
            int k;
            std::cout << "Enter a key: " << std::endl;
            k = getNumber(MIN_INT, MAX_INT);
            tree.remove(k);
            tree.print();

        }
        break;
        case 8:
        {
            tree.show();
            tree.print();
        }
        break;
        case 9:
        {
            tree.showKeys();
            tree.print();
        }
        break;
        case 10:
        {
            it = myBST<int, int>::it(tree);
            it.begin();
            myBST<int, int>::Node* node = *(it);
            std::cout << node->key << std::endl;
            tree.print();
            allowedToGetBal = true;

        }

        break;
        case 11:
        {   try {
            it.operator++();
            myBST<int, int>::Node* node = *(it);
            std::cout << node->key << std::endl;
            tree.print();
        }
        catch (std::runtime_error e) {
            std::cout << e.what() << std::endl;
        }
        }
        break;
        case 12:
        {
            try {
                it.operator--();
                myBST<int, int>::Node* node = *(it);
                std::cout << node->key << std::endl;
                tree.print();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }
        }

        break;

        case 13:
        {
            try {
                myBST<int, int>::Node* node = *(it);
                std::cout << "Insert new data" << std::endl;
                int k = getNumber(MIN_INT, MAX_INT);
                node->data = k;
                tree.print();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }

        }
        break;

        case 14:
        {
            it = myBST<int, int>::it(tree);
            it.end();
            tree.print();
            allowedToGetBal = true;
        }

        break;
        case 15:
        {

            rIt = myBST<int, int>::rIt(tree);
            rIt.rBegin();
            myBST<int, int>::Node* node = *(rIt);
            std::cout << node->key << std::endl;
            tree.print();
            break;
        }
        case 16:
        {
            try {
                rIt.operator++();
                myBST<int, int>::Node* node = *(rIt);
                std::cout << node->key << std::endl;
                tree.print();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;
        case 17:
        {
            try {
                rIt.operator--();
                myBST<int, int>::Node* node = *(rIt);
                std::cout << node->key << std::endl;
                tree.print();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }

        }
        break;

        case 18:
        {
            try {
                myBST<int, int>::Node* node = *(rIt);
                std::cout << "Insert new data" << std::endl;
                int k = getNumber(MIN_INT, MAX_INT);
                node->data = k;
                tree.print();
            }
            catch (std::runtime_error e) {
                std::cout << e.what() << std::endl;
            }

        }
        break;

        case 19:
        {
            rIt = myBST<int, int>::rIt(tree);
            rIt.rEnd();
            tree.print();

        }

        break;
        case 20:
        {   if (allowedToGetBal) {
            myBST<int, int>::Node* node = *(it);
            std::cout << node->getBal() << std::endl;
        }
        else {
            std::cout << "Wrong iterator" << std::endl;
        }

        }
        break;
        case 21:
        {
            std::cout << "Ops: " << tree.getOps() << std::endl;

        }
        break;
        case 0:
            std::cout << "Exit" << std::endl;
            done = true;
            break;
        default:
            Menu();
        }

    }  
    return 0;

}
int getNumber(int min, int max) {
    int number = min - 1;
    while (true) {
        std::cin >> number;
        if ((number >= min) && (number <= max) && (std::cin.peek() == '\n'))
            break;
        else {
            std::cout << "Type an int value: " << std::endl;

            std::cin.clear();

            while (std::cin.get() != '\n') {
            }
        }
    }
    return number;
}
int Menu() {
    std::cout << "\n============= Menu =============" << std::endl;
    std::cout << "1. Insert" << std::endl;
    std::cout << "2. Size" << std::endl;
    std::cout << "3. Clear?" << std::endl;
    std::cout << "4. Clear" << std::endl;
    std::cout << "5. Get data by a key" << std::endl;
    std::cout << "6. Set data" << std::endl;
    std::cout << "7. Remove by a key" << std::endl;
    std::cout << "8. Show Lt -> Rt -> t" << std::endl;
    std::cout << "9. Show Lt -> Rt -> t without values" << std::endl;
    std::cout << "10. Set Iterator" << std::endl;
    std::cout << "11. ++" << std::endl;
    std::cout << "12. --" << std::endl;
    std::cout << "13. *" << std::endl;
    std::cout << "14. Set Iterator end" << std::endl;
    std::cout << "15. Set Reverse Iterator" << std::endl;
    std::cout << "16. ++" << std::endl;
    std::cout << "17. --" << std::endl;
    std::cout << "18. *" << std::endl;
    std::cout << "19. Set Reverse Iterator end" << std::endl;
    std::cout << "20. tbal(it(Node))" << std::endl;
    std::cout << "21. Last operations: " << std::endl;
    std::cout << "0. Exit" << std::endl << std::endl;
    std::cout << "Select: ";

    return getNumber(0, 22);*/
}
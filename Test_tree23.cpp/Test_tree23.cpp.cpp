// Подключаемые библиотеки и заголовочные файлы
#include <iostream>
#include <string> 
#include <math.h>
#include <time.h>
#include <locale.h>
#include "Tree23.h"

const size_t MAX_SIZE = 1000000; // Максимально допустимый размер дерева

using std::string;
using std::invalid_argument;
using std::out_of_range;
typedef unsigned long long INT_64;

// Переменная и константы генератора LineRand() 
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

// Прототипы функций
void sRand();
INT_64 LineRand();
void test_rand(int n);
void test_ord(int n, INT_64 delta);
INT_64 get_val();


int main()
{
    INT_64 start, end, step;

    setlocale(LC_ALL, "Russian");

    cout << "\t\t\tПрограмма тестирования операций вставки, удаления и поиска в 2-3 дереве." << endl << endl;

    cout << "Введите начальное значение размерности дерева(не больше " << MAX_SIZE << "): " << endl;
    start = get_val();
    while (start > MAX_SIZE)
    {
        cout << "Ошибка: введённое значение начальной размерности превышает допустимое максимальное "
            "значение размерности дерева. ";
        cout << "Повторите ввод." << endl;
        start = get_val();
    }
    cout << "Введите конечное значение размерности дерева(не больше " << MAX_SIZE << "): " << endl;
    end = get_val();
    while (end > MAX_SIZE)
    {
        cout << "Ошибка: введённое значение начальной размерности превышает допустимое максимальное "
            "значение размерности дерева. ";
        cout << "Повторите ввод." << endl;
        end = get_val();
    }
    cout << "Введите шаг для размерности дерева." << endl;
    step = get_val();

    cout << "Тестирования началось..." << endl;
    for (INT_64 i = start; i <= end; i += step)
    {
        cout << "Результаты тестирования случайного дерева размерностью " << i << ":" << endl;
        test_rand(i);
        cout << "Результаты тестирования вырожденного дерева размерностью " << i << ":" << endl;
        test_ord(i, 1000000);
        cout << endl;
    }
    cout << "Тестирования окончено." << endl;
    cout << "Программа успешно завершила свою работу. Нажмите Enter для выхода из программы...";
    cin.get();

    return 0;
}

// Функция установки первого случайного числа от часов компьютера
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

// Функция генерации случайного числа  линейным конгруэнтным генератором Xi+1 =(a * Xi + c) % arr 
//habr.com/ru/post/208178 
INT_64 LineRand()
{
    INT_64 y1, y2;

    y1 = (aRand * RRand + cRand) % mRand;
    y2 = (aRand * y1 + cRand) % mRand;
    RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;

    return RRand;
}

// Тестирование случайного 2-3 дерева
void test_rand(int n)
{
    Tree23<INT_64, int> tree;  // Создание дерева для 64 – разрядных ключей типа INT_64
    INT_64* arr = new INT_64[n]; // Массив для ключей, которые присутствуют в дереве

    sRand(); // Установка первого случайного числа

    for (int i = 0; i < n; i++)  // Заполнение дерева и массива элементами со случайными ключами 
    {
        arr[i] = LineRand();
        tree.append(arr[i], 1);
    }

    // Обнуляем счётчик операций самого 2-3 дерева
    tree.setLastOp(0);

    // Вывод размера дерева до теста 
    cout << "Размерность дерева до испытаний: " << tree.getSize() << "." << endl;

    // Обнуление счётчиков трудоёмкости вставки, удаления и поиска 
    double I = 0;
    double D = 0;
    double S = 0;

    // Генерация потока операций, 10% - промахи операций 
    for (int i = 0; i < n / 2; i++)
        if (i % 10 == 0) // 10% промахов 
        {
            try { tree.del(LineRand()); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            try { tree.append(arr[rand() % n], 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();

            try { tree.getItemByKey(LineRand()); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        }
        else // 90% успешных операций 
        {
            int index = rand() % n;

            try { tree.del(arr[index]); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            INT_64 key = LineRand();

            try { tree.append(key, 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();
            arr[index] = key;

            try { tree.getItemByKey(arr[rand() % n]); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        } // Конец теста 

    // Вывод результатов:  
    // Вывод размера дерева после теста
    cout << "Размерность дерева после испытаний: " << tree.getSize() << endl;

    // Теоретической оценки трудоёмкости операций 2-3 дерева
    cout << "Теоретическая оценка([log3(n), log2(n)]): " << "[" << (log((double)n) / log(3.0))
        << ", " << (log((double)n) / log(2.0)) << "]" << endl;

    // Экспериментальной оценки трудоёмкости вставки 
    cout << "Среднее количество переборов на одну вставку: " << I / (n / 2) << endl;

    // Экспериментальной оценки трудоёмкости удаления  
    cout << "Среднее количество переборов на одно удаление: " << D / (n / 2) << endl;

    // Экспериментальной оценки трудоёмкости поиска 
    cout << "Среднее количество переборов на один поиск: " << S / (n / 2) << endl;

    delete[] arr; // Освобождение памяти массива arr[]
}

// Тестирование вырожденного 2-3 дерева
// delta - интервал между последовательно возрастающими числами
void test_ord(int n, INT_64 delta)
{
    Tree23<INT_64, int> tree; // Создание дерева для 64 – разрядных ключей типа INT_64  
    INT_64* arr = new INT_64[n]; // Массив для ключей, которые присутствуют в дереве

    sRand(); // Установка первого случайного числа

    // Заполнение дерева и массива элементами с возрастающими чётными ключами на интервале
    // [0, delta, 2 * delta, ... ,delta * n] 
    for (int i = 0; i < n; i++)
    {
        arr[i] = i * delta;
        tree.append(arr[i], 1);
    }

    // Обнуляем счётчик операций самого 2-3 дерева
    tree.setLastOp(0);

    cout << "Размерность дерева до испытаний: " << tree.getSize() << endl; // Вывод размера дерева до теста

    // Обнуление счётчиков трудоёмкости вставки, удаления и поиска
    double I = 0;
    double D = 0;
    double S = 0;

    // Генерация потока операций, 10% - промахи операций 
    for (int i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0)  // 10% промахов 
        {
            int key = LineRand() % (delta * n);

            key += ~(key % 2); // Случайный нечётный ключ 

            try { tree.del(key); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            try { tree.append(arr[rand() % n], 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();

            key = LineRand() % (delta * n);
            key += ~(key % 2); // Случайный нечётный ключ 
            try { tree.getItemByKey(key); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        }
        else  // 90% успешных операций 
        {
            int index = rand() % n;

            try { tree.del(arr[index]); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            int key = LineRand() % (delta * n);

            key += key % 2;  // Случайный чётный ключ 
            try { tree.append(key, 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();
            arr[index] = key;

            try { tree.getItemByKey(arr[rand() % n]); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        }
    }

    // Вывод результатов:
    // Вывод размера дерева после теста
    cout << "Размерность дерева после испытаний: " << tree.getSize() << endl;

    // Теоретической оценки трудоёмкости операций 2-3 дерева
    cout << "Теоретическая оценка([log3(n), log2(n)]): " << "[" << (log((double)n) / log(3.0))
        << ", " << (log((double)n) / log(2.0)) << "]" << endl;

    // Экспериментальной оценки трудоёмкости вставки
    cout << "Среднее количество переборов на одну вставку: " << I / (n / 2) << endl;

    // Экспериментальной оценки трудоёмкости удаления
    cout << "Среднее количество переборов на одно удаление: " << D / (n / 2) << endl;

    // Экспериментальной оценки трудоёмкости поиска
    cout << "Среднее количество переборов на один поиск: " << S / (n / 2) << endl;

    delete[] arr; // Освобождение памяти массива arr[]
}

INT_64 get_val()
{
    string buf;
    INT_64 val;

    cout << ">>> ";
    getline(cin, buf);

    while (true)
    {
        try
        {
            val = stoi(buf);
            break;
        }
        catch (invalid_argument)
        {
            cout << "Ошибка: неверный формат входных данных." << endl;
        }
        catch (out_of_range)
        {
            cout << "Ошибка: введённое число вышло из диапазона int." << endl;
        }

        cout << ">>> ";
        getline(cin, buf);
    }

    return val;
}
// ������������ ���������� � ������������ �����
#include <iostream>
#include <string> 
#include <math.h>
#include <time.h>
#include <locale.h>
#include "Tree23.h"

const size_t MAX_SIZE = 1000000; // ����������� ���������� ������ ������

using std::string;
using std::invalid_argument;
using std::out_of_range;
typedef unsigned long long INT_64;

// ���������� � ��������� ���������� LineRand() 
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

// ��������� �������
void sRand();
INT_64 LineRand();
void test_rand(int n);
void test_ord(int n, INT_64 delta);
INT_64 get_val();


int main()
{
    INT_64 start, end, step;

    setlocale(LC_ALL, "Russian");

    cout << "\t\t\t��������� ������������ �������� �������, �������� � ������ � 2-3 ������." << endl << endl;

    cout << "������� ��������� �������� ����������� ������(�� ������ " << MAX_SIZE << "): " << endl;
    start = get_val();
    while (start > MAX_SIZE)
    {
        cout << "������: �������� �������� ��������� ����������� ��������� ���������� ������������ "
            "�������� ����������� ������. ";
        cout << "��������� ����." << endl;
        start = get_val();
    }
    cout << "������� �������� �������� ����������� ������(�� ������ " << MAX_SIZE << "): " << endl;
    end = get_val();
    while (end > MAX_SIZE)
    {
        cout << "������: �������� �������� ��������� ����������� ��������� ���������� ������������ "
            "�������� ����������� ������. ";
        cout << "��������� ����." << endl;
        end = get_val();
    }
    cout << "������� ��� ��� ����������� ������." << endl;
    step = get_val();

    cout << "������������ ��������..." << endl;
    for (INT_64 i = start; i <= end; i += step)
    {
        cout << "���������� ������������ ���������� ������ ������������ " << i << ":" << endl;
        test_rand(i);
        cout << "���������� ������������ ������������ ������ ������������ " << i << ":" << endl;
        test_ord(i, 1000000);
        cout << endl;
    }
    cout << "������������ ��������." << endl;
    cout << "��������� ������� ��������� ���� ������. ������� Enter ��� ������ �� ���������...";
    cin.get();

    return 0;
}

// ������� ��������� ������� ���������� ����� �� ����� ����������
void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

// ������� ��������� ���������� �����  �������� ������������ ����������� Xi+1 =(a * Xi + c) % arr 
//habr.com/ru/post/208178 
INT_64 LineRand()
{
    INT_64 y1, y2;

    y1 = (aRand * RRand + cRand) % mRand;
    y2 = (aRand * y1 + cRand) % mRand;
    RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;

    return RRand;
}

// ������������ ���������� 2-3 ������
void test_rand(int n)
{
    Tree23<INT_64, int> tree;  // �������� ������ ��� 64 � ��������� ������ ���� INT_64
    INT_64* arr = new INT_64[n]; // ������ ��� ������, ������� ������������ � ������

    sRand(); // ��������� ������� ���������� �����

    for (int i = 0; i < n; i++)  // ���������� ������ � ������� ���������� �� ���������� ������� 
    {
        arr[i] = LineRand();
        tree.append(arr[i], 1);
    }

    // �������� ������� �������� ������ 2-3 ������
    tree.setLastOp(0);

    // ����� ������� ������ �� ����� 
    cout << "����������� ������ �� ���������: " << tree.getSize() << "." << endl;

    // ��������� ��������� ����������� �������, �������� � ������ 
    double I = 0;
    double D = 0;
    double S = 0;

    // ��������� ������ ��������, 10% - ������� �������� 
    for (int i = 0; i < n / 2; i++)
        if (i % 10 == 0) // 10% �������� 
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
        else // 90% �������� �������� 
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
        } // ����� ����� 

    // ����� �����������:  
    // ����� ������� ������ ����� �����
    cout << "����������� ������ ����� ���������: " << tree.getSize() << endl;

    // ������������� ������ ����������� �������� 2-3 ������
    cout << "������������� ������([log3(n), log2(n)]): " << "[" << (log((double)n) / log(3.0))
        << ", " << (log((double)n) / log(2.0)) << "]" << endl;

    // ����������������� ������ ����������� ������� 
    cout << "������� ���������� ��������� �� ���� �������: " << I / (n / 2) << endl;

    // ����������������� ������ ����������� ��������  
    cout << "������� ���������� ��������� �� ���� ��������: " << D / (n / 2) << endl;

    // ����������������� ������ ����������� ������ 
    cout << "������� ���������� ��������� �� ���� �����: " << S / (n / 2) << endl;

    delete[] arr; // ������������ ������ ������� arr[]
}

// ������������ ������������ 2-3 ������
// delta - �������� ����� ��������������� ������������� �������
void test_ord(int n, INT_64 delta)
{
    Tree23<INT_64, int> tree; // �������� ������ ��� 64 � ��������� ������ ���� INT_64  
    INT_64* arr = new INT_64[n]; // ������ ��� ������, ������� ������������ � ������

    sRand(); // ��������� ������� ���������� �����

    // ���������� ������ � ������� ���������� � ������������� ������� ������� �� ���������
    // [0, delta, 2 * delta, ... ,delta * n] 
    for (int i = 0; i < n; i++)
    {
        arr[i] = i * delta;
        tree.append(arr[i], 1);
    }

    // �������� ������� �������� ������ 2-3 ������
    tree.setLastOp(0);

    cout << "����������� ������ �� ���������: " << tree.getSize() << endl; // ����� ������� ������ �� �����

    // ��������� ��������� ����������� �������, �������� � ������
    double I = 0;
    double D = 0;
    double S = 0;

    // ��������� ������ ��������, 10% - ������� �������� 
    for (int i = 0; i < n / 2; i++)
    {
        if (i % 10 == 0)  // 10% �������� 
        {
            int key = LineRand() % (delta * n);

            key += ~(key % 2); // ��������� �������� ���� 

            try { tree.del(key); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            try { tree.append(arr[rand() % n], 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();

            key = LineRand() % (delta * n);
            key += ~(key % 2); // ��������� �������� ���� 
            try { tree.getItemByKey(key); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        }
        else  // 90% �������� �������� 
        {
            int index = rand() % n;

            try { tree.del(arr[index]); }
            catch (Exception exception) {}
            D += tree.getLastOp();

            int key = LineRand() % (delta * n);

            key += key % 2;  // ��������� ������ ���� 
            try { tree.append(key, 1); }
            catch (Exception exception) {}
            I += tree.getLastOp();
            arr[index] = key;

            try { tree.getItemByKey(arr[rand() % n]); }
            catch (Exception exception) {}
            S += tree.getLastOp();
        }
    }

    // ����� �����������:
    // ����� ������� ������ ����� �����
    cout << "����������� ������ ����� ���������: " << tree.getSize() << endl;

    // ������������� ������ ����������� �������� 2-3 ������
    cout << "������������� ������([log3(n), log2(n)]): " << "[" << (log((double)n) / log(3.0))
        << ", " << (log((double)n) / log(2.0)) << "]" << endl;

    // ����������������� ������ ����������� �������
    cout << "������� ���������� ��������� �� ���� �������: " << I / (n / 2) << endl;

    // ����������������� ������ ����������� ��������
    cout << "������� ���������� ��������� �� ���� ��������: " << D / (n / 2) << endl;

    // ����������������� ������ ����������� ������
    cout << "������� ���������� ��������� �� ���� �����: " << S / (n / 2) << endl;

    delete[] arr; // ������������ ������ ������� arr[]
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
            cout << "������: �������� ������ ������� ������." << endl;
        }
        catch (out_of_range)
        {
            cout << "������: �������� ����� ����� �� ��������� int." << endl;
        }

        cout << ">>> ";
        getline(cin, buf);
    }

    return val;
}
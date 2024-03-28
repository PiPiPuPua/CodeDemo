#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <inttypes.h>
//#include <float.h >
//#include <errno.h >
#include <omp.h>

int check(int n) {

    int len = 0;
    while (n > 1) {

        n = (n % 2 == 0 ? n / 2 : 3 * n + 1);
        len++;
    }

    return len;
}

int linearSearch(int n) {

    printf("Starting linear search...\n");

    int curLead = 0, maxlen = 0;

    double startTime = omp_get_wtime();

    for (int i = n - 1; i >= 1; i--) {

        int res = check(i);
        if (res > maxlen) {

            maxlen = res;
            curLead = i;
        }
    }

    double time = omp_get_wtime();

    printf("Linear algorithm time: %lf\n", time - startTime);

    return curLead;
}

int parallelSearch(int N) {

    printf("Starting parallel search...\n");
   
    const int num_threads = 4;
    omp_set_num_threads(num_threads);

    double startTime = omp_get_wtime();

    int curLead = 0, maxlen = 0;

#pragma omp parallel
    {
#pragma omp for
        for (int i = N-1; i >= 1; i--) {

            int res = check(i);

            if (res > maxlen) {
#pragma omp critical
                {
                    curLead = i;
                    maxlen = res;
                }
            }
        }
    }

    double time = omp_get_wtime();
    printf("Parallel algorithm time: %lf\n", time - startTime);

    return curLead;
}

uint64_t inputCheck(){ // проверяет больше ли число единицы, отрицательное ли оно, входит ли в диапазон значений ull, есть ли посторонние символы
    
    char* num = (char*)malloc(31);
    fgets(num, 30, stdin);
    if (strcmp(num, "EXIT\n") == 0)
        return INT64_MIN;

    for (int i = 0; (num[i] != '\n') && (i < 30); i++)
        if ((57 - num[i] > 9) || (57 - num[i] < 0))
            return 0;

    return strtoull(num, NULL, 10);;
}  

int main()
{

    printf("Provide a NUMBER > 1 fitting ull, then press ENTER:\n");

    while (1) {

        uint64_t n = inputCheck();

        if (n == INT64_MIN)
            return 0;

        while (errno == ERANGE || n <= 1) {

            errno = NULL;
            printf("not appropriate number, try one more time, don't forget about ENTER:\n");
            n = inputCheck();

            if (n == INT64_MIN)
                return 0;
        }

        int res1, res2;
        res1 = linearSearch(n);
        res2 = parallelSearch(n);
        
        printf("%d %d\n", res1, res2);

        if(res1 == res2)
            printf("Both results are equal to %d\n", res1);
        else
            printf("Resuls are not equal\n");

        printf("Keep inserting numbers to continue working\nTo exit insert EXIT and then ENTER\n");
    }

    return 0;
        
}


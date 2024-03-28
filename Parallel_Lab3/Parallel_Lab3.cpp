//Найти минимальное, большее заданного, число, которое равно сумме его десятичных цифр, возведенной в степень, большую 1 (например, 4913 = (4+9+1+3)3)

#include <malloc.h>
#include <math.h>
#include <stdio.h>// для проверки вывода
#include <limits.h>
#include <omp.h>
#include <clocale>
#include <cstdlib>
#include "mpi.h"
#define ull unsigned long long
#define THREADSPERBLOCK 512
#define THREAD_NUM 128

int sum_digits(ull n) {
	int sum = 0;
	for (; n > 0; n /= 10) {
		sum += n % 10;
	}
	return sum;
}

bool is_integer(long double num) { //проверка на целость числа
	ull int_num = (ull)num; //откидываем дробную часть приведением
	if (num - int_num > 0) return false; //проверяем разницу	
	return true;
}

bool is_num_degree(ull num) {//проверка является ли число каким либо корнем
	long double sqrt_num = ULONG_MAX;
	for (ull i = 2; sqrt_num >= 2; i++) { //перебор степеней корня
		sqrt_num = pow(num, 1.0 / i);//нахождение корня нной степени
		if (is_integer(sqrt_num)) return true;//если корень - целое число, то число является корнем
	}
	return false;
}

double lianear_function(ull N) {//функция перебора чисел от минимальнго, до нахождения по заданному условию, или дохождение до максимального
	double time = omp_get_wtime();
	for (ull i = N; i < ULLONG_MAX; i++) {
		if (is_num_degree(sum_digits(i))) {
			printf("%llu\n", i);
			printf("%d\n", sum_digits(i));
			return omp_get_wtime() - time;
		}
	}
	printf("no such num\n");
	return omp_get_wtime() - time;
}


void paralell_function(ull N, int rank, int size) {

	int min_val = ULLONG_MAX;

	MPI_Status status;

	if (rank > 0) {
		printf("%d\n", rank);
		ull max_i = ULLONG_MAX / (size - 1);
		for (int i = 0; i < max_i; i++) {

			ull n = N + i * (size - 1) + rank - 1;

			if (is_num_degree(sum_digits(i))) {
				MPI_Send(&n, 1, MPI_UNSIGNED_LONG_LONG, 0, rank, MPI_COMM_WORLD);
				break;
			}

		}
	}


	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			ull answ = ULLONG_MAX;
			MPI_Recv(&answ, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, &status);
			if (answ < min_val) {
				min_val = answ;
			}
		}
		if (min_val < ULLONG_MAX) {
			printf("%llu\n", min_val);
		}

	}

}


int main(int argc, char** argv) {

	int rank, size;
	ull N;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	setlocale(LC_ALL, "Rus");
	N = atoi(argv[1]);

	paralell_function(N, rank, size);
	if (rank == 0) {
		printf("Время последовательных вычислений: %f\n", lianear_function(N));
		double time = omp_get_wtime();
		printf("Время параллельных вычислений: %f\n", omp_get_wtime() - time);
	}

	MPI_Finalize();

	return 0;
}
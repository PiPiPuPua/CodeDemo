#include "CL/cl.h"
#define _CRT_SECURE_NO_WARNINGS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS // clCreateCommandQueue в последних версиях заменили, так мы обходим ограничение
#include <omp.h>
#include <malloc.h>
#include <stdio.h>
#include <climits>
#include <clocale>
#include < string.h >

using namespace std;

//#define lld long double -- opencl не поддерживает этот тип данных
#define lld double 

//очищаем память
int MatrixDeinit(lld** matrix, int size) {

	for (int i = 0; i < size; i++)
		free(matrix[i]);

	free(matrix);

	return 0;
}

// вывод матрицы - отладка
void PrintMatrix(lld** matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			//printf("%10.3ld ", matrix[i][j]);
			printf("%f ", matrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("\n");
}


void SwichDioganalZero(lld** matrix, int size) {
	for (int i = 0; i < size; i++) {

		//PrintMatrix(matrix, size);

		int k = 0;
		// если диагональный элемент равен нулю
		if (matrix[i][i] < 0.0001)
			// ищем другой ниже не нулевой
			for (; matrix[i + k][i] != 0, i + k < size; k++);
		// если ненулевых нет переходим к следующему ряду
		if (i + k == size)
			break;
		// меняем строки
		if (k != 0) {
			lld* buf = (lld*)malloc(size * sizeof(lld));
			memcpy(buf, matrix[i], size * sizeof(lld));
			memcpy(matrix[i], matrix[i + k], size * sizeof(lld));
			memcpy(matrix[i + k], buf, size * sizeof(lld));
			free(buf);
		}
	}
}

//находим определитель
lld FindDeterminant(lld** matrix, int size) {

	lld det = 1;


	// приводим матрицу к треугольному виду
	for (int i = 0; i < size; i++) {
		//перебор по следующим строкам
		for (int m = 1; m + i < size; m++) {
			//перебор по следующим столбцам
			lld coef = matrix[i + m][i] / matrix[i][i];
			for (int j = i; j < size; j++) {
				matrix[i + m][j] -= matrix[i][j] * coef;
			}
		}
	}



	// перемножаем главную диагональ треугольной матрицы
	for (int i = 0; i < size; i++)
		det *= matrix[i][i];
	return det;
}

int main() {
	setlocale(LC_ALL, "Rus");

	char filename[] = "matrix_9_1024x1024f.txt";

	// читаем файл, выделяем память под матрицу
	int size = 0;
	FILE* file;
	file = fopen(filename, "r");

	if (file == NULL) {
		printf("Не удалось открыть файл.\n");
		return 1;
	}

	fscanf(file, "%d", &size);
	int row = size;
	fscanf(file, "%d", &size);

	if (row != size) // если матрица не является квадратной
		return 1;

	lld** matrix = (lld**)malloc(sizeof(lld*) * size); //выделение памяти под строки
	lld* matrix_parallel = (lld*)malloc(sizeof(lld) * size * size); //выделение памяти под строки

	for (int i = 0; i < size; i++) { // выделение памяти под элементы
		matrix[i] = (lld*)malloc(sizeof(lld) * size);
	}

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			matrix[i][j] = 0;

		}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			fscanf(file, "%lf", &matrix[i][j]);
			//matrix[i][j] /= 10000;
		}
	}

	fclose(file);
	//////////////////////////////////////////////////////////////////////////

	SwichDioganalZero(matrix, size);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			matrix_parallel[i * size + j] = matrix[i][j];
		}
	}

	//последовательная часть

	double start = omp_get_wtime();

	lld determinant = FindDeterminant(matrix, size);

	double end = omp_get_wtime();

	printf("Последовательный метод\n");
	printf("Определитель матрицы равен: %f \n", determinant);
	printf("Время последовательного метода: %f \n", end - start);

	MatrixDeinit(matrix, size);
	// конец последовательной части

	// параллельная часть

	cl_int ret; //если равен нулю - все правильно
	cl_platform_id platform_id;
	cl_uint ret_num_platforms;

	/* получить доступные платформы */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	cl_device_id device_id;
	cl_uint ret_num_devices;

	/* получить доступные устройства */
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	if (ret != CL_SUCCESS)
		return 1;

	cl_context context;
	/* создать контекст */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//if (ret == NULL)
		//return 1;

	// Получаем информацию о размерности рабочей группы
	cl_uint maxWorkItemDimensions;
	size_t maxWorkItemSizes[3];
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxWorkItemDimensions, NULL);
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &maxWorkItemSizes, NULL);

	// Выводим информацию
	printf("Максимальная размерность рабочей группы: %u\n", maxWorkItemDimensions);
	printf("Максимальные размеры рабочей группы по каждому измерению: %u, %u, %u\n", maxWorkItemSizes[0], maxWorkItemSizes[1], maxWorkItemSizes[2]);


	// прочитать код из кернела

	FILE* program = fopen("kernel.cl", "r");

	int cnt = 0;
	char** code = (char**)malloc(150 * sizeof(char*));
	for (int i = 0; i < 150; i++)
		code[i] = (char*)malloc(100 * sizeof(int));

	while (fgets(code[cnt], 100, program) != NULL)
		cnt++;

	//for (int i = 0; i < cnt; i++)
		//printf("%s", code[i]);

	cl_program clprg = clCreateProgramWithSource(context, cnt, (const char**)code, NULL, &ret);

	// компиляция
	ret = clBuildProgram(clprg, 0, NULL, NULL, NULL, NULL);

	// данные о компиляции
	size_t l_size = 1000;
	void* log = malloc(1000);
	clGetProgramBuildInfo(clprg, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &l_size);
	clGetProgramBuildInfo(clprg, device_id, CL_PROGRAM_BUILD_LOG, l_size, log, NULL);
	printf("компиляция параллельного алгоритма:  %s", (char*)log);


	cl_kernel krnl = clCreateKernel(clprg, "find_det", &ret);

	if (ret != CL_SUCCESS)
		return 1;

	cl_command_queue command_queue;
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	cl_mem matrix_cl;
	cl_mem size_cl;

	matrix_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_double) * size * size, NULL, &ret);
	size_cl = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int), NULL, &ret);

	clEnqueueWriteBuffer(command_queue, matrix_cl, CL_TRUE, 0, sizeof(cl_double) * size * size, matrix_parallel, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, size_cl, CL_TRUE, 0, sizeof(cl_int), &size, 0, NULL, NULL);

	clSetKernelArg(krnl, 0, sizeof(cl_mem), (void*)&size_cl);
	clSetKernelArg(krnl, 1, sizeof(cl_mem), (void**)&matrix_cl);

	cl_uint work_dim = 1;
	size_t global_work_size[1] = { 1024 };

	start = omp_get_wtime();

	//запускаем для каждой следующей строки
	for (int i = 0; i < size; i++) {
		cl_mem iteration_cl;
		iteration_cl = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_int), NULL, &ret);
		clEnqueueWriteBuffer(command_queue, iteration_cl, CL_TRUE, 0, sizeof(cl_int), &i, 0, NULL, NULL);
		clSetKernelArg(krnl, 2, sizeof(cl_mem), (void*)&iteration_cl);

		// запуск программы
		cl_int error = clEnqueueNDRangeKernel(command_queue, krnl, work_dim, NULL, global_work_size, NULL, 0, NULL, NULL);

		if (error != CL_SUCCESS)
			return 1;
	}

	// читаем результат
	clEnqueueReadBuffer(command_queue, matrix_cl, CL_TRUE, 0, sizeof(cl_double) * size * size, matrix_parallel, 0, NULL, NULL);
	//clEnqueueReadBuffer(command_queue, size_cl, CL_TRUE, 0, sizeof(cl_int), (void*) & size, 0, NULL, NULL);

	double det = 1;

	for (int i = 0; i < size; i++) {
		det *= matrix_parallel[i * size + i];
	}

	end = omp_get_wtime();

	printf("Параллельный метод\n");
	printf("Определитель матрицы равен: %lf \n", det);
	printf("Время параллельного метода: %lf \n", end - start);

	free(matrix_parallel);

}
#include "CL/cl.h"
#define _CRT_SECURE_NO_WARNINGS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS // clCreateCommandQueue � ��������� ������� ��������, ��� �� ������� �����������
#include <omp.h>
#include <malloc.h>
#include <stdio.h>
#include <climits>
#include <clocale>
#include < string.h >

using namespace std;

//#define lld long double -- opencl �� ������������ ���� ��� ������
#define lld double 

//������� ������
int MatrixDeinit(lld** matrix, int size) {

	for (int i = 0; i < size; i++)
		free(matrix[i]);

	free(matrix);

	return 0;
}

// ����� ������� - �������
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
		// ���� ������������ ������� ����� ����
		if (matrix[i][i] < 0.0001)
			// ���� ������ ���� �� �������
			for (; matrix[i + k][i] != 0, i + k < size; k++);
		// ���� ��������� ��� ��������� � ���������� ����
		if (i + k == size)
			break;
		// ������ ������
		if (k != 0) {
			lld* buf = (lld*)malloc(size * sizeof(lld));
			memcpy(buf, matrix[i], size * sizeof(lld));
			memcpy(matrix[i], matrix[i + k], size * sizeof(lld));
			memcpy(matrix[i + k], buf, size * sizeof(lld));
			free(buf);
		}
	}
}

//������� ������������
lld FindDeterminant(lld** matrix, int size) {

	lld det = 1;


	// �������� ������� � ������������ ����
	for (int i = 0; i < size; i++) {
		//������� �� ��������� �������
		for (int m = 1; m + i < size; m++) {
			//������� �� ��������� ��������
			lld coef = matrix[i + m][i] / matrix[i][i];
			for (int j = i; j < size; j++) {
				matrix[i + m][j] -= matrix[i][j] * coef;
			}
		}
	}



	// ����������� ������� ��������� ����������� �������
	for (int i = 0; i < size; i++)
		det *= matrix[i][i];
	return det;
}

int main() {
	setlocale(LC_ALL, "Rus");

	char filename[] = "matrix_9_1024x1024f.txt";

	// ������ ����, �������� ������ ��� �������
	int size = 0;
	FILE* file;
	file = fopen(filename, "r");

	if (file == NULL) {
		printf("�� ������� ������� ����.\n");
		return 1;
	}

	fscanf(file, "%d", &size);
	int row = size;
	fscanf(file, "%d", &size);

	if (row != size) // ���� ������� �� �������� ����������
		return 1;

	lld** matrix = (lld**)malloc(sizeof(lld*) * size); //��������� ������ ��� ������
	lld* matrix_parallel = (lld*)malloc(sizeof(lld) * size * size); //��������� ������ ��� ������

	for (int i = 0; i < size; i++) { // ��������� ������ ��� ��������
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

	//���������������� �����

	double start = omp_get_wtime();

	lld determinant = FindDeterminant(matrix, size);

	double end = omp_get_wtime();

	printf("���������������� �����\n");
	printf("������������ ������� �����: %f \n", determinant);
	printf("����� ����������������� ������: %f \n", end - start);

	MatrixDeinit(matrix, size);
	// ����� ���������������� �����

	// ������������ �����

	cl_int ret; //���� ����� ���� - ��� ���������
	cl_platform_id platform_id;
	cl_uint ret_num_platforms;

	/* �������� ��������� ��������� */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);

	cl_device_id device_id;
	cl_uint ret_num_devices;

	/* �������� ��������� ���������� */
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

	if (ret != CL_SUCCESS)
		return 1;

	cl_context context;
	/* ������� �������� */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	//if (ret == NULL)
		//return 1;

	// �������� ���������� � ����������� ������� ������
	cl_uint maxWorkItemDimensions;
	size_t maxWorkItemSizes[3];
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxWorkItemDimensions, NULL);
	clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t) * 3, &maxWorkItemSizes, NULL);

	// ������� ����������
	printf("������������ ����������� ������� ������: %u\n", maxWorkItemDimensions);
	printf("������������ ������� ������� ������ �� ������� ���������: %u, %u, %u\n", maxWorkItemSizes[0], maxWorkItemSizes[1], maxWorkItemSizes[2]);


	// ��������� ��� �� �������

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

	// ����������
	ret = clBuildProgram(clprg, 0, NULL, NULL, NULL, NULL);

	// ������ � ����������
	size_t l_size = 1000;
	void* log = malloc(1000);
	clGetProgramBuildInfo(clprg, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &l_size);
	clGetProgramBuildInfo(clprg, device_id, CL_PROGRAM_BUILD_LOG, l_size, log, NULL);
	printf("���������� ������������� ���������:  %s", (char*)log);


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

	//��������� ��� ������ ��������� ������
	for (int i = 0; i < size; i++) {
		cl_mem iteration_cl;
		iteration_cl = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_int), NULL, &ret);
		clEnqueueWriteBuffer(command_queue, iteration_cl, CL_TRUE, 0, sizeof(cl_int), &i, 0, NULL, NULL);
		clSetKernelArg(krnl, 2, sizeof(cl_mem), (void*)&iteration_cl);

		// ������ ���������
		cl_int error = clEnqueueNDRangeKernel(command_queue, krnl, work_dim, NULL, global_work_size, NULL, 0, NULL, NULL);

		if (error != CL_SUCCESS)
			return 1;
	}

	// ������ ���������
	clEnqueueReadBuffer(command_queue, matrix_cl, CL_TRUE, 0, sizeof(cl_double) * size * size, matrix_parallel, 0, NULL, NULL);
	//clEnqueueReadBuffer(command_queue, size_cl, CL_TRUE, 0, sizeof(cl_int), (void*) & size, 0, NULL, NULL);

	double det = 1;

	for (int i = 0; i < size; i++) {
		det *= matrix_parallel[i * size + i];
	}

	end = omp_get_wtime();

	printf("������������ �����\n");
	printf("������������ ������� �����: %lf \n", det);
	printf("����� ������������� ������: %lf \n", end - start);

	free(matrix_parallel);

}
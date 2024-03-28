#define lld double 
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

kernel void find_det(__global int* size, __global lld* matrix, __global int* iterationNum)
{
	int x = get_global_id(0);
	
	int _size = *size;
	int _iterationNum = *iterationNum;
	
	if (x + _iterationNum + 1 > _size) return;
	
	int line = _iterationNum + 1 + x;
	
	lld coef = matrix[line * _size + _iterationNum] / matrix[_iterationNum * _size + _iterationNum];
	
	for (int j = _iterationNum; j < _size; j++)
	{
		matrix[line*_size+j] -= matrix[_iterationNum*_size + j];
	}
	return;
} 
#define _CRT_SECURE_NO_WARNINGS

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include "bst.h"

//MACROs
#define PORTS 4

//structures
typedef struct {
	FILE* fp;
	unsigned int* times_arr;
	int times_arr_size;
} ports_manager;

//functions declarations
unsigned int* getTimesArr(FILE* fp, int* size);
void sortArray(unsigned int* arr, int size);
void swapInt(int* a, int* b);
#include "main.h"

//functions declarations
unsigned int* getTimesArr(FILE* fp, int* size) {
	*size = 0;
	packet pkt;
	byte data_size;
	int num;
	//find amount of packets in the file
	while (1) {
		fscanf(fp, "%u %d %d %d %d ", &(pkt.time), (int*)&(pkt.da), (int*)&(pkt.sa), (int*)&(pkt.prio), (int*)&(pkt.data_length));
		data_size = pkt.data_length;
		assert(pkt.data = (byte*)malloc(data_size * sizeof(byte)));
		//finish scanning
		for (byte i = 0; i < data_size; i++) {
			fscanf(fp, "%d ", &num);
			pkt.data[i] = (byte)num;
		}
		//if end of file has been reached, break the loop
		if (fscanf(fp, "%d\n", (int*)&(pkt.checksum)) == EOF) {
			free(pkt.data);
			break;
		}
		(*size)++;
		free(pkt.data);
	}
	//set the pointer back to the beginning of the file
	fseek(fp, 0, SEEK_SET);
	//creating array
	unsigned int* times_arr;
	assert(times_arr = (unsigned int*)malloc(sizeof(unsigned int)*(*size)));
	//initializing array
	for (int i = 0; i < *size; i++) {
		packetRead(fp, &pkt);
		//fseek(fp, sizeof(packet), SEEK_CUR);
		times_arr[i] = pkt.time;
	}
	return times_arr;
}
void sortArray(unsigned int* arr, int size) {
	int i, j, min_index;
	//one by one move boundary of unsorted subarray 
	for (i = 0; i < size - 1; i++) {
		//find the minimum element in unsorted array 
		min_index = i;
		for (j = i + 1; j < size; j++)
			if (arr[j] < arr[min_index])
				min_index = j;
		//swap the found minimum element with the first element
		swapInt(&arr[min_index], &arr[i]);
	}
}
void swapInt(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
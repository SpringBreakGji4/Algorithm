#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX 1000

void insertion_sort(int *array, int size){
	int tem,i,j,key,index;
	for(i=1 ; i<size ; i++){
		key = array[i];
		index = i-1;
		while(index >= 0 && key < array[index]){
			array[index+1] = array[index];
			index --;
		}
		array[index+1] = key;
	}
}
void print_array(int *array, int size){
	int i;
	for(i=0 ; i<size ; i++){
		printf("%d ",array[i]);
	}
	printf("\n");
}
long long current_timestamp(){
	struct timeval time;
	gettimeofday(&time, NULL);
	long long milliseconds = time.tv_sec*1000L + time.tv_usec/1000;
	return milliseconds;
}
int main(){
	//struct timeval start, stop;
	long long start, end;
	int i, size = 0;
	int *input = (int *)malloc(sizeof(int)*MAX);
	FILE *file = fopen("test_case.txt", "r");
	while(fscanf(file,"%d",&input[size])==1){
                size++;
        }
        int *array = (int *)malloc(sizeof(int)*size);
        memcpy(array,input,sizeof(int)*size);
	printf("\n---Insertion sort---\n\n");	
	printf("Original array: ");
	print_array(array,size);
	printf("\n");
	//start = current_timestamp();
	insertion_sort(array, size);
	//end = current_timestamp();
	printf("Sorted array: ");
        print_array(array,size);
        printf("\n");
	fclose(file);
}


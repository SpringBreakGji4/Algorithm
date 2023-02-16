#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX 1000

void bubble_sort(int *array, int size){
	int tem,i,j;
	int done;
	for(i=0 ; i<size-1 ; i++){
		done = 0;
		for(j=0 ; j<size-i-1 ; j++){
			if(array[j] > array[j+1]){
				tem = array[j];
				array[j] = array[j+1];
				array[j+1] = tem;
				done = 1;
			}
		}
		if(done == 0){
			break;
		}		
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
	printf("\n---Bubble sort---\n\n");
	printf("Original array: ");
	print_array(array,size);
	printf("\n");
	//start = current_timestamp();
	bubble_sort(array, size);
	//end = current_timestamp();
	printf("Sorted array: ");
        print_array(array,size);
        printf("\n");
	fclose(file);
}

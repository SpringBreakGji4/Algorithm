#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX 1000

void selection_sort(int *array, int size){
	int smallest_index,i,j,tem;
	for(i=0 ; i<size-1 ; i++){
		smallest_index = i;
		for(j=i+1 ; j<size ; j++){
			if(array[j] < array[smallest_index]){
				smallest_index = j;
			}
		}
		tem = array[i];
		array[i] = array[smallest_index];
		array[smallest_index] = tem;
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
	printf("\n---Selection sort---\n\n");
	printf("Original array: ");
	print_array(array,size);
	printf("\n");
	//start = current_timestamp();
	selection_sort(array, size);
	//end = current_timestamp();
	printf("Sorted array: ");
        print_array(array,size);
        printf("\n");
	//printf("%lld\n",end-start);
	fclose(file);
}


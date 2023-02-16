#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX 1000
void print_array();
void swap(int *a, int *b){
	int tem = *a;
	*a = *b;
	*b = tem;
}
int medium(int *a, int *b, int *c){
	if((*a > *b && *a < *c) || (*a < *b && *a > *c)){
                swap(a,c);
        }
        else if((*b > *a && *b < *c) || (*b < *a && *b > *c)){
                swap(b,c);
        }
	return *c;
}
void quick_sort(int *array, int size){
	if(size <= 1){
		return;
	}
	else if(size == 2){
		if(*array > *(array+1)){
                        swap(array,(array+1));
                }
		return;
	}
	int small, large = 0, mid = size/2;
	int pivot = medium(&array[0],&array[mid],&array[size-1]);
	while(*(array+large) < pivot){
		large++;
	}
	small = large+1;
	while(small < size-1){
		while(*(array+small) > pivot){
			small++;
		}
		if(small == size-1){
			break;
		}
		swap(array+(large++),array+(small++));
	}
	swap(array+large,array+small);
	quick_sort(array,large);
	quick_sort(array+large+1,size-large-1);
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
	printf("\n---Quick sort---\n\n");	
	printf("Original array: ");
	print_array(array,size);
	printf("\n");
	//start = current_timestamp();
	quick_sort(array,size);
	//end = current_timestamp();
	printf("Sorted array: ");
        print_array(array,size);
        printf("\n");
	fclose(file);
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#define MAX 50000

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
long current_timestamp(){
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_nsec/1000;
}	
int check_array(int *array, int size){
	for(int i=1 ; i<size ; i++){
		if(array[i] < array[i-1]){
			return 0;
		}
	}
	return 1;
}
int main(){
	long start, end;
	int index = 1, size = 0;
	struct dirent* dir_file;
	int *input = (int *)malloc(sizeof(int)*MAX);
	DIR *dir;
	if((dir = opendir("../test_case/")) == NULL){
		fprintf(stderr, "Error: Failed to open testcase directory ...\n");
		return 1;
	}
	char name[100];
	FILE *file;
	start = current_timestamp();
	printf("\n---Quick sort---\n");
	while((dir_file = readdir(dir))){
		if((!strcmp(dir_file->d_name, ".")) || (!strcmp(dir_file->d_name, ".."))){
			continue;
		}
		printf("Test case %d ...", index);
		memset(name, 0, sizeof(char)*100);
		strcat(name, "../test_case/");
		strcat(name, dir_file->d_name);
	
		file = fopen(name, "r");
		size = 0;
		while(fscanf(file,"%d",&input[size])==1){
			size++;
		}

		int *array = (int *)malloc(sizeof(int)*size);
		memcpy(array,input,sizeof(int)*size);
		quick_sort(array,size);
		if(check_array(array, size)){
			printf(" Pass!\n");
		}
		else{
			printf(" Fail ...\n");
		}
		fclose(file);
		index++;
	}
	end = current_timestamp();
	printf("time: %ld msec\n",end - start);
}


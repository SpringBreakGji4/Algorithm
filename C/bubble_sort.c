#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
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
int check_array(int *array, int size){
	for(int i=1 ; i<size ; i++){
		if(array[i] < array[i-1]){
			return 0;
		}
	}
	return 1;
}
int main(){
	//struct timeval start, stop;
	long long start, end;
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
	printf("\n---Bubble sort---\n");
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
		//printf("Original array: ");
		//print_array(array,size);
		//printf("\n");
		bubble_sort(array, size);
		//printf("Sorted array: ");
		//print_array(array,size);
		//printf("\n");
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
}

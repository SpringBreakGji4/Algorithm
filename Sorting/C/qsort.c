#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#define MAX 50000

int cmp(const void *a, const void *b){
    int *c = (int *)a;
    int *d = (int *)b;
    return *c-*d;
    
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
	printf("\n---Built-in qsort---\n");
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
		qsort(array, size, sizeof(int),cmp);
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


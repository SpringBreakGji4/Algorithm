#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#define MAX 50000

void count_sort(int *array, int size, int exp, int offset){
	int *output = (int *)malloc(sizeof(int)*size);
	int *count = (int *)calloc(10, sizeof(int));
	int i;
	for(i=0 ; i<size ; i++){
		count[((array[i]+offset)/exp) % 10]++;
	}
	
	for(i=1 ; i<10 ; i++){
		count[i] += count[i-1];
	}
	
	for(i=size-1 ; i>=0 ; i--){
		output[count[((array[i]+offset)/exp)%10]-1] = array[i];
		count[((array[i]+offset)/exp)%10]--;
	}

	for(i=0 ; i<size ; i++){
		array[i] = output[i];
	}
	free(output);
	free(count);
}	

void radix_sort(int *array, int size){
	int max = array[0];
	int min = array[0];
	for(int i=1 ; i<size ; i++){
		max = array[i]>max?array[i]:max;
		min = array[i]<min?array[i]:min;
	}
	int offset = min<0?min*(-1):0;
	max += offset;
	for(int exp=1 ; max/exp > 0 ; exp *= 10){
		count_sort(array, size, exp, offset);
	}
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
	return time.tv_nsec/1000 + time.tv_sec*1000000;
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
	printf("\n---Radix sort---\n");
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
		radix_sort(array, size);
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

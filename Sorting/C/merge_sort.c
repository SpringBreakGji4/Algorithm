#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#define MAX 50000

void merge_sort(int *now_arr,int now_size, int *ans, int ans_size){
    //printf("now size: %d\n",now_size);
    if(now_size == 1){
        return;
    }
    int mid = now_size/2;
    
    int *left = now_arr;
    int left_size = mid;
    
    int *right = now_arr+mid;
    int right_size = now_size-mid;
        
    merge_sort(left, left_size, ans, left_size);
    merge_sort(right, right_size, ans+mid, right_size);
    int *L = left;
    int *R = right;
    int *buf = ans;
    while(L<left+left_size && R < right+right_size){
        if(*L < *R){
            *(buf++) = *(L++);
        }
        else{
            *(buf++) = *(R++);
        }
    }
    while(L<left+left_size){
        *(buf++) = *(L++);
    }
    while(R < right+right_size){
        *(buf++) = *(R++);
    }
    memcpy(now_arr,ans,ans_size*sizeof(*ans));
    
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
	return time.tv_nsec/1000+time.tv_sec*1000000;
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
	printf("\n---Merge sort---\n");
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
		int *buf = (int *)malloc(sizeof(int)*size);
		merge_sort(array, size, buf, size);
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


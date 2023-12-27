#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define MAX 1000

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
long long current_timestamp(){
	struct timeval time;
	gettimeofday(&time, NULL);
	long long milliseconds = time.tv_sec*1000L + time.tv_usec/1000;
	return milliseconds;
}
int main(){
	//struct timeval start, stop;
	long long start, end;
	int i, size=0;
	int *input = (int *)malloc(sizeof(int)*MAX);
	FILE *file = fopen("test_case.txt", "r");
	while(fscanf(file,"%d",&input[size])==1){
		size++;
	}
	int *array = (int *)malloc(sizeof(int)*size);
	memcpy(array,input,sizeof(int)*size);
	printf("\n---Merge sort---\n\n");	
	printf("Original array: ");
	print_array(array,size);
	printf("\n");
	//start = current_timestamp();
	int *buf = (int *)malloc(sizeof(int)*size);
	merge_sort(array, size, buf, size);
	//end = current_timestamp();
	printf("Sorted array: ");
        print_array(array,size);
        printf("\n");
	fclose(file);
}


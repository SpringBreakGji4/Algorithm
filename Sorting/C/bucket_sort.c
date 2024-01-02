#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <dirent.h>
#define MAX 50000
#define NUM_BUCKET 500
typedef struct Node{
	int data;
	struct Node* next;
} Node;

void insert(Node** bucket, int value){
	Node *newNode = (Node*)malloc(sizeof(Node));
	newNode->data = value;
	newNode->next = NULL;

	newNode->next = *bucket;
	*bucket = newNode;
}

void insertion_sort(Node **bucket){
	if(*bucket == NULL || (*bucket)->next == NULL){
		return;
	}
	
	Node *head = NULL;
	Node *current = *bucket;

	while(current != NULL){
		Node *next = current->next;
	
		if(head == NULL || head->data >= current->data){
			current->next = head;
			head = current;
		}	
		else{
			Node *tem = head;
			while(tem->next != NULL && tem->next->data < current->data){
				tem = tem->next;
			}
			current->next = tem->next;
			tem->next = current;
		}
		current = next;
	}
	*bucket = head;
}
void bucket_sort(int *array, int size){
	Node* buckets[NUM_BUCKET+1];
	for(int i=0 ; i<NUM_BUCKET+1 ; i++){
		buckets[i] = NULL;
	}
	
	int index;
	for(int i=0 ; i<size ; i++){
		index = (array[i]+20000)/NUM_BUCKET;
		insert(&buckets[index], array[i]);
	}
	
	for(int i=0 ; i<NUM_BUCKET+1 ; i++){
		insertion_sort(&buckets[i]);
	}
	
	index = 0;
	for(int i=0 ; i<NUM_BUCKET+1 ; i++){
		Node *current = buckets[i];
		while(current != NULL){
			array[index++] = current->data;
			Node* tem = current;
			current = current->next;
			free(tem);
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
	printf("\n---Bucket sort---\n");
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
		bucket_sort(array, size);
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
	printf("time: %lld msec\n",end - start);
}


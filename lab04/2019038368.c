#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);
void closeAll(int* array);


void DeleteTree(TREE* current){
    //왼쪽 트리 삭제
    if (current -> left_pointer != NULL){
        DeleteTree(current -> left_pointer);
    }
    //오른쪽 트리 삭제
    if (current -> right_pointer != NULL){
        DeleteTree(current -> right_pointer);
    }
    //현재 트리 노드 삭제
    free(current);
}
void printPreorder(TREE* current){
    //현재 트리 노드 출력
    fprintf(fout, "%d ", current -> data);
    //왼쪽 트리 출력 
    if (current -> left_pointer != NULL){ // 왼쪽에 값이 있다면 이동 후 출력한다. 왼쪽에 더이상 값이 없을때까지 반복한다.
        printPreorder(current -> left_pointer); 
    }
    //오른쪽 트리 출력
    if (current -> right_pointer != NULL){ // 왼쪽으로의 이동이 끝나면 다시 오른쪽으로 이동한다. 다시 이 프로세스를 반복한다.
        printPreorder(current -> right_pointer);
    }
}
void printInorder(TREE* current){
    if (current == NULL){ // 아무 값도 없다면 리턴한다. 
        return;
    }
    //왼쪽 트리 출력
    printInorder(current -> left_pointer); // 왼쪽 노드로 이동한다
    //현재 트리 노드 출력
    fprintf(fout, "%d ", current -> data); // 먼저 왼쪽 노드로 이동이 끝나면 현재 노드를 출력한다.
    //오른쪽 트리 출력
    printInorder(current -> right_pointer); // 오른쪽 노드로 이동한다 다시 반복한다.
}
void printPostorder(TREE* current){
    if (current == NULL){ // 현재 노드가 없다면 리턴한다.
        return;
    }
    //왼쪽 트리 출력
    printPostorder(current -> left_pointer); // 왼쪽 노드로 이동한다
    //오른쪽 트리 출력
    printPostorder(current -> right_pointer); // 오른쪽 노드로 이동한다
    //현재 트리 노드 출력
    fprintf(fout, "%d ", current -> data);  // 이 두가지를 다 실행하고 다시 돌아올시 현재 노드를 출력한다.
}
void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){
    //현재 트리 노드 데이터 설정
    current -> data = inserted_data_array[current_index];  // 현재의 노드에 그에 맞는 값을 입력한다.
    //if 왼쪽 노드 가능할 경우
    //왼쪽 노드 메모리 할당
    //왼쪽 노드에 대한 재귀
    if (current_index * 2 < length){ // 왼쪽 노드가 있는가?
        current -> left_pointer = malloc(sizeof(TREE)); // 있다면 말록 할당하고 
        CreateFromArray(current -> left_pointer, current_index * 2, inserted_data_array, length); // 재귀적으로 왼쪽 노드를 만든다.
    }
    //if 오른쪽 노드 가능할 경우
    //오른쪽 노드 메모리 할당
    //오른쪽 노드에 대한 재귀
    if (current_index * 2 + 1 < length){ // 위 왼쪽 노드와 비슷함
        current -> right_pointer = malloc(sizeof(TREE));
        CreateFromArray(current -> right_pointer, current_index * 2 + 1, inserted_data_array, length);
    }
}

void closeAll(int* array){
    free(array);
    fclose(fin);
    fclose(fout);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
    if (fscanf(fin, "%d", &input_data) != 1) { // 파일에서 숫자를 읽어 온다 실패시 에러 출력
        fprintf(fout, "Error: Failed to read an integer from the input file.\n");
        closeAll(array);
        return -1;
    }
	    array[i + 1] = input_data;
	}
	TREE* header = malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1);
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
    DeleteTree(header);
    closeAll(array);
	return;
}
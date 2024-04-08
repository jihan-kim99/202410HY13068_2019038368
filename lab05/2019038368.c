#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
    //if current == NULL일 경우 할당
		if (current == NULL) {
			current = (BSTNode*)malloc(sizeof(BSTNode));
			current->value = key;
			current->left = NULL;
			current->right = NULL;
		}
    //elif key < current->value 일 경우 recursive
		//키의 값이 현재 노드의 값보다 작다면 왼쪽으로 가야한다.
		else if (key < current->value) {
			current->left = insertNode(current->left, key);
		}
    //elif key > current->value 일 경우 recursive
		//키의 값이 현재 노드의 값보다 크다면 오른쪽으로 가야한다.
		else if (key > current->value) {
			current->right = insertNode(current->right, key);
		}
    //else 에러 크고나 작지 않음으로 같은 노드가 있다는 뜻
		else {
			fprintf(fout, "Insertion error : There is already %d in the tree\n", key);
		}
    //return current
    return current;
}
//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){
    //current == NULL일경우 return current
		if (current == NULL) {
			return current;
		}
    //if   key < current->value 일 경우 recursive
		//키의 값이 현재 노드의 값보다 작다면 왼쪽으로 가야한다.
		if (key < current->value) {
			current->left = deleteNode(current->left, key);
		}
    //elif key > current->value 일 경우 recursive
		//키의 값이 현재 노드의 값보다 크다면 오른쪽으로 가야한다.
		else if (key > current->value) {
			current->right = deleteNode(current->right, key);
		}
    //else 의 경우 4가지
		else {
    //  자식이 둘일 경우 -> 우측 자식의 가장 좌측 자식 찾기 -> 그 노드의 값으로 current->value 설정 -> 우측 자식의 가장 좌측 자식 삭제 recursive
			if (current->left != NULL && current->right != NULL) {
				BSTNode* temp = current->right;
				while (temp->left != NULL) {
					temp = temp->left;
				}
				current->value = temp->value;
				current->right = deleteNode(current->right, temp->value);
			}
    //  자식이 하나일 경우 -> free current -> return 자식
			else if (current->left == NULL) {
				BSTNode* temp = current->right;
				free(current);
				return temp;
			}
			else if (current->right == NULL) {
				BSTNode* temp = current->left;
				free(current);
				return temp;
			}
    //  자식이 없을 경우 -> free current -> return NULL
			else {
				free(current);
				return NULL;
			}
		}
	//return current
	return current;
}

BSTNode* findNode(BSTNode* current, int key){
    //current == NULL일경우 return current
		if(current == NULL){
				return current;
		}
    //if   key < current->value 일 경우 current->left에 대해 recursive
		if (key < current->value) {
			return findNode(current->left, key);
		}
    //elif key > current->value 일 경우 current->right에 대해 recursive
		else if (key > current->value) {
			return findNode(current->right, key);
		}
    //else return current
		else {
			return current;
		}
}
void printInorder(BSTNode* current){
		//current == NULL일경우 return
		if (current == NULL) {
			return;
		}
		//current->left에 대해 recursive
		printInorder(current->left);
		//current->value 출력
		fprintf(fout, "%d ", current->value);
		//current->right에 대해 recursive
		printInorder(current->right);
}

void deleteTree(BSTNode* current){
		//current == NULL일경우 return
		if (current == NULL) {
			return;
		}
		//current->left에 대해 recursive
		deleteTree(current->left);
		//current->right에 대해 recursive
		deleteTree(current->right);
		//free current
		free(current);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}
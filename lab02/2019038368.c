#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key;
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);

void Insert(LE* header, int inserted_key, int target_key){
    // 찾는 key가 없는 경우
    if (Find(header, target_key) == NULL){
        fprintf(fout, "Insertion %d Failed: can not find location\n", inserted_key);
        return;
    }
    // 이미 존재하는 경우
    else if (Find(header, inserted_key) != NULL){
        fprintf(fout, "Insertion %d Failed: the key already exist\n", inserted_key);
        return;
    }
    // 찾는 key가 있는 경우 & 넣을 키가 없는 경우 & 찾는 key가 마지막 노드인 경우
    else if (Find(header, target_key)->next_pointer == NULL){
        LE* inserted = (LE*)malloc(sizeof(LE)); // malloc을 사용한 메모리 할당
        inserted->key = inserted_key; // key값 저장
        inserted->next_pointer = NULL; // next_pointer NULL로 초기화
        Find(header, target_key)->next_pointer = inserted; // target_key의 next_pointer를 inserted로 변경
        return;
    }
    // 찾는 key 찾는 key가 있는 경우 & 넣을 키가 없는 경우 & 찾는 key가 마지막 노드가 아닌 경우 = 정상적인 경우
    else{
        LE* current = header; // current를 header로 설정
        while (1){
            if(current->key == target_key){ // key가 일치하면
                LE* inserted = (LE*)malloc(sizeof(LE)); // malloc을 사용한 메모리 할당
                inserted->key = inserted_key; // key값 저장
                inserted->next_pointer = current->next_pointer; // next_pointer를 target_key의 next_pointer로 설정
                current->next_pointer = inserted; // target_key의 next_pointer를 inserted로 변경
                return;
            }
            current = current->next_pointer; // key가 일치하지 않을 경우 다음 노드로 이동
        }
    }
    return;
}

void Delete(LE* header, int target_key){
    LE* previous = FindPrevious(header, target_key); // FindPrevious를 사용하여 previous 찾기
    LE* current = Find(header, target_key); // Find를 사용하여 current 찾기
    if(current == NULL){ // current가 NULL인 경우 존재하지 않는 노드이다
        fprintf(fout, "Deletion %d Failed:  node is not in the list\n", target_key);
    }
    else{ // current가 NULL이 아닌 경우 존재하는 노드이다
        previous->next_pointer = current->next_pointer; // previous의 next_pointer를 target_key의 next_pointer로 변경
        free(current); // target_key 메모리 해제
    }
    return;
}

LE* Find(LE* header, int target_key){
    LE* current = header; // header부터 시작
    while (1){
        if(current->key == target_key){ // key가 일치하면 반환 current 반환
            return current;
        }
        current = current->next_pointer; // 일치하지 않을 경우 다음 노드로 이동
        if(current == NULL){ // 다음 노드가 없는 즉 마지막 노드일 경우 NULL 반환
            return NULL;
        }
    }
    return NULL;
}

LE* FindPrevious(LE* header, int target_key){
    LE* current = header; // header부터 시작
    while (1){
        if(current->next_pointer->key == target_key){ // 다음 노드의 key가 일치하는 경우 current 반환
            return current;
        }
        else if(current->next_pointer->next_pointer == NULL){ // 마지막 노드일 경우 NULL 반환
            return current;
        }
        current = current->next_pointer; // 일치하지 않을 경우 다음 노드로 이동
    }
    return NULL;
}

void PrintPrevious(LE* header, int target_key){
    //FindPrevious를 사용하여 출력
    LE* previous = FindPrevious(header, target_key); // FindPrevious를 사용하여 previous 찾기
    if(previous == NULL){
        fprintf(fout, "Finding %d Failed: node is not on the list\n", target_key); // previous가 NULL인 경우 없는 노드
    }
    else if(previous->key == -1){ // previous가 header인 경우
        fprintf(fout, "previous node of %d is head\n", target_key);
    }
    else{ // previous가 header가 아닌 경우
        fprintf(fout, "previous node of %d is %d\n", target_key, previous->key);
    }
    return;
}

void PrintList(LE* header){
    //header부터 next_pointer를 따라가면서 출력
    LE* current = header->next_pointer; // header의 next_pointer부터 시작
    while (current != NULL){
        fprintf(fout, "%d ", current->key); // key값 출력
        current = current->next_pointer; // 다음 노드로 이동
    }
    if(header->next_pointer == NULL){
        fprintf(fout, "empty list\n"); // 노드가 없는 경우 empty 출력
        return;
    }
    fprintf(fout, "\n"); // 개행
    return;
}

LE* CreateList(){
    //malloc을 이용하여 header 생성
    LE * header = (LE*)malloc(sizeof(LE));
    //header의 key는 -1로 설정
    header->key = -1;
    //header의 next_pointer는 NULL로 설정
    header->next_pointer = NULL;
    //header 반환
    return header;
}

void DeleteList(LE* header){
    //free를 이용하여 header부터 끝까지 전부 해제
    LE* current = header; // header부터 시작
    LE* next = header->next_pointer; // header의 next_pointer부터 시작
    while (next != NULL){
        free(current); // current 메모리 해제
        current = next; // current를 next로 변경
        next = next->next_pointer; // next를 기존 next의 다음 노드로 변경
    }
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

  LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return 0;
}
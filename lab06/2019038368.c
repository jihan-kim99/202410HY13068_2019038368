#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);
void Swap(int *a, int *b);

void Swap(int *a, int *b){
    // 두 엘레멘트를 받아 swap
    int temp = *a;
    *a = *b;
    *b = temp;
}

Heap* CreateHeap(int heapSize){
    // heap malloc
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    // heap 총 사이즈 초기화
    heap->Capacity = heapSize;
    // heap size를 0으로 초기화
    heap->Size = 0;
    // heap의 elements 들 전체 할당
    heap->Elements = (int*)malloc(sizeof(int) * (heapSize + 1));
    // head element는 -INF로 초기화
    heap->Elements[0] = -INF;
    return heap;
}
void DeleteHeap(Heap* heap){
    // 먼저 heap->Elements free해줘야 한다
    free(heap->Elements);
    // heap free
    free(heap);
}
void Push(Heap *heap, int value){
    // heap이 꽉 찼을 경우
    if (heap->Size >= heap->Capacity) {
        fprintf(fout, "push error : heap is full\n");
        return;
    }
    // 이미 존재하는 value일 경우
    if (Find(heap, value)) {
        fprintf(fout, "push error : %d is already in the heap\n", value);
        return;
    }
    // heap->Elements 끝에 value 삽입
    heap->Elements[++heap->Size] = value;
    // 반복:
    //     if 부모_value < 현재_value
    //          swap (부모_pointer, 현재_pointer)
    //          현재_pointer = 부모_pointer
    int current = heap->Size;   // 맨 마지막부터 시작
    int parent = current / 2;   // 부모 index를 계산
    while (current > 1 && heap->Elements[parent] < heap->Elements[current]) { // 부모가 존재하고, 부모가 현재보다 작을 경우
        Swap(&heap->Elements[parent], &heap->Elements[current]); // 두 엘레멘트를 swap
        current = parent; // 현재 index를 부모 index로 변경
        parent = current / 2;  // 부모 index를 다시 계산후 반복
    }
}
bool Find(Heap *heap, int value){
    // 모든 heap->Elements를 돌면서 value가 있는지 확인
    for (int i = 1; i <= heap->Size; i++) {
        if (heap->Elements[i] == value) {
            // 있는경우 참을 반환
            return true;
        }
    }
    // 없어서 끝났을 경우 거짓을 반환
    return false;
}
int Pop(Heap* heap){

    // heap이 비어있을 경우
    if (heap->Size == 0) {
        fprintf(fout, "pop error : heap is empty\n");
        return -INF;
    }
    // heap->Elements[1]을 heap->Elements의 마지막 key와 변경
    int maxElement = heap->Elements[1];
    // 마지막 key를 첫번째 key로 변경
    Swap(&heap->Elements[1], &heap->Elements[heap->Size]);

    // Size 변경이 이루어 짐으로 마지막 키가 삭제된것과 같다 첫번째 키는 마지막키로 대체 됨
    heap->Size--;
    // 반복:
    //     자식_왼쪽_value = -INF
    //     자식_오른쪽_value = -INF
    //     if 자식_왼쪽_있음
    //         자식_왼쪽_value 설정
    //     if 자식_오른쪽_있음
    //         자식_오른쪽_value 설정
    //
    //     if 자식_왼쪽_value < 자식_오른쪽_value
    //     and 현재_value < 자식_오른쪽_value
    //         swap(현재_pointer, 자식_오른쪽_pointer)
    //         현재_pointer = 자식_오른쪽_pointer
    //     elif 자식_오른쪽_value < 자식_왼쪽_value
    //     and 현재_value < 자식_왼쪽_value
    //         swap(현재_pointer, 자식_왼쪽_pointer)
    //         현재_pointer = 자식_왼쪽_pointer
    // return 삭제한 key
    int current = 1; // 첫번째 부터 시작해서 트리를 재정렬한다
    while (1) {
        int left = current * 2; // 왼쪽 자식 index
        int right = current * 2 + 1; // 오른쪽 자식 index
        int left_value = -INF; // 자식의 초기 값은 -INF로 설정
        int right_value = -INF;
        if (left <= heap->Size) {
            left_value = heap->Elements[left]; // 왼쪽 자식이 존재할 경우 왼쪽 자식의 값을 설정
        }
        if (right <= heap->Size) {
            right_value = heap->Elements[right]; // 오른쪽 자식이 존재할 경우 오른쪽 자식의 값을 설정
        }
        if (left_value < right_value && heap->Elements[current] < right_value) { // 오른쪽 자식이 왼쪽 값보다 더 크고 현재 값보다 큰 경우
            Swap(&heap->Elements[current], &heap->Elements[right]); // 두 엘레멘트를 swap 이러면 더 큰 값이 부모가 된다
            current = right;
        } else if (right_value < left_value && heap->Elements[current] < left_value) { // 왼쪽 자식이 오른쪽 값보다 더 크고 현재 값보다 큰 경우
            Swap(&heap->Elements[current], &heap->Elements[left]);
            current = left;
        } else { // 더이상 자식이 없거나 자식이 현재 값보다 작을 경우
            break;
        }
    }
    return maxElement;
}
void PrintHeap(Heap* heap){
    // heap이 비어있을 경우
    if (heap->Size == 0) {
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    // heap->Elements 순서대로 print
    for (int i = 1; i <= heap->Size; i++) {
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				Push(maxHeap, key);
				break;
			case 'd':
				max_element = Pop(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "%d is not in the heap\n", key);
				break;
		}
	}
	DeleteHeap(maxHeap);

	return 0;
}
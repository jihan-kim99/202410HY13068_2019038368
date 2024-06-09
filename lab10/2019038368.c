#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    // 기본 배열 생성 함수
    Array* array = (Array*)malloc(sizeof(Array));
    array->size = size;
    array->values = (int*)malloc(sizeof(int) * size);
    return array;
}

void QuickSort(Array* array, int left, int right, int depth){
    //pivot 찾기
    //pivot 기준 왼쪽 Quicksort
    //pivot 기준 오른쪽 Quicksort
    if(left < right){
        int pivot = Partition(array, left, right, depth);
        QuickSort(array, left, pivot - 1, depth + 1);
        QuickSort(array, pivot + 1, right, depth + 1);
    }
}

int Partition(Array* array, int left, int right, int depth) {
    int pivot = array->values[right];  // 오른쪽 끝 값을 pivot으로 설정
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot); // 초기 값 한번 출력해주기
    PrintArray(array, left, right);
    
    int i = left - 1;  // i위치 초기화 첫번째 원소보다 전 원소
    int j = right;     // j위치 초기화 피봇으로 설정

    while (1) {
        // i 가  pivot 보다 작은 값을때 계속 오른쪽으로 이동
        do {
            i++;
        } while (array->values[i] < pivot);
        // 똑같이 j 가 pivot 보다 큰 값일때 계속 왼쪽으로 이동
        do {
            j--;
        } while (j >= left && array->values[j] > pivot);
        // 두 포인터가 만나면 종료
        if (i >= j) {
            break;
        }
        // 두 포인터가 만나지 않았고 여기 까지 왔다면 두 포인터의 값을 바꿔준다.
        swap(&array->values[i], &array->values[j]);
        PrintArray(array, left, right);
    }

    // 피봇 값이 들어갈 위치를 찾아서 피봇값을 넣어준다.
    if(i < right){
        swap(&array->values[i], &array->values[right]);
        PrintArray(array, left, right);    
    }
    return i;  // 피봇 위치 반환
}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    free(array->values);
    free(array);
}

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

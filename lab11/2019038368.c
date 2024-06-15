#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_cell;
	int* ptr_cell;
	int size_wall;
	bool* ptr_wall;
}DisjointSets;

DisjointSets* init(int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets);

void swap(int *x, int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	fscanf(fin, "%d", &num);

	DisjointSets* sets = init(num);
	createMaze(sets, num);
	printMaze(sets, num);

	freeMaze(sets);

	fclose(fin);
	fclose(fout);

	return 0;
}

DisjointSets* init(int num) {
    // num * num 으로 size_cell, ptr_cell 초기화
    // num * num * 2 로 size_wall, ptr_wall 초기화(i 번째 셀의 우측은 i*2, 하단은 i*2+1로 정의)
    // sets->size_wall-2 = 0 (최우측 하단은 출구)
		int i;
		DisjointSets* sets = (DisjointSets*)malloc(sizeof(DisjointSets)); // 할당
		sets->size_cell = num * num; // 크기 정의
		sets->ptr_cell = (int*)malloc(sizeof(int) * sets->size_cell); // 할당
		sets->size_wall = num * num * 2;
		sets->ptr_wall = (bool*)malloc(sizeof(bool) * sets->size_wall);

		for (i = 0; i < sets->size_cell; i++) {
			sets->ptr_cell[i] = i;
		}

		for (i = 0; i < sets->size_wall; i++) {
			sets->ptr_wall[i] = true;
		}

		sets->ptr_wall[sets->size_wall - 2] = false; // 출구

		return sets;
}

void Union(DisjointSets *sets, int i, int j) {
    //pi = find i
    //pj = find j
    //ptr_cell[pi] = pj

		int pi = find(sets, i);
		int pj = find(sets, j);

		sets->ptr_cell[pi] = pj;
}

int find(DisjointSets *sets, int i) {	
    //ptr_cell[i] 가 자기 자신이면 자기 자신을 반환
    //ptr_cell[i] 을 find(ptr_cell[i])로 갱신
    //ptr_cell[i] 반환

		if (sets->ptr_cell[i] == i) {
			return i;
		}
		sets->ptr_cell[i] = find(sets, sets->ptr_cell[i]);
		return sets->ptr_cell[i];
}

void shuffle(int* array, int num){
		// use random to shuffle array
		int i, j;
		for (i = num - 1; i > 1; i--) {
			j = rand() % (i + 1);
			swap(&array[i], &array[j]);
		}
}

void createMaze(DisjointSets *sets, int num) {
	//i번째 배열이 i로 차있는 num*num 만큼의 배열 선언

	int i;
	int* shuffle_array = (int*)malloc(sizeof(int) * num * num);

	for (i = 0; i < num * num; i++) {
		shuffle_array[i] = i;
	}

    //위의 배열 shuffle 
	shuffle(shuffle_array, num * num);

	//shuffle된 배열에서 처음부터 끝까지 뽑기

	for (i = 0; i < num * num; i++) {
		int x = shuffle_array[i] % num;
		int y = shuffle_array[i] / num;

	//  우측 벽 없앨 수 있을 때(maze 밖이 아니고, x + 1이 num을 넘지 않고, 같은 group이 아닐 때)
	//    union, 그 사이 벽 제거 

		if (x + 1 < num && find(sets, y * num + x) != find(sets, y * num + x + 1)) {
			Union(sets, y * num + x, y * num + x + 1);
			sets->ptr_wall[y * num * 2 + x * 2] = false;
		}

	//  하단 벽 없앨 수 있을 때(maze 밖이 아니고, y + 1이이 num을 넘지 않고, 같은 group이 아닐 때)
	//    union, 그 사이 벽 제거  

		if (y + 1 < num && find(sets, y * num + x) != find(sets, (y + 1) * num + x)) {
			Union(sets, y * num + x, (y + 1) * num + x);
			sets->ptr_wall[y * num * 2 + x * 2 + 1] = false;
		}
	}

	free(shuffle_array);
}

void printMaze(DisjointSets *sets, int num) {
    int i, j;
    
		// 윗부분 프린트
    for (i = 0; i < num; i++) {
        fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");

    for (i = 0; i < num; i++) {
				// 벽 프린트
        for (j = 0; j < num; j++) {
            if (j == 0) {
								if(i==0){
                	fprintf(fout, " ");
								}
								else{
                	fprintf(fout, "|");
								}
            }
            if (sets->ptr_wall[i * num * 2 + j * 2]) {
                fprintf(fout, "   |");
            } else {
                fprintf(fout, "    ");
            }
        }
        fprintf(fout, "\n");

				// 바닥 프린트
        for (j = 0; j < num; j++) {
            fprintf(fout, "+");
            if (sets->ptr_wall[i * num * 2 + j * 2 + 1]) {
                fprintf(fout, "---");
            } else {
                fprintf(fout, "   ");
            }
        }
        fprintf(fout, "+\n");
    }
}

void freeMaze(DisjointSets *sets) {
    free(sets->ptr_cell);
    free(sets->ptr_wall);
    free(sets);
}

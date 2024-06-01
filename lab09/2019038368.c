#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct Node {
	int index; //index of this node
	int dist;	//distance for heap
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** distMatrix;
	Node** nodes;
}Graph;

typedef struct Heap {
	int capacity;
	int size;
	Node** element;
}Heap;

//구현해야 하는 함수
Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
Node* Pop(Heap* heap);

//구현하면 좋은 함수
bool isEmpty(Heap* heap){
	
	return heap->size == 0;
}
void swap(Node** node1, Node** node2){
	Node* tmp = *node1;
	*node1 = *node2;
	*node2 = tmp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = CreateGraph(size);
	
	char tmp = 0;
	
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		g->distMatrix[node1 - 1][node2 - 1] = weight;
		tmp = fgetc(fin);
	}
	PrintShortestPath(g);

	DeleteGraph(g);
}

Graph* CreateGraph(int size) {
    //graph->nodes 의 index = i, dist = INF, prev = INF로 초기화

		Graph* graph = (Graph*)malloc(sizeof(Graph)); // Graph 구조체 동적할당
		graph->size = size; // 사이즈 초기화
		graph->distMatrix = (int**)malloc(sizeof(int*) * size); // distMatrix 안에 들어갈 int* 의 동적할당
		graph->nodes = (Node**)malloc(sizeof(Node*) * size); // nodes 안에 들어갈 Node* 의 동적할당
		for (int i = 0; i < size; i++) {
			graph->distMatrix[i] = (int*)malloc(sizeof(int) * size); // distMatrix 내부 int 동적할당
			graph->nodes[i] = (Node*)malloc(sizeof(Node)); // nodes 내부 동적할당
			graph->nodes[i]->index = i; // 초기화 작업
			graph->nodes[i]->dist = INF;
			graph->nodes[i]->prev = INF;
			for (int j = 0; j < size; j++) {
				graph->distMatrix[i][j] = INF; // 2차원 배열 초기화
			}
		}
    return graph;
}

void DeleteGraph(Graph* graph){
 // 먼저 distMatrix, nodes를 free
	for(int i = 0; i < graph->size; i++){
		free(graph->distMatrix[i]);
		free(graph->nodes[i]);
	}
	free(graph->distMatrix);
	free(graph->nodes);
	// 그 후 graph를 free
	free(graph);
	
}

Heap* CreateHeap(int heapSize) {
	// heap 생성, 초기화
		Heap* heap = (Heap*)malloc(sizeof(Heap));
		heap->capacity = heapSize;
		heap->size = 0;
		heap->element = (Node**)malloc(sizeof(Node*) * heapSize); // Node* 의 동적할당
		return heap;
}

void DeleteHeap(Heap* heap){
	free(heap->element);
	free(heap);
}

void PrintShortestPath(Graph* graph) {
    //0번 노드 dist 0으로 설정(시작점이므로)
    //힙 생성
    //0번 노드 추가
    //반복, !isEmpty:
    //  a = pop(heap)
    //  distOA = a->dist
    //  반복 b, 0 ~ graph->size:
    //      a==b -> continue
    //      distOB = b->dist
    //      만약 distOA + AB거리 < distOB:
    //          b->dist = distOA + AB거리
    //          b->prev = a
    //          push(b)

	graph->nodes[0]->dist = 0;
	Heap* heap = CreateHeap(graph->size);
	Push(heap, graph->nodes[0]);
	while (!isEmpty(heap)) {
		Node* a = Pop(heap);
		
		int distOA = a->dist;
		for (int b = 1; b < graph->size; b++) {
			if (a->index == b)
				continue;
			int distOB = graph->nodes[b]->dist;
			int edgeDist = graph->distMatrix[a->index][b];
			if (distOA + edgeDist < distOB) { // 새로운 거리가 더 짧으면 업데이트 진행
				graph->nodes[b]->dist = distOA + graph->distMatrix[a->index][b];
				graph->nodes[b]->prev = a->index;
				Push(heap, graph->nodes[b]);
			}
		}
	}
	// distMatrix 업데이트
	for (int i = 0; i < graph->size; i++) {
		for (int j = 0; j < graph->size; j++) {
			if (i != j) {
				graph->distMatrix[i][j] = graph->nodes[j]->dist;
			}
		}
	}
    //반복 b, 0 ~ graph->size:
    //   b->prev를 따라가면서 이전 노드 찾아서 순서 만들기
	for(int b = 1; b < graph->size; b++){
    int prev = b;
    int path[graph->size]; // 경로 저장
    int pathCost = 0; // 경로의 길이
		
    while(prev != INF){
        path[pathCost++] = prev;
        prev = graph->nodes[prev]->prev;
    }
    if (graph->nodes[b]->dist == INF) { // 전혀 갈수 없는 노드를 뜻하게 된다.
        fprintf(fout, "can not reach to node %d\n", b + 1);
        continue;
    }
    for (int i = pathCost - 1; i >= 0; i--) {
        fprintf(fout, "%d", path[i] + 1);
        if (i > 0) fprintf(fout, "->");
    }

    fprintf(fout, " (cost : %d)\n", graph->nodes[b]->dist);
	}

	//힙 삭제
	DeleteHeap(heap);
}

void Push(Heap* heap, Node* node) {
		if (heap->size == heap->capacity) { // 빈 공간이 없으면
        return;
    }
		for (int i = 0; i < heap->size; i++) { // 이미 있는 노드인 경우 리턴
        if (heap->element[i] == node) {
            return;
        }
    }
    //heap push, 이전 heap 과제를 참조
		heap->element[heap->size] = node;
		int current = heap->size;
		int parent = (current - 1) / 2;
		while (current > 0 && heap->element[current]->dist < heap->element[parent]->dist) {
			swap(&heap->element[current], &heap->element[parent]);
			current = parent;
			parent = (current - 1) / 2;
		}
		heap->size++;
}

Node* Pop(Heap *heap) {
	if (isEmpty(heap)) {
        return NULL;
    }
    //heap pop, 이전 heap 과제를 참조
	
	Node* result = heap->element[0];
	heap->size--;
	heap->element[0] = heap->element[heap->size];
	int current = 0;
	while (true) {
		int left = current * 2 + 1;
		int right = current * 2 + 2;
		int next = current;
		if (left < heap->size && heap->element[left]->dist < heap->element[next]->dist) {
			next = left;
		}
		if (right < heap->size && heap->element[right]->dist < heap->element[next]->dist) {
			next = right;
		}
		if (next == current) {
			break;
		}
		swap(&heap->element[current], &heap->element[next]);
		current = next;
	}
	return result;
}

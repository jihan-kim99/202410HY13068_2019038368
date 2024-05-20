#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

/*
구현해야 하는 함수들
*/
Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void TopologicalSort(Graph* G);
Queue* CreateQueue();
bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
/*
구현하면 좋은 함수들
*/
// Indegree 배열 설정
void SetIndegree(Graph* G, int* idg);
// Bubble sort도 무방
void SortNode(int* arr, int n);



void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){
    //graph malloc
    //matrix의 경우는 bool 2차원 array
    // -> malloc으로 bool* 우선 할당 후 for문으로 내부 bool 크기만큼 할당
    Graph *G = (Graph*)malloc(sizeof(Graph));   // Graph 구조체 동적할당
    G->size = n; // 초기화 작업 size = n
    G->node = nodes; // 노드는 이미 할당된 nodes를 가져다 쓰기
    G->matrix = (bool**)malloc(sizeof(bool*) * n); // matrix 동적할당
    for(int i = 0; i < n; i++){ 
        G->matrix[i] = (bool*)malloc(sizeof(bool) * n); // matrix 내부 동적할당
        for(int j = 0; j < n; j++){
            G->matrix[i][j] = false; // matrix 초기화
        }
    }

    return G;
}

void InsertEdge(Graph* G, int a, int b){
    // a, b값과 index값은 다르므로 G->node에서 찾기
    // 해당 값을 a', b'라 하면 G->matrix[a'][b'] = true
    // G->matrix[b'][a'] 안하는 이유는 '방향 그래프'라서서
    for(int i = 0; i < G->size; i++){
        if(G->node[i] == a){ // a값 찾기
            for(int j = 0; j < G->size; j++){
                if(G->node[j] == b){ // b값 찾기
                    G->matrix[i][j] = true; // a', b'에 true 할당
                    break;
                }
            }
            break;
        }
    }
}

void PrintGraph(Graph* G){
    // 출력(띄어쓰기에 주의)
    // 0 2 4 5 6
    // 2 0 0 0 1
    // 4 0 0 0 0
    // 5 1 0 1 0
    // 6 0 0 1 1
    // 위와 같은 형태로 출력이 목표(bool 은 %d로 0 혹은 1로 출력 가능)

    // 첫 줄은 노드들 출력
    fprintf(fout, "0 ");
    for(int i = 0; i < G->size; i++){
        fprintf(fout, "%d ", G->node[i]);
    }
    fprintf(fout, "\n");


    for(int i = 0; i < G->size; i++){
        fprintf(fout, "%d ", G->node[i]); // 첫 열은 노드 출력
        for(int j = 0; j < G->size; j++){
            fprintf(fout, "%d ", G->matrix[i][j]); // matrix 출력
        }
        fprintf(fout, "\n");
    }
}

void DeleteGraph(Graph* G){
    //G 내부의 malloc 포인터들부터 free 후 G free
    //matrix의 경우 for문으로 matrix[i]를 free 후 matrix를 free
    for(int i = 0; i < G->size; i++){
        free(G->matrix[i]); // 모든 matrix[i] free
    }
    free(G->node); // 할당된 노드를 free
    free(G); // Graph 구조체 free
}

void TopologicalSort(Graph* G){
    //Queue, printorder, printorder_size 초기화 -> printorder, printorder_size의 경우 오류 발생시 오류 메시지만을 출력하기 위함
    //indegree의 경우 setindegree 사용해서 초기화
    
    //indegree 가 0인 노드 인덱스를 queue에 삽입
    //queue가 비어있을 때까지
    //     Dequeue 후 해당 노드를 선행으로 하는 노드를 찾기(matrix[i][j] 이용) -> 그 노드의 indegree를 1개 줄이기
    //     만약 그 노드의 indegree가 0이면 queue에 삽입
    
    //사이클 판단(size, G->size와 비교)
    //에러메시지나 정상적인 값 출력
    
    //indegree, printorder, queue 전부 free(queue는 deletequeue 사용)
    Queue* Q = CreateQueue(); // 빈 queue 생성
    int* indegree = (int*)malloc(sizeof(int) * G->size); // indegree 동적할당
    int* printorder = (int*)malloc(sizeof(int) * G->size); // printorder 동적할당
    int printorder_size = 0; 
    SetIndegree(G, indegree); // Indegree 초기화
    for(int i = 0; i < G->size; i++){ // 시작시 indegree가 0인 노드를 queue에 삽입
        if(indegree[i] == 0){
            Enqueue(Q, i);
        }
    }
    while(!IsEmpty(Q)){ // 빌때까지
        int node = Dequeue(Q); // Dequeue
        printorder[printorder_size++] = node; // printorder에 삽입 후 size 증가
        for(int i = 0; i < G->size; i++){ // 가리키는 노드를 찾아서 그 노드의 indegree를 1 줄이기
            if(G->matrix[node][i] == true){
                indegree[i]--;
                if(indegree[i] == 0){ // 0이 되면 queue에 삽입
                    Enqueue(Q, i);
                }
            }
        }
        // empty queue가 될때 까지 반복
    }
    if(printorder_size != G->size){ 
        // 사이클 판단 0 이 안되는 것은 자신에게 돌아오는 사이클이 존재한다는 것
        fprintf(fout, "sorting error : cycle\n");
    }else{
        // 정상적인 값 출력
        for(int i = 0; i < G->size; i++){
            fprintf(fout, "%d ", G->node[printorder[i]]);
        }
    }
    free(indegree);
    free(printorder);
    DeleteQueue(Q);
}

Queue* CreateQueue(){
    //queue malloc
    //여기서 queue는 linked list로 구현
    Queue *Q = (Queue*)malloc(sizeof(Queue)); 
    Q->front = NULL;
    Q->rear = NULL;
    Q->size = 0;
    // queue 초기화 후 반환
    return Q;
}

bool IsEmpty(Queue* Q){
    //size 0인지 체크
    if(Q->size == 0) return true;
    return false;
}

int Dequeue(Queue* Q){
    //queue 맨 앞쪽의 노드를 꺼내기
    //size --
    //linked list때와 동일하게 Q->front 포인터를 갱신(빼면 비게될 경우 front, rear 둘 다 NULL로)

    // 맨 앞의 노드를 꺼내서
    QueueNode* temp = Q->front;
    // 저장
    int data = temp->data;
    // front를 다음 노드로 갱신
    Q->front = temp->next;
    // 만약 front 가 NULL이 되면 빈 queue이므로 rear도 NULL로
    if(Q->front == NULL) Q->rear = NULL;
    // free 후 size 감소
    free(temp);
    Q->size--;

    return data;
}

void Enqueue(Queue* Q, int X){
    //queue 맨 뒤에 노드를 삽입
    //size ++
    //linked list때와 동일하게 Q->rear->next와 Q->rear 포인터를 갱신(빈 queue에 추가할 경우 front, rear 둘 다 새로 만든 node로)
    // 새로운 노드 생성
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    // 값 저장, next는 NULL로
    newNode->data = X;
    newNode->next = NULL;
    Q->size++;
    if(Q->rear == NULL){ // 빈 queue일 경우
        Q->front = newNode;
        Q->rear = newNode;
    }else{
        // 마지막의 next를 새로운 노드로
        Q->rear->next = newNode;
        // 마지막을 새로운 노드로
        Q->rear = newNode;
    }
}

void DeleteQueue(Queue* Q){
    //IsEmpty일 때까지 Dequeue 후 free
    while(!IsEmpty(Q)){
        Dequeue(Q);
    }
    free(Q);
}

void SetIndegree(Graph* G, int* idg){
    //Indegree array를 초기화
    //2중 for문
    //G->matrix[j][i]가 true면 idg[i]++
    for(int i = 0; i < G->size; i++){
        for(int j = 0; j < G->size; j++){
            if(G->matrix[j][i] == true){
                idg[i]++;
            }
        }
    }
}

void SortNode(int* arr, int n){
    //입력값의 순서가 정해저 있지 않음. 따라서 정렬이 필요
    //간단한 버블 소트도 n이 작으므로 충분히 가능
    //버블 소트
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - i - 1; j++){
            if(arr[j] > arr[j + 1]){
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}


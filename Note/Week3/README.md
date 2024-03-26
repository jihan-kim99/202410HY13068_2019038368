# Week 3 Stack and Queue

Before We learned

- Array
- linked list
- Time compacity for each action(insert, delete, search, Access)

For this class will be Stack

## Stack

- Insertions and deletions can be performed at on end. only one end.
- Insert ~ push / delete ~ pop
- Last in First out

| 1   |     |     | 2    |     |     | 7   |
| --- | --- | --- | ---- | --- | --- | --- |
| 2   | pop | 3   | push | 2   |
| 3   | ->  | 4   | ->   | 3   |
| 4   |     | 5   |      | 4   |
| 5   |     | 6   |      | 5   |
| 6   |     |     |      | 6   |

Here is exaple code

```c
void f2(){
  return
}

void f1(){
  return
}

int main(){
  f1();
}
```

When the `f1()` is called the stack will be (f1), then f1() will call f2(). So the Stack will be (f2, f1), now the f2 will return and f1 will be returned.

```c
int factorial(int n){
  if (n==1)
    return n;
  return n*factorial(n-1)
}
int main(){
  return factorial(3)
}
```

## Queue

First In, First Out.
Insertion of back deletion on front

Enqueue -> Back ||||||| Front -> dequeue

Can be used in job scheduling

Priority Queue. When the job comes in can have priority. <- Next Class

## Stack and Queue Implementation

### Stack

#### Using Array

Using Array to Create Stack

make a pointer to the last index and operate Push and pop in that position only

```c
struct Stack{
  int top;
  int capacity;
  int *array;
}

struct Stack* createStack(int capacity){
  struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
  stack->capacity = capacity;
  stack->top = -1;
  stack->array = (int*)malloc(stack->capacity * sizeof(int));
  return stack;
}

int isFull(struct Stack* stack){
  return stack->top == stack->capacity - 1;
}

int isEmpty(struct Stack* stack){
  return stack->top == -1;
}

void push(struct Stack* stack, int item){
  if (isFull(stack))
    return;
  stack->array[++stack->top] = item; // 먼저 더해야 함 실행후 더하는 것이 아닌
  printf("%d pushed to stack\n", item);
}

int pop(struct Stack* stack){
  if (isEmpty(stack))
    return;
  return stack->array[stack->top--];
}

void main(){
  struct Stack* stack = createStack(100);
  push(stack, 10);
  push(stack, 20);
  push(stack, 30);
  printf("%d popped from stack\n", pop(stack));
}
```

#### Using Linked List

Using Linked list to Create Stack

Push and pop only between the first element header

```c
struct Node{
  int data;
  struct Node* next;
}

struct Node* newNode(int data){
  struct Node* node = (struct Node*)malloc(sizeof(struct Node));
  node->data = data;
  node->next = NULL;
  return node;
}

int isEmpty(struct Node* root){
  return !root;
}
,
void push(struct Node** root, int data){
  struct Node* node = newNode(data);
  node->next = *root;
  *root = node;
  printf("%d pushed to stack\n", data);
}

int pop(struct Node** root){
  if (isEmpty(*root))
    return INT_MIN;
  struct Node* temp = *root;
  *root = (*root)->next;
  int popped = temp->data;
  free(temp);
  return popped;
}

void main(){
  struct Node* root = NULL;
  push(&root, 10);
  push(&root, 20);
  push(&root, 30);
  printf("%d popped from stack\n", pop(&root));
}

```

### Queue

#### Linked List

```c
struct QNode{
  int data;
  struct QNode* next;
}

struct Queue{
  struct QNode *front, *rear;
}

struct QNode* newNode(int data){
  struct QNode* temp = (struct QNode*)malloc(sizeof(struct QNode));
  temp->data = data;
  temp->next = NULL;
  return temp;
}

struct Queue* createQueue(){
  struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
  q->front = q->rear = NULL;
  return q;
}

void enQueue(struct Queue* q, int data){
  struct QNode* temp = newNode(data);
  if (q->rear == NULL){
    q->front = q->rear = temp;
    return;
  }
  q->rear->next = temp;
  q->rear = temp;
}

void deQueue(struct Queue* q){
  if (q->front == NULL)
    return;
  struct QNode* temp = q->front;
  q->front = q->front->next;
  if (q->front == NULL)
    q->rear = NULL;
  free(temp);
}
```

#### Array

```c
struct Queue{
  int front, rear, size;
  unsigned capacity;
  int* array;
}

struct Queue* createQueue(unsigned capacity){
  struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
  queue->capacity = capacity;
  queue->front = queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (int*)malloc(queue->capacity * sizeof(int));
  return queue;
}

int isFull(struct Queue* queue){
  return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue){
  return (queue->size == 0);
}

void enQueue(struct Queue* queue, int item){
  if (isFull(queue))
    return;
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
  printf("%d enqueued to queue\n", item);
}

int deQueue(struct Queue* queue){
  if (isEmpty(queue))
    return INT_MIN;
  int item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity; // 맨 끝의 경우 대비
  queue->size = queue->size - 1;
  return item;
}

int main(){
  struct Queue* queue = createQueue(1000);
  enQueue(queue, 10);
  enQueue(queue, 20);
  enQueue(queue, 30);
  printf("%d dequeued from queue\n", deQueue(queue));
  return 0;
}
```

Now we know how to make the stack and queue using Array and Linked List

All of them has only constant compacity.

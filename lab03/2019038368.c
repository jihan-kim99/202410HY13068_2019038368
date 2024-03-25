#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);
void EndPhase(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
			Push(stack,input_str[i]-'0');
		}
		else{
			int b = Pop(stack);
			int a = Pop(stack);
			switch (input_str[i]){
				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;
				case '/' : 
          if (b == 0) {
            fprintf(fout, "\n");
            fprintf(fout, "error : invalid postfix expression, divide by zero");
						EndPhase(stack);
						return;
					} else {
						Push(stack, a/b); 
          	break;
					}
				case '%' :
          if (b == 0) {
            fprintf(fout, "\n");
            fprintf(fout, "error : invalid postfix expression, divide by zero");
						EndPhase(stack);
						return;
          } else {
						Push(stack, a%b);
						break;
					}
				default : break;
			}
		}
		fprintf(fout, "%d ", Top(stack));
		i++;
	}
  if (stack->top == 0) {
    fprintf(fout, "\n");
    fprintf(fout, "evaluation result : %d", Pop(stack));
  } else {
    fprintf(fout, "\n");
    fprintf(fout, "error : invalid postfix expression, %d numbers left in the stack", stack->top+1);
  }
	EndPhase(stack);
}

Stack* CreateStack(int max){
	// create stack
	Stack* S = (Stack*)malloc(sizeof(Stack));
	// create stack on the max size
	S->key = (int*)malloc(sizeof(int)*max);
	// set max size
	S->max_stack_size = max;
	// set top pointer
	S->top = -1;
	// return stack
	return S;
};

void Push(Stack* S, int X){
	// if stack is not full
	if(!IsFull(S)){
		S->top++;
		S->key[S->top] = X;
	}
	// if stack is full
	else{
		fprintf(fout, "error : invalid postfix expression, stack is full");
	}
};

int Pop(Stack* S){
	// If stack is not empty
	if(!IsEmpty(S)){
		// return top element and decrease top pointer
		S->top--;
		return S->key[S->top+1];
	}
	// if stack is empty
	else{
		// give error
		fprintf(fout, "error : invalid postfix expression, stack is empty");
		return -1;
	}
};

int Top(Stack* S){
	// if stack is not empty
	if(!IsEmpty(S)){
		// return top element	
		return S->key[S->top];
	}
	// if stack is empty
	else{
		// give error
		fprintf(fout, "Error: stack is empty\n");
		return -1;
	}
};

void DeleteStack(Stack* S){
	// free the key elements of the stack first, If free the stack before doing this 
	// will cause memory leak due not able to access the key elements
	free(S->key);
	free(S);
  // this is to check if the stack is deleted
  printf("Stack is deleted\n");
};

bool IsEmpty(Stack* S){
	// if the top is -1 then it is empty cause there will not be any in -1 index
	if (S->top == -1){
		return true;
	} 
	else {
		return false;
	};
};

bool IsFull(Stack* S){
	// if the top is equal to max_stack_size-1 then it is full
	if (S->top == S->max_stack_size-1){
		return true;
	} 
	else {
		return false;
	}
};

void EndPhase(Stack* S){
	// free the stack and close the files	
	fclose(fin);
	fclose(fout);
	DeleteStack(S);
};
#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode{
	Element element;
	AVLNode* left;
	AVLNode* right;
	int height;
};

AVLNode* Insert(Element X, AVLNode* T);
AVLNode* Delete(Element X, AVLNode* T);
AVLNode* rebalance(AVLNode* T);
AVLNode* UpdateHeight(AVLNode* T);
AVLNode* getMax(AVLNode* T);
AVLNode* RotateRight(AVLNode* node);
AVLNode* RotateLeft(AVLNode* node);
int getHeight(AVLNode* T);
void PrintInorder(AVLNode* T);
void DeleteTree(AVLNode* T);
int max(int a, int b){ // max 함수 추가
    return a > b ? a : b;
}


//구현하면 좋은 함수
//getHeight, max, updateHeight, rebalance

AVLNode* RotateRight(AVLNode* node){
    if(node == NULL){
        return node;
    }
    AVLNode* L = node->left; // 왼쪽 값
    if(L == NULL){
        return node;
    }
    
    node->left = L->right; // 노드의 왼쪽에 L의 오른쪽 노드를 부착
    L->right = node; // L의 오른쪽에 노드를 부착
    node->height = 1 + max(getHeight(node->left), getHeight(node->right)); //높히 업데이트
    L->height = 1 + max(getHeight(L->left), getHeight(L->right));
    
    //우회전
    //node->left를 L이라 할 때
    //p->left를 L->right로
    //L->right를 p로
    //p와 L의 height update
    //return L
    return L; // 이제 L이 Node의 위치로 변경
}

AVLNode* RotateLeft(AVLNode* node){
    // 좌회전의 정 반대
    if(node == NULL){
        return node;
    }
    AVLNode* R = node->right;
    if(R == NULL){
        return node;
    }

    node->right = R->left;
    R->left = node;
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    R->height = 1 + max(getHeight(R->left), getHeight(R->right));

    return R;

    //좌회전
    //node->right를 R이라 할 때
    //p->right를 R->left로
    //R->left를 p로
    //p와 R의 height update
    //return R
    //rotateRight에서 전부 반대방향
}

AVLNode* rebalance(AVLNode* T){
    if(T == NULL){
        return T;
    }
    int leftHeight = getHeight(T->left);
    int rightHeight = getHeight(T->right);
    // 높이를 비교하여 회전 필요가 있는지 확인
    if(leftHeight > rightHeight + 1){
        // 만약 LR 케이스면 좌회전 후 우회전ㄴ
        if(getHeight(T->left->right) > getHeight(T->left->left)){
            T->left = RotateLeft(T->left);
        }
        T = RotateRight(T);
    }
    else if(leftHeight + 1 < rightHeight){
        // 만약 RL 케이스면 우회전 후 좌회전
        if(getHeight(T->right->left) > getHeight(T->right->right)){
            T->right = RotateRight(T->right);
        }
        T = RotateLeft(T);
    }
    // 높이 갱신
    T = UpdateHeight(T);
    // if 왼쪽 높이 > 오른쪽 높이 + 1 일 경우
    //     만약 LR 케이스면 좌회전
    //     우회전
    // elif 왼쪽 높이 + 1 < 오른쪽 높이 일 경우 
    //     만약 RL 케이스면 우회전
    //     좌회전
    // 높이 갱신
    return T;
}

AVLNode* UpdateHeight(AVLNode* T){
    if(T == NULL){
        return T;
    }
    // 순회하면서 높이를 업데이트
    // 아래 자식 노드들을 전부 업데이트 후 현재 노드의 높이를 업데이트
    T->left = UpdateHeight(T->left);
    T->right = UpdateHeight(T->right);
    T->height = 1 + max(getHeight(T->left), getHeight(T->right));
    return T;
}

AVLNode* getMax(AVLNode* T){
    if(T == NULL){
        return T;
    }
    // 오른쪽이 제일 큰 노드임으로 계속 이동후 없으면 반환 순회적으로 제일 큰 노드를 찾음
    if(T->right == NULL){
        return T;
    }
    return getMax(T->right);
}

int getHeight(AVLNode* T){
    // 빈 노드의 경우 0 반환
    if(T == NULL){
        return 0;
    }else {
        // 높이를 반환
        return T->height;
    }
}


AVLNode* Insert(Element X, AVLNode* T){
    //NULL이면 AVLNode 생성 후 반환
    // NULL인 경우는 새로운 노드를 생성 할수 있다는 뜻
    if(T == NULL){
        T = (AVLNode*)malloc(sizeof(AVLNode));
        // fatal error
        if(T == NULL){
            fprintf(fout, "Out of space\n");
            exit(1);
        }
        else{
            // 새로운 노드를 생성하고 값을 넣어줌
            T->element = X;
            T->height = 1;
            T->left = T->right = NULL;
        }
    }
    else if(X == T->element){
        // 값이 같은 경우는 이미 존재하는 값이므로 에러 출력
        fprintf(fout, "insertion error : %d is already in the tree\n", X);
    }
    // 순회적으로 값을 찾아 들어갈 위치를 찾음
    else if(X < T->element){
        // 왼쪽으로 이동
        T->left = Insert(X, T->left);
        // 높이를 업데이트
        T = UpdateHeight(T);
        T = rebalance(T);
    }
    else if(X > T->element){
        T->right = Insert(X, T->right);
        T = UpdateHeight(T);
        T = rebalance(T);
    }
    return T;
}

AVLNode* Delete(Element X, AVLNode* T){
    // NULL이면 전체 트리를 다 돌았으나 X가 없는 값이므로 에러 출력
    if(T == NULL){
        fprintf(fout, "deletion error : %d is not in the tree\n", X);
        return T;
    }
    if(X < T->element){
        T->left = Delete(X, T->left);
        // 높이를 업데이트
        T = UpdateHeight(T);
        T = rebalance(T);
    }
    else if(X > T->element){
        T->right = Delete(X, T->right);
        T = UpdateHeight(T);
        T = rebalance(T);
    }
    else{
        // 같은 경우에는 삭제를 진행
        // 자식이 둘인 경우
        if(T->left != NULL && T->right != NULL){
            // 왼쪽의 제일 큰 값으로 대체
            AVLNode* temp = getMax(T->left);
            T->element = temp->element;
            T->left = Delete(temp->element, T->left);
            T = UpdateHeight(T);
            T = rebalance(T);
        }
        else{
            // 자식이 하나인 경우
            AVLNode* temp = T;
            if(T->left == NULL){
                T = T->right;
            }
            else if(T->right == NULL){
                T = T->left;
            }
            free(temp);
        }
        // 없는 경우는 아무것도 안해도 됨
    }
    //NULL이면 오류
    //Insert와 동일하게 recursive
    //T->element == X면 BST 삭제 과정
    //마지막으로 해당 노드를 서브트리의 루트로 rebalance
    return T;
}

void PrintInorder(AVLNode* T){
    if(T == NULL){
        return;
    }
    PrintInorder(T->left);
    fprintf(fout, "%d(%d) ", T->element, T->height);
    PrintInorder(T->right);
    //inorder로 출력
}

void DeleteTree(AVLNode* T){
    if(T == NULL){
        return;
    }
    DeleteTree(T->left);
    DeleteTree(T->right);
    free(T);
    //postorder로 free
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLNode* Tree = NULL;
	char cv;
	int key;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintInorder(Tree);
		fprintf(fout, "\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);

	return 0;
}

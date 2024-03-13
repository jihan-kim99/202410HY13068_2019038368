# week 2 Array, Linked List

Vector, Matrix, Tensor...

Most Data In the Deep learning is n-dimentional array.

can be Written in `R^3*100*100`

Let's learn how to handle this N-dimentional Array.

### Array

Given `int a[5]` there gonna be 5 of them. We want to know the time complexity for

1. Access i th element in a (\*(a+i)).
2. Find x in a.
3. Insert a new element x between a[1] and a[i+1].
4. Delete a[i] from a.

address store in array name (a) = address of first element.

#### Complexity

1. θ(1)
2. Find x in a.

- Worst Case θ(n) or O(n)
- Avg. Case θ(n),
- Best Case θ(1)

3. Insert a new element x between a[1] and a[i+1].

- need swift Worst Case θ(n),
- Avg. Case θ(n),
- Best Case θ(1)

4. Delete a[i] from a.

- Worst Case Delete the First one.

### Linked List

Data and next node address in one node.
Data can be Element and Next node will be pointer. This two combine become a structure.
The Name for Linked List is pointed at first node.  
No need to put in a linear place, Just anywhere and give the next address.

Let's ask the same question.

1. Access i th element in a (\*(a+i)).

- Worst Case at the end of the list. θ(n)
- Avg. Case θ(n)
- Best Case at the first node θ(1)

2. Find x in a.

Have to Go through each one and check element linearly.

- Worst Case at the end of the list. θ(n)
- Avg. Case θ(n)
- Best Case at the first node θ(1)

3. Insert a new element x between a[1] and a[i+1].

Set incoming node pointer to the next node then change the previous node to the incoming node.
Time Complexity: Given the accessing position, θ(1)

Much faster than the Array List.

4. Delete a[i] from a.

Same as Insert Only need to change the a[i-1] to a[i+1] then delete the a[i]
Time Complexity: θ(1)

### Memory Allocation

Arrays need a continuous space so it is static. Able to indexing, numerical operations.

Linked list can be in anywhere so it is dynamic. Faster insertion/deletion.

As we have done beforehand.  
Query:

- Access
- Search

Modification:

- Insert
- Delete

#### Search in Sorted List

Array need only θ(log N)
This is done using binary search.

Linked List cannot use binary due to it cannot use indexing and accessing the middle element already need θ(N)

### Type declaration for a linked list

```c
typedef struct Node *PtrToNode;
typedef int ElementType;
typedef PtrToNode Position;
typedef PtrToNode List;
struct Node{
  ElementType Element;
  Position Next;
};

List MakeEmpty();
int isEmpty( List L );
int isLast( Position P, List L);
Position Find (ElementType X, List L);
Position FindPrevious (ElementType X, List L);
void Delete(ElementType X, List L);
void Insert(ElementType X, List L, Position P);
void DeleteList( List L );
```

MakeEmpty();

```c
List MakeEmpty()
{
  List L;
  L = (List)malloc(sizeof(struct node));
  L->Element = 0; //header of the list
  L->Next=NULL;
  reutrn L;
}
```

isEmpty(); Return True when the List L is Empty

```c
int IsEmpty(List L)
{
  return L->Next == Null;
}
```

is Last returning true if P is last of the List

```c
int isLast( Position P, List L)
{
  return P->Next == Null;
}
```

etc....

### Different Linked list

Doubly linked list

```c
struct Node{
  ElementType Element;
  Position Next;
  Position Prev;
}
```

This way has both way to prev and next node.

Insert Algorithm for this

```c
void Insert(ElementType X, List L, Position P)
{
  // for the Singly Linked List
  Position TmpCell;
  TmpCell = malloc(sizeof(struct Node));
  if (TmpCell==NULL)
    FatalError("Ouf of Space;;;");
  TmpCell->Element = X;
  TmpCell->Next = P->Next;
  P->Next = TmpCell;
  // for the double Linked List will be much easier

  if(TmpCell->Next !=NULL)
    TmpCell->Next->Prev = TmpCell;
  TmpCell->Prev = P;
}
```

Rest of the Algorithm will be updated later.

## Conclude

We want to learn how to handle data.

Elements -> Set -> Giving Sequence and Linear Structure

Array / List

Processing for
Queue, Stack, Priority Queue

& Non-linear structure
Heap/tree/graph

In this semester we will go through all of them.

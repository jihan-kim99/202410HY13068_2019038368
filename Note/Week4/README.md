# Priority Queue

Highest Priority In, First out.

Dequeue = DeleteMax / DeleteMin

Binary Max/min

- binary tree
- partially ordered tree

root = Max/Min

## Complete Binary Tree

Binary tree with completely filled.

Bottom is exception.

Filled from left to right.

Children of A[k] = A[2k+1] & A[2k+2]

Parents of A[k] = A[$\frac{k-1}{2}$]

with depth d, The # of nodes n, the # of nodes $n_b$ at the bottom level.

```math
2^{d-1} \leq n \leq 2^d - 1
```

```math
\log_2{(n+1)} \leq d \leq \log_2{(n)} + 1
```

$$d = \Theta(\log{n}) $$

```math
1 \leq n_b \leq2^{d-1}
```

### Heap

Heap is one of the Complete binary tree.

Heap property

- parents are always bigger than the child.

### Heap Insert

When the new elements comes in
The new elements will be placed in the last of the elements will have to compare with the parents and forward.

```c
void heapify(int arr[], int n, int i) {
  int largest = i; // Initialize largest as root
  int left = 2 * i + 1; // left child
  int right = 2 * i + 2; // right child

  // If left child is larger than root
  if (left < n && arr[left] > arr[largest])
    largest = left;

  // If right child is larger than largest so far
  if (right < n && arr[right] > arr[largest])
    largest = right;

  // If largest is not root
  if (largest != i) {
    swap(arr[i], arr[largest]);

    // Recursively heapify the affected sub-tree
    heapify(arr, n, largest);
  }
}

```

Now Let's see how to do Insert
This will be percolate up with time complexity $O(d) = O(\log n)$

```c
void swap(int* a, int* b){
  int t = *a;
  *a = *b;
  *b = t;
}

void percolateUp(int arr[], int n, int i) {
  if (i == 0) {
    return;
  }

  int parent = (i - 1) / 2;
  if (arr[parent] < arr[i]) {
    swap(arr[i], arr[parent]);
    percolateUp(arr, n, parent);
  }
}

void insertHeap(int arr[], int n, int key) {
  // Increase the size of the heap
  n++;

  // Insert the new key at the end
  arr[n - 1] = key;

  // Percolate up
  percolateUp(arr, n, n - 1);
}
```

### Delete

Can easily find the the Biggest one. The root. Remove the root first.

```c
void deleteRoot(int arr[], int n) {
  if (n <= 0) {
    return;
  }

  // Replace root with the last element
  arr[0] = arr[n - 1];

  // Decrease the size of the heap
  n--;

  // Heapify the heap starting from the root
  heapify(arr, n, 0);
}

```

How to Build the Heap

```c
void buildHeap(int arr[], int n) {
  // Build heap (rearrange array)
  // Only need to do the first half of the elements

  for (int i = n / 2 - 1; i >= 0; i--)
    heapify(arr, n, i);
}

int main() {
  int arr[] = {12, 11, 13, 5, 6, 7};
  int n = sizeof(arr) / sizeof(arr[0]);

  buildHeap(arr, n);

  printf("Heap array: ");
  printArray(arr, n);

  return 0;
}
```

Time complexity will be only $\Theta(n)$

// 알고리즘 과제
// Exchange, Merge, Quick, Heap, Radix sort 구현
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int val;
    struct node *next;
}node;

typedef struct queue {
    node *front;
    node *rear;
}queue;

void Testing_case(int t_case[], int t_size);
int random_generator(int data[], int size);
void Exchage_sort(int data[], int size);
void Merge(int data[], int low, int mid, int high);
void Merge_sort(int data[], int low, int high);
void Quick_sort(int data[], int low, int high);
int Partition(int data[], int low, int high, int pivotpoint);
void MakeHeap(int data[], int size);
void Heap_sort(int data[], int size);
int IsEmpty(queue *q);
void Enqueue(queue *q, int data);
int Dequeue(queue *q);
void InitQueue(queue *q);
int get_digit(int data[], int size);
void Radix_sort(int data[], int size, int max_digit);

int main(int argc, char *argv[]) {
    int case1[100], case2[500], case3[5000], case4[10000];
    int size1 = 100, size2 = 500, size3 = 5000, size4 = 10000;
    srand(time(NULL));

    Testing_case(case1, size1);
    Testing_case(case2, size2);
    Testing_case(case3, size3);
    Testing_case(case4, size4);

    return 0;
}

void Testing_case(int t_case[], int t_size) {
    float total_time = 0;
    int repetition = 5;
    clock_t start, end;

    printf("\nTesting size = %d\n", t_size);

    // Exchange sort
    for(int i=0; i<repetition; i++){
        random_generator(t_case, t_size);
        start = clock();
        Exchage_sort(t_case, t_size);
        end = clock();
        total_time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    printf("Exchange sort's average time\t: %f\n", total_time / repetition);
    total_time = 0;

    // Merge sort
    for(int i=0; i<repetition; i++){
        random_generator(t_case, t_size);
        start = clock();
        Merge_sort(t_case, 0, t_size-1);
        end = clock();
        total_time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    printf("Merge sort's average time\t: %f\n", total_time / repetition);
    total_time = 0;

    // Quick sort
    for(int i=0; i<repetition; i++){
        random_generator(t_case, t_size);
        start = clock();
        Quick_sort(t_case, 0, t_size-1);
        end = clock();
        total_time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    printf("Quick sort's average time\t: %f\n", total_time / repetition);
    total_time = 0;

    // Heap sort
    for(int i=0; i<repetition; i++){
        random_generator(t_case, t_size);
        start = clock();
        Heap_sort(t_case, t_size);
        end = clock();
        total_time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    printf("Heap sort's average time\t: %f\n", total_time / repetition);
    total_time = 0;

    // Radix sort
    for(int i=0; i<repetition; i++){
        random_generator(t_case, t_size);
        int digit = get_digit(t_case, t_size);
        start = clock();
        Radix_sort(t_case, t_size, digit);
        end = clock();
        total_time += (float)(end - start)/CLOCKS_PER_SEC;
    }
    printf("Radix sort's average time\t: %f\n", total_time / repetition);
    total_time = 0;
}

int random_generator(int data[], int size) {
    for(int i=0; i<size; i++){
        data[i] = rand() % size;
    }
}

void Exchage_sort(int data[], int size) {
    int temp = 0;

    for(int i=0; i<size; i++) {
        for(int j=0; j<size-1-i; j++) {
            if(data[i]>data[j]) {
                temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}

void Merge(int data[], int low, int mid, int high) {
    int i = low, j = mid+1 , k = low;
    int *U = (int*)malloc(sizeof(int) * (high+1));
    
    while(i<=mid && j<=high) {
        if(data[i] < data[j]) {
            U[k] = data[i];
            i++;
        }
        else {
            U[k] = data[j];
            j++;
        }
        k++;
    }
    if (i > mid){
        for(int l=j; l<=high; l++) {
            U[k] = data[l];
            k++;
        }
    }
    else {
        for(int l=i; l<=mid; l++) {
            U[k] = data[l];
            k++;
        }
    }

    for(int l=low; l<=high; l++) {
        data[l] = U[l];
    }
    free(U);
}

void Merge_sort(int data[], int low, int high) {
    int mid = 0;
    
    if(low < high) {
        mid = (low + high) / 2;
        Merge_sort(data, low, mid);
        Merge_sort(data, mid+1, high);
        Merge(data, low, mid, high);
    }
}

int Partition(int data[], int low, int high, int pivotpoint) {
    int i, j, tmp;
    int pivotitem = data[low];
    j = low;
    for(i = low+1; i<=high; i++) {
        if(data[i]<pivotitem) {
            j++;
            tmp = data[i];
            data[i] = data[j];
            data[j] = tmp;
        }
    }
    pivotpoint = j;
    tmp = data[low];
    data[low] = data[pivotpoint];
    data[pivotpoint] = tmp;
    return pivotpoint;
}

void Quick_sort(int data[], int low, int high) {
    int pivotpoint = 0;

    if(high>low) {
        pivotpoint = Partition(data, low, high, pivotpoint);
        Quick_sort(data, low, pivotpoint-1);
        Quick_sort(data, pivotpoint+1, high);
    }
}

void MakeHeap(int data[], int size) {
    for(int i=1; i<size; i++) {
        int child = i;
        do {
            int root = (child - 1) / 2;
            if(data[root] < data[child]) {
                int tmp = data[root];
                data[root] = data[child];
                data[child] = tmp;
            }
            child = root;
        } while(child != 0);
    }
}

void Heap_sort(int data[], int size) {
    MakeHeap(data, size);

    for(int i=size-1; i>= 0; i--) {
        int tmp = data[0];
        data[0] = data[i];
        data[i] = tmp;
        int root = 0;
        int child = 1;
        do {
            child = 2*root + 1;
            if(child<i-1 && data[child]<data[child+1]) {
                child++;
            }
            if(child<i && data[root]<data[child]) {
                tmp = data[root];
                data[root] = data[child];
                data[child] = tmp;
            }
            root = child;
        } while(child<i);
    }
}

int IsEmpty(queue *q) {
    return q->front == NULL;
}

void Enqueue(queue *q, int val) {
    node *new_node = (node *)malloc(sizeof(node));
    new_node->val = val;
    new_node->next = NULL;

    if(IsEmpty(q)) {
        q->front = new_node;
    }
    else {
        q->rear->next = new_node;
    }
    q->rear = new_node;
}

int Dequeue(queue *q) {
    int val;
    node *n;
    if(IsEmpty(q)) {
        return -1;
    }
    n = q->front;
    val = n->val;
    q->front = n->next;
    free(n);

    return val;
}

void InitQueue(queue *q) {
    q->front = q->rear = NULL;
}

int get_digit(int data[], int size) {
    int max = 0, max_digit = 1;
    for(int i=0; i<size; i++) {
        if(max<data[i]) {
            max = data[i];
        }
    }

    while(max/10>0) {
        max /= 10;
        max_digit++;
    }

    return max_digit;
}

void Radix_sort(int data[], int size, int max_digit) {
    queue buckets[10];
    int factor = 1;

    for(int i=0; i<10; i++) {
        InitQueue(&buckets[i]);
    }

    for(int i=0; i<max_digit; i++) {
        for(int j=0; j<size; j++) {
            Enqueue(&buckets[(data[j]/factor)%10], data[j]);
        }

        for(int k=0,j=0; k<10; k++) {
            while(!IsEmpty(&buckets[k])) {
                data[j++] = Dequeue(&buckets[k]);
            }
        }
        factor *= 10;
    }

}
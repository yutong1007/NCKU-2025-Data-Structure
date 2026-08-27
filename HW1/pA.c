#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 對 array 中的每個 element a[i] 都找到左邊最近的比自己小的數
// 用 stack 存 index，確保 stake 裡面的值(a[index]) 是遞增的
// 每次讀到新的 a[i]，就把 stack 裡面比 a[i] 大的都 pop 出來
// 如果 stack 是空的，代表左邊沒有比 a[i] 小的數，輸出 -1
// 如果 stack 不空，stack top 的 index 就是左邊最近的比 a[i] 小的數，輸出 a[stack top]
// 最後把 i push 進 stack


// stack
typedef struct {
    int *data;
    int top; 
    int capacity;
} stack;

// create
stack* stack_create(int capacity) {
    stack *s = (stack *)malloc(sizeof(stack));
    s->data = (int *)malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

// pop
void stack_pop(stack *s) {
    if (s == NULL || s->top == -1) 
        return;
    s->top--;
}

// push
void stack_push(stack *s, int value) {
    if (s == NULL || s->top == s->capacity - 1) 
        return;
    s->data[++(s->top)] = value;
}

// top
int stack_top(const stack *s) {
    if (s == NULL || s->top == -1) 
        return -1;
    return s->data[s->top];
}

// is empty
bool stack_isempty(const stack *s) {
    if (s == NULL || s->top == -1) 
        return true;
    return false;
}

// free
void stack_free(stack *s) {
    if (s == NULL) 
        return;
    free(s->data);
    free(s);
}

// queue
typedef struct {
    int *buf;
    int cap;
    int head, tail; 
} queue;

// create
queue* queue_create(int capacity) {
    queue *q = (queue *)malloc(sizeof(queue));
    q->buf = (int *)malloc(capacity * sizeof(int));
    q->cap = capacity;
    q->head = q->tail = 0;
    return q;
}

// enqueue
void queue_enqueue(queue *q, int value) {
    if (q == NULL || (q->tail + 1) % q->cap == q->head) 
        return;
    q->buf[q->tail] = value;
    q->tail = (q->tail + 1) % q->cap;
}

// is empty
bool queue_isempty(const queue *q) {
    if (q == NULL || q->head == q->tail) 
        return true;
    return false;
}

// free
void queue_free(queue *q) {
    if (q == NULL) 
        return;
    free(q->buf);
    free(q);
}


int main() {
    int length;
    scanf("%d", &length);

    int *arr;
    arr = (int *)malloc(length * sizeof(int));
    for (int i = 0; i < length; i++) {
        scanf("%d", &arr[i]);
    }

    
    int *dist = (int *)malloc(length * sizeof(int));
    stack *s = stack_create(length);
    for (int i = 0; i < length; i++) {
        
        // 把 stack 裡面比 arr[i] 大的都 pop 出來
        while ( !stack_isempty(s) && arr[stack_top(s)] >= arr[i]) {
            stack_pop(s); 
        }

        // stack 剩下的就是比 arr[i] 小的數，stack 空的話就是沒有比較小的數
        if (!stack_isempty(s)) {
            dist[i] = i - stack_top(s);
        } 
        else {
            dist[i] = -1;
        }
        
        // 把 i push 進 stack
        stack_push(s, i);
    }
    for (int i = 0; i < length; i++) {
        printf("%d ", dist[i]);
    }
    printf("\n");

    queue *q = queue_create(length + 1);
    for (int i = 0; i < length; i++) {
        if (dist[i] != -1)
            queue_enqueue(q, dist[i]);
    }
    if (queue_isempty(q)) {
        printf("EMPTY\n");
    } 
    else {
        // 一個一個輸出 queue（FIFO）
        while (!queue_isempty(q)) {
            printf("%d ", q->buf[q->head]);
            q->head = (q->head + 1) % q->cap;
        }
        printf("\n");
    }

    free(arr);
    free(dist);
    stack_free(s);
    queue_free(q);
}
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXN (200000 + 200000 + 5)

// 輸入兩個數 N Q
// N 是序列長度 Q 是 operation 次數
// 接著輸入 Q 行 operation
// operation 有四種
// H x：在 head 前插入新節點 x
// T x：在 tail 後插入新節點 x
// A k x：在節點 k 後插入新節點 x
// B k x：在節點 k 前插入新節點 x
// D k：刪除節點 k
// MH k：把節點 k 移動到 head
// MT k：把節點 k 移動到 tail

// L 儲存的是每個節點（index）的左邊節點
// R 儲存的是每個節點（index）的右邊節點
// Val 儲存的是每個節點（index）的值
int L[MAXN], R[MAXN], Val[MAXN];
int head = 0, tail = 0;
int next_node;

// H
void H(int new_node, int x) {
    Val[new_node] = x;

    // list 是空的
    if (!head) {
        head = tail = new_node;
        L[new_node] = R[new_node] = 0;
    }
    // 把原本的 head 左邊接上 new_node，new_node 右邊接上原本的 head 左邊沒有
    else {
        L[head] = new_node;
        L[new_node] = 0;
        R[new_node] = head;
        head = new_node;
    }
}

// T
void T(int new_node, int x) {
    Val[new_node] = x;

    // list 是空的
    if (!tail) {
        head = tail = new_node;
        L[new_node] = R[new_node] = 0;
    }
    // 把原本的 tail 右邊接上 new_node，new_node 左邊接上原本的 tail 右邊沒有
    else {
        R[tail] = new_node;
        R[new_node] = 0;
        L[new_node] = tail;
        tail = new_node;
    }
}

// A
void A(int new_node, int x, int k) {
    Val[new_node] = x;

    // 如果 k 是 tail 的話直接插在後面，不用紀錄 k 的右邊
    if (k == tail) {
        R[tail] = new_node;
        R[new_node] = 0;
        L[new_node] = tail;
        tail = new_node;
    }
    // 紀錄原本 k 的右邊，將 k 的右邊接上 new_node，new_node 的左邊接上 k，new_node 的右邊接上原本 k 的右邊，原本 k 的右邊的左邊接上 new_node
    else {
        int k_right = R[k];
        R[k] = new_node;
        L[new_node] = k;
        R[new_node] = k_right;
        L[k_right] = new_node;
    }   
}

// B
void B(int new_node, int x, int k) {
    Val[new_node] = x;

    // 如果 k 是 head 的話直接插在前面，不用紀錄 k 的左邊
    if (k == head) {
        L[head] = new_node;
        L[new_node] = 0;
        R[new_node] = head;
        head = new_node;
    }
    //
    else {
        int k_left = L[k];
        L[k] = new_node;
        R[new_node] = k;
        L[new_node] = k_left;
        R[k_left] = new_node;
    }
}


// D
void D(int k){
    int k_left = L[k], k_right = R[k];
    
    // 同時接上 k 的左邊右邊
    if (k_left) {
        R[k_left] = k_right;
    } 
    else { 
        head = k_right;
    }

    if (k_right) { 
        L[k_right] = k_left; 
    }
    else { 
        tail = k_left;
    }
    L[k] = R[k] = 0;                      
}


// MH
void MH(int k){
    D(k);

    // 原本是空的
    if (!head){                 
        head = tail = k;
        L[k] = R[k] = 0;
        return;
    }

    // 和 H() 一樣的 code
    L[head] = k;
    L[k] = 0;
    R[k] = head;
    head = k;
}

// MT
void MT(int k){
    D(k);

    // 原本是空的
    if (!tail){
        head = tail = k;
        L[k] = R[k] = 0;
        return;
    }

    // 和 T() 一樣的 code
    R[tail] = k;
    R[k] = 0;
    L[k] = tail;
    tail = k;
}


int main() {
    int N, Q;
    char operation;
    scanf("%d %d", &N, &Q);

    // 建立 Doubly linked list
    for (int i = 1; i <= N; ++i) {
        int x; 
        scanf("%d", &x);
        Val[i] = x;
        L[i] = i - 1;
        R[i] = i + 1;
    }
    L[1] = 0; 
    R[N] = 0;
    head = 1; 
    tail = N;
    next_node = N;
    
    // Q 次 operation
    for (int i = 0; i < Q; i++) {
        scanf(" %c", &operation);
        if (operation == 'H') {
            int x;
            scanf(" %d", &x);
            H(++next_node, x);
        }
        else if (operation == 'T') {
            int x;
            scanf(" %d", &x);
            T(++next_node, x);
        }
        else if (operation == 'A') {
            int k, x;
            scanf(" %d %d", &k, &x);
            A(++next_node, x, k);
        }
        else if (operation == 'B') {
            int k, x;
            scanf(" %d %d", &k, &x);
            B(++next_node, x, k);
        }
        else if (operation == 'D') {
            int k;
            scanf(" %d", &k);
            D(k);
        }
        else if (operation == 'M') {
            char sub_op;
            scanf(" %c", &sub_op);
            if (sub_op == 'H') {
                int k;
                scanf(" %d", &k);
                MH(k);
            }
            else if (sub_op == 'T') {
                int k;
                scanf(" %d", &k);
                MT(k);
            }
        }
    }

    // 正向輸出
    if (!head) {
        printf("EMPTY\n");
    }
    else {
        int first = 1;
        for (int i = head; i; i = R[i]) {
            printf("%d ", Val[i]);
            first = 0;
    }
    printf("\n");
    }

    // 逆向輸出
    if (!tail) {
        printf("EMPTY\n");
    }
    else {
        int first = 1;
        for (int i = tail; i; i = L[i]) {
            printf("%d ", Val[i]);
            first = 0;
    }
    printf("\n");
    }    
}
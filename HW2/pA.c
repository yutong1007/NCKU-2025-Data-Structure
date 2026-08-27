#include <stdio.h>
#include <stdlib.h>

// 有一個 tower 會把最大值放在最上面（max heap properity)
// 輸入一個數字代表要進行幾次 operation
// operation 有三種 
// 0 X：加入 X 到 heap 然後維持 max heap 
// 1：取出 heap 中最大的兩個數，最大的 - 第二大的後得到的數再加入 heap 然後維持 max heap
// 2 idx delta：將 idx 的值後加上 delta，然後維持 max heap
// 最後按順序輸出 max heap


// 用 array 存 max heap
// parent (i - 1) / 2, left child 2i + 1, right child 2i + 2
long long *max_heap;

int size = 0;

// 維持 max heap
// shift up (這個位置的值比 parent 大時，往上比)
void shift_up(int idx) {
    // 持續往上比
    while (idx > 0) {
        int parent_idx = (idx - 1) / 2;
        // 如果 parent 的值比較小就交換，然後從 parent 的位置繼續往上比
        if (max_heap[idx] > max_heap[parent_idx]) {
            long long temp = max_heap[idx];
            max_heap[idx] = max_heap[parent_idx];
            max_heap[parent_idx] = temp;
            idx = parent_idx;
        } 
        // 比到當 parent 的值比較大時停止
        else {
            break;
        }
    } 
}

// shift down (這個位置的值比 child 小時，往下比)
void shift_down(int idx) {
    // 持續往下比
    while (1) {
        int left_child_idx = 2 * idx + 1;
        int right_child_idx = 2 * idx + 2;
        // 沒有 child 停止 (沒有 left child 就一定沒有 right child)
        if (left_child_idx >= size) {
            break;
        }

        // 先假設 left child 比較大，如果有 right child 而且還比較大的話就用 right child
        int largest_child_idx = left_child_idx;
        if (right_child_idx < size && max_heap[right_child_idx] > max_heap[left_child_idx]) {
            largest_child_idx = right_child_idx;
        }

        // 只需要和比較大的 child 比
        // 如果 child 的值比較大就交換，然後從 child 的位置繼續往上比
        if (max_heap[idx] < max_heap[largest_child_idx]) {
            long long temp = max_heap[idx];
            max_heap[idx] = max_heap[largest_child_idx];
            max_heap[largest_child_idx] = temp;
            idx = largest_child_idx;
        }
        // 比到當 child 的值比較小時停止 
        else {
            break;
        }
    }
}

// Add a Block(x)
void add_a_block(long long x) {
    max_heap[size] = x;
    size++;
    shift_up(size - 1);
}

// Battle and Merge()
void battle_and_merge() {
    // 取出最大值
    long long first_max = max_heap[0];
    max_heap[0] = max_heap[size - 1];
    size--;
    shift_down(0);

    // 取出第二大值
    long long second_max = max_heap[0];
    max_heap[0] = max_heap[size - 1];
    size--;
    shift_down(0);

    // 加入 new block
    long long new_block = first_max - second_max;
    add_a_block(new_block);
} 

// Change a Block
void change_a_block(int idx, long long delta) {
    max_heap[idx] += delta;
    
    // 如果 delta > 0，表示值變大了，只有可能這個 index 比 parent 大
    if (delta > 0) {
        shift_up(idx);
    } 
    // 如果 delta < 0，表示值變小了，只有可能這個 index 比 child 小
    else if (delta < 0) {
        shift_down(idx);
    }
}

int main() {
    int N;
    scanf("%d", &N);
    max_heap = malloc((N + 10) * sizeof(long long));

    for (int i = 0; i < N; i++) {
        int operation;
        scanf(" %d", &operation);

        if (operation == 0) {
            long long x;
            scanf(" %lld", &x);
            add_a_block(x);
        } 
        else if (operation == 1) {
            battle_and_merge();
        } 
        else if (operation == 2) {
            int idx;
            long long delta;
            scanf(" %d %lld", &idx, &delta);
            change_a_block(idx, delta);
        }
    }

    // 輸出
    for (int i = 0; i < size; i++) {
        if (i > 0) printf(" ");
            printf("%lld", max_heap[i]);
    }
    printf("\n");
}
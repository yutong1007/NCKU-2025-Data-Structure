#include "mst.h"
#include <stdlib.h> 

// 實作 minimum spanning tree

// edge
// 連接的兩個 node、權重 w、keep 表示是否保留這條 edge 進 MST、var 可以自由使用

// node
// 有這個 node 的 id、有幾條 edge、1D array 存所有 edge、var 當作用來判斷這個 node 是否被加入 MST（初始是 NULL，加入後設為 1）

// 因為有給一個起始點，所以用 Prim’s Algorithm
// 用一個 min heap 存目前可以走的 edge，每次從 heap 拿出最小權重的 edge 走下去
// 將這個 edge 的 keep = 1，走到的 node 的所有 edge 放進 heap
// 重複直到所有 node 都走過為止

// min heap
// data 是存 Edge* 的陣列、size 現在有幾個元素、capacity 最大容量
typedef struct {
    Edge **data;
    int size;
    int capacity;
} MinHeap;

// create
MinHeap* minheap_create(int capacity) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->data = (Edge **)malloc(capacity * sizeof(Edge *));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// insert
void minheap_insert(MinHeap *heap, Edge *edge) {
    // heap 滿了
    if (heap->size >= heap->capacity) 
        return;

    // 將 edge 放到最後面 size（最後）
    heap->data[heap->size] = edge;
    int i = heap->size;
    heap->size++;

    // 從最後面開始往上調整維持 min heap
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->data[parent]->w <= heap->data[i]->w) 
            break;
        Edge *temp = heap->data[parent];
        heap->data[parent] = heap->data[i];
        heap->data[i] = temp;
        i = parent;
    }
}

// delete min
Edge* minheap_delete_min(MinHeap *heap) {
    // heap 空了
    if (heap->size == 0) 
        return NULL;

    // 取出最小值（data array 第一個 Edge*），並將最後一個元素放到 root
    Edge *min_edge = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;

    // 從 root 開始往下調整維持 min heap
    int i = 0;
    while (1) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;
        if (left < heap->size && heap->data[left]->w < heap->data[smallest]->w) {
            smallest = left;
        }
        if (right < heap->size && heap->data[right]->w < heap->data[smallest]->w) {
            smallest = right;
        }
        if (smallest == i) 
            break;
        Edge *temp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = temp;
        i = smallest;
    }
    return min_edge;
}

void generate_mst(Node *node) {
    MinHeap *heap = minheap_create(MAX_EDGES);
    
    // 從起點 node 開始走，將這個 node 標為已加入 MST，並將這個 node 可以走的 edge 加入 min heap
    node->var = (void *) 1;
    for (int i = 0; i < node->edge_count; i++) {
        minheap_insert(heap, node->edges[i]);
    }

    // 繼續從 min heap 拿出最小權重的 edge，直到所有 node 都加入 MST 為止
    while (heap->size > 0) {
        Edge *edge = minheap_delete_min(heap);
        Node *u = edge->u;
        Node *v = edge->v;

        // 如果這條 edge 連接的兩個 node 有一個已經加入 MST，就跳過這條 edge
        if (u->var != NULL && v->var != NULL) {
            continue;
        }

        // 將這條 edge 標為已加入 MST
        edge->keep = 1;

        // 走這條 edge 走到下一個 node（看是走到 u 還是 v）
        Node *next_node;
        // u 加入了 MST，v 沒有
        if (u->var != NULL && v->var == NULL) {
            next_node = v;
        }
        // v 加入了 MST，u 沒有 
        else if (u->var == NULL && v->var != NULL) {
            next_node = u;
        } 
        next_node->var = (void *) 1;
        
        // 將走到的下一個 node 可以走的 edge 加入 min heap
        for (int i = 0; i < next_node->edge_count; i++) {
            Edge *next_edge = next_node->edges[i];

            // 判斷一條 edge 連接的另一個 node 是否已經加入 MST，沒加入才加入 min heap
            Node *other_node;
            if (next_edge->u == next_node) {
                other_node = next_edge->v;
            }
            else {
                other_node = next_edge->u;
            }
            if (other_node->var == NULL) {
                minheap_insert(heap, next_edge);
            }
        }
    }

}
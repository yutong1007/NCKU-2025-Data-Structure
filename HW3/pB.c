#include <stdio.h>
#include <stdlib.h>

// 輸出 N 個任務和 D 個依賴性（兩個任務之間）
// 接下來 D 行，每行兩個整數 u v，代表任務 u 必須在任務 v 之前完成
// 輸出所有任務是否可以按照順序組裝（沒有一組任務要互相等對方）

// 有向圖
// 每個任務是一個 vertex，依賴性是一條從 u 指向 v 的有向邊
// 最後判斷這個圖是否有環（有的話 1 沒有 0）

// 用 adjacency list 建構 graph
// 用 DFS 判斷有沒有環

// adjacency list
// 用 3 個 array 表示
// head[N] 是紀錄每個 vertex 的第一條 edge 的 index，-1代表沒有
// to_vertex[D] 紀錄第 D 條 edge 指向哪個 vertex，
// other_edge[D] 是紀錄這條 edge D 的起點還有沒有其他 edge 指向其他 vertex
int *head;
int *to_vertex;
int *other_edge; 

// color array 紀錄每個 vertex 的狀態：0 還沒走到過 1 在 DFS 路徑中 2 已完成
int *color;
int has_cycle = 0;
void DFS(int start) {
    // 從起始點開始走
    color[start] = 1;
    
    // e 是 DFS 要走的下一條 edge，當 e = other_edge[e] 時表示沒有其他 edge 可以走
    for (int e = head[start]; e != -1; e = other_edge[e]) {
        // v 是 DFS 沿著這條 edge 走到的下一個 vertex
        int v = to_vertex[e];
        if (color[v] == 0) {
            // 如果這個 vertex 還沒走過，繼續往下走
            DFS(v);
            // 已發現有 cycle 可以提早結束
            if (has_cycle == 1) 
                return;
        } 
        // 如果走回到在 DFS 路徑中的點，表示有 cycle
        else if (color[v] == 1) {
            has_cycle = 1;
            return;
        }
    }

    // 這個 vertex 已經走完
    color[start] = 2;
}

int main() {
    int N, D;
    scanf("%d %d", &N, &D);

    // adjacency list
    // 用 3 個 array 表示
    // head[N] 是紀錄每個 vertex 的第一條 edge 的 index
    // to_vertex[D] 紀錄第 D 條 edge 指向哪個 vertex，
    // other_edge[D] 是紀錄這條 edge D 的起點還有沒有其他 edge 指向其他 vertex
    head = (int*)malloc(sizeof(int) * N);
    to_vertex = (int*)malloc(sizeof(int) * D);
    other_edge = (int*)malloc(sizeof(int) * D);
    for (int i = 0; i < N; i++) 
        head[i] = -1;

    // 讀 D 條 edges 建構 graph
    for (int i = 0; i < D; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        // 順序要對
        // 第 i 條 edge 指到 v
        // 第 i 條 edge 的起點有一條 edge 指到其他 vertex
        // u vertex 有一條 edge 指到 v
        to_vertex[i] = v;
        other_edge[i] = head[u];  
        head[u] = i;
    }
    
    // 初始化 color array
    color = (int*)calloc(N, sizeof(int));  
    // 走 DFS 判斷有沒有環
    for (int i = 0; i < N && has_cycle != 1; i++) {
        if (color[i] == 0) 
            DFS(i);
    }

    printf("%d\n", has_cycle);
}
#include <stdio.h>
#include <stdlib.h> 
#include <stdbool.h>

// 輸入 M 和 N 代表這個迷宮的 row 和 column
// 接下來 M 行，每行有 N 個字元代表迷宮的架構
// B: 起點
// E: 出口
// #: 牆（不能走）
// .: 路（可以走）
// A − Z: 傳送門（可以走，走到同一個字母的另一個位置，所以最多出現 2 次）
// 輸出從 B 走到 E 最少需要多少步，如果無法到達則輸出 -1

// 用 BFS 找最短路徑，每一個可以走的位置是一個 node
// 從起點開始走，每次把能走到的位置（上下左右然後不是#）加入 queue，然後 pop queue 當作下一個走的位置
// 傳送門：記住每個字母的位置，走到該字母的話可以直接跳到另一個位置

// 用一個相同大小的 2D array 紀錄每個位置走到的步數，初始值為 -1（表示還沒走到過）

// queue
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

// dequeue
int queue_dequeue(queue *q) {
    if (q == NULL || queue_isempty(q)) 
        return -1;
    int value = q->buf[q->head];
    q->head = (q->head + 1) % q->cap;
    return value;
}

// free
void queue_free(queue *q) {
    if (q == NULL) 
        return;
    free(q->buf);
    free(q);
}

int M, N;
char **Maze;
int **dist;
int magic_hole[26][4];

// BFS
void BFS(int start_r, int start_c, int end_r, int end_c) {
    // 建立 queue（queue 是要放要走的 node，每個 node 有 x y 座標）
    queue *q = queue_create(M * N * 2);

    // 起點放進 queue
    queue_enqueue(q, start_r);
    queue_enqueue(q, start_c);
    dist[start_r][start_c] = 0;

    // 往四個方向走的
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};
    
    // 開始 BFS
    while (!queue_isempty(q)) {
        // 從 queue 拿出下一個要走的 node
        int r = queue_dequeue(q);
        int c = queue_dequeue(q);

        // 如果提前走到 E 可以先結束
        if (r == end_r && c == end_c) {
            break;
        }

        // 將上下左右的 node 放進 queue
        for (int d = 0; d < 4; d++) {
            int next_r = r + dr[d];
            int next_c = c + dc[d];

            // 旁邊是邊界
            if (next_r < 0 || next_r >= M || next_c < 0 || next_c >= N) continue;
            // 旁邊是牆
            if (Maze[next_r][next_c] == '#') continue;
            // 旁邊已經走過
            if (dist[next_r][next_c] != -1) continue;

            // 旁邊可以走，放進 queue，steps + 1
            dist[next_r][next_c] = dist[r][c] + 1;
            queue_enqueue(q, next_r);
            queue_enqueue(q, next_c);
        }

        // 如果現在走到的點是傳送門的話
        char ch = Maze[r][c];
        if (ch >= 'A' && ch <= 'Z' && ch != 'B' && ch != 'E') {
            int idx = ch - 'A';
            int r1 = magic_hole[idx][0];
            int c1 = magic_hole[idx][1];
            int r2 = magic_hole[idx][2];
            int c2 = magic_hole[idx][3];

            // 嘗試傳送到另一個座標（如果只有一個座標的話傳到原本的位置）
            if (r1 != -1 && (r1 != r || c1 != c) && dist[r1][c1] == -1) {
                dist[r1][c1] = dist[r][c] + 1;
                queue_enqueue(q, r1);
                queue_enqueue(q, c1);
                }
            if (r2 != -1 && (r2 != r || c2 != c) && dist[r2][c2] == -1) {
                dist[r2][c2] = dist[r][c] + 1;
                queue_enqueue(q, r2);
                queue_enqueue(q, c2);
                }
        }
    }
}

int main() {
    scanf("%d %d", &M, &N);

    // 迷宮架構
    Maze = (char**)malloc(M * sizeof(char*));
    for (int i = 0; i < M; i++) {
        Maze[i] = (char*)malloc((N + 1) * sizeof(char));
        scanf("%s", Maze[i]);
    }
    
    // dist 紀錄走到[i][j]的 steps
    dist = (int**)malloc(M * sizeof(int*));
    for (int i = 0; i < M; ++i) {
        dist[i] = (int*)malloc(N * sizeof(int));
        for (int j = 0; j < N; ++j) dist[i][j] = -1;
    }

    // 找起點 B 和出口 E
    int B_row, B_col, E_row, E_col;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (Maze[i][j] == 'B') {
                B_row = i;
                B_col = j;
            } 
            else if (Maze[i][j] == 'E') {
                E_row = i;
                E_col = j;
            }
        }
    }

    // 紀錄傳送門
    // 26 個字母，每個字母最多兩個位置 (row1, col1, row2, col2)
    for (int i = 0; i < 26; i++) {
        magic_hole[i][0] = -1;
        magic_hole[i][1] = -1;
        magic_hole[i][2] = -1;
        magic_hole[i][3] = -1;
    }
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (Maze[i][j] >= 'A' && Maze[i][j] <= 'Z') {
                // 這樣 index 剛好會是 A 擺在 0 B 擺在 1 ...
                int index = Maze[i][j] - 'A';

                // 如果第一個位置還沒被記錄，記錄在前兩個
                if (magic_hole[index][0] == -1) {
                    magic_hole[index][0] = i;
                    magic_hole[index][1] = j;
                } 
                else {
                    magic_hole[index][2] = i;
                    magic_hole[index][3] = j;
                }
            }
        }
    }

    // BFS
    BFS(B_row, B_col, E_row, E_col);

    // 輸出
    printf("%d\n", dist[E_row][E_col]);
}
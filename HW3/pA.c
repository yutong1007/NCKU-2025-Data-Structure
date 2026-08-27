#include <stdio.h>
#include <stdlib.h>

// 輸入 n 個村莊 m 座橋 q 次通報
// 接著輸入 m 行，每行兩個整數 u v，代表村莊 u 和 v 之間有一座橋
// 接著輸入 q 行，每行一個整數 e，代表橋 e 垮了（橋以輸入順序編號，從 1 開始）
// 能透過橋相連的村莊被視為同一個聯盟（單獨一個村莊也算）
// 每次通報會告訴哪一座橋垮了
// 輸出 q+1 行，第一行是所有橋都存在時的聯盟數量，接著第 i 行輸出是第 i 次通報後的聯盟數量

// Union-Find
// 先建好最終結果的圖，接著把橋一座一座加回去
// 如果 u v 本來不在同一個聯盟，加入橋後聯盟數量會減少 1
// 如果 u v 本來就在同一個聯盟，加入橋後聯盟數量不變
// 加回 i 座橋後的聯盟數量就是第 q-i 次通報後的聯盟數量


// parent 是每一個集合的 root，size 是每個集合的大小
int *parent, *size;

// 找到 x 的集合的 root
int find(int x) {
    // x 的 parent 還是 x 時，表示 x 是 root
    while (parent[x] != x) {
        // 持續往上找
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

// 合併集合（有橋連接兩個村莊時）
int unite(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);
    
    // 已經在同一個集合
    if (root_a == root_b)
        return 0;

    // 把小的集合合併到大的集合（直接把小的集合的 root 的 parent 指向大的集合的 root）
    if (size[root_a] < size[root_b]) {
        parent[root_a] = root_b;
        size[root_b] = size[root_b] + size[root_a];
    } 
    else {
        parent[root_b] = root_a;
        size[root_a] = size[root_a] + size[root_b];
    }
    return 1;
}


int main() {
    int n, m, q;
    scanf("%d %d %d", &n, &m, &q);

    // 初始化
    parent = (int*)malloc((n + 1) * sizeof(int));
    size = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; ++i) {
        parent[i] = i;
        size[i] = 1;
    }

    int bridge_u[m + 1];
    int bridge_v[m + 1];

    // 讀 m 座橋（1-based）
    for (int i = 1; i <= m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        bridge_u[i] = u;
        bridge_v[i] = v;
    }
    
    // q 次通報（紀錄會被刪掉的橋）num 紀錄第 i 次通報會垮掉的橋（bridge_uv 的 index）bridge 是標記這座橋會不會垮
    int *collapse_num = (int*)calloc(q + 1, sizeof(int));
    int *collapse_bridge = (int*)calloc(m + 1, sizeof(int));
    for (int i = 1; i <= q; i++) {
        int e;
        scanf("%d", &e);
        collapse_num[i] = e;
        collapse_bridge[e] = 1;
    }

    // 先把不會垮掉的橋加入 Union-Find
    int alliances = n;
    for (int i = 1; i <= m; i++) {
        // 如果這座橋沒有被標記會垮掉就加入
        if (collapse_bridge[i] != 1) {
            // 如果這座橋連接的兩個村莊本來不在同一個 Union（需要合併），加入橋後聯盟數量會減少 1
            if (unite(bridge_u[i], bridge_v[i]) == 1)
                alliances--;
        }
    }

    int ans[q + 1];
    ans[q] = alliances;

    // 反向把垮掉的橋加回來
    for (int i = q; i >= 1; --i) {
        int idx = collapse_num[i];
        if (unite(bridge_u[idx], bridge_v[idx]) == 1) 
            alliances--;
        ans[i - 1] = alliances; 
    }

    // 輸出（要從 0 開始因為要輸出橋還沒垮前）
    for (int i = 0; i <= q; i++) {
       printf("%d\n", ans[i]); 
    }
}
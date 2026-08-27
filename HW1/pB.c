#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAXN (1000000 + 5)

// 輸入兩個數字 N K
// 有 N 個小隊 每隊人數 >= k 人
// 輸入 N 個數字 ai 代表每隊人數
// 決定參賽隊伍的人數，使能參加大叫之聲的小隊員總數最大
// 輸出最多能有多少人參加

// 題目敘述有點難懂 QQ
/*
EX
N = 3, K = 2
ai = [3, 6, 10]

當每隊 2 人時：
a1：3 個人可以組 1 隊 (2 人參加)
a2：6 個人可以組 3 隊 (6 人參加)
a3：10 個人可以組 5 隊 (10 人參加)
最多 18 人參加

當每隊 3 人時：
a1：3 個人可以組 1 隊 (3 人參加)
a2：6 個人可以組 2 隊 (6 人參加)
a3：10 個人可以組 3 隊 (9 人參加)
最多 18 人參加

當每隊 4 人時：
a1：3 個人可以組 0 隊 (0 人參加)
a2：6 個人可以組 1 隊 (4 人參加)
a3：10 個人可以組 2 隊 (8 人參加)
最多 12 人參加
*/

// count[] 統計每個數字 (index) 出現幾次
// prefix_sum[] 儲存 count[] 的 prefix sum
int count[MAXN];
int prefix_sum[MAXN];

// 用 count 的前綴和可以找出有多少隊伍的人數在 L 和 R 之間

int main() {
    int N, K;
    scanf("%d %d", &N, &K);

    // 紀錄隊伍人數的最大值，因為只需要嘗試可能隊伍人數到最大值就好
    int max_ai = 0;

    // 統計
    for (int i = 0; i < N; i++) {
        int ai;
        scanf("%d", &ai);
        count[ai]++;
        if (ai > max_ai) {
            max_ai = ai;
        }
    }

    // 前綴和
    prefix_sum[0] = count[0];
    for (int i = 1; i <= max_ai; i++) {
        prefix_sum[i] = prefix_sum[i - 1] + count[i];
    }

    // 10^6^3 會大於 int
    long long ans = 0;

    // 嘗試 k < 每個可能的隊伍人數 < = max_ai
    for (int i = K; i <= max_ai; i++) {
        long long total = 0;
        for (int j = 1; j * i <= max_ai; j++) {
            
            // L = Pv
            // R = P(v+1) - 1
            int L = j * i;
            int R = (j + 1) * i - 1;
            if (R > max_ai) {
                R = max_ai;
            }

            int teams = prefix_sum[R] - prefix_sum[L - 1];
            total = total + (long long)teams * j * i;
        }
        if (total > ans) {
            ans = total;
        }
    }

    printf("%lld\n", ans);
}
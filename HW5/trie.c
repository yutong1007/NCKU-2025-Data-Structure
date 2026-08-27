#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 輸入一個長度 n 的字串和 k 個 words 的 dict
// 輸出：用 dict 裡面的 k 個 words 組成這個組串有幾種可能 (words 可以重複使用)
// 答案最後要 mod 10^9+7

// 把字串組合想像成切一刀
// ex：apple = f("a") + c("pple")
// apple combination = f(apple)
// f("") = 1
// f("a") = f("") + c("a")
// f("ap") = f("") + c("ap") + f("a") + c("p")
// ...
// 由前面的 f() 可以推得 f("目標字串")  

// 建 trie 紀錄 c("字串")
// 每個節點多紀錄一個 cnt 代表到這個 T2 字串有幾個
// 把 trie 反過來建，就不用每次都從 root 往下走到，而是從底下往上走一層
// 接著用 Dynamic Programming 從 f("") 解到 f("string")

#define MOD 1000000007
char string[5001];
int k;

// Node struct 
// next：對應的下一個子母 ex：next[0] = -1 代表這個 node 下一個走 "a" 的路徑是沒有的
// cnt：走到這個 node 組成的前綴有幾種組合方法
typedef struct {
    int next[26];
    int cnt;
}Node;

// Tire struct
typedef struct {
    Node *nodes;
    int size;
    int cap;
}Trie;

// trie initiation
void trie_init(Trie *trie, int cap) {
    trie->cap = cap;
    trie->size = 1;              
    trie->nodes = malloc(sizeof(Node) * cap);

    // root 走到下一個字母都是 -1
    for (int i = 0; i < 26; i++)
        trie->nodes[0].next[i] = -1;
    trie->nodes[0].cnt = 0;
}

// trie insert (反向)
void trie_insert(Trie *trie, char *word) {
    // 目前在 root
    int cur = 0;
    int len = strlen(word);

    // 反向建，所以從 len -1 開始
    for (int i = len - 1; i >= 0; i--) {
        // 字母轉成 idx 'a' 是 0, - 'a' 算 index
        int idx = word[i] - 'a';
        // 看走到下一個字母的路徑在不在
        if (trie->nodes[cur].next[idx] == -1) {
            if (trie->size >= trie->cap) {
                trie->cap *= 2;
                trie->nodes = realloc(trie->nodes, sizeof(Node) * trie->cap);
            }
            
            // 路徑不在，建一個新的 node 接上去
            int id = trie->size++;
            for (int k = 0; k < 26; k++)
                trie->nodes[id].next[k] = -1;
            trie->nodes[id].cnt = 0;
            trie->nodes[cur].next[idx] = id;
        }
        // 路徑在或不在都會往下一個字母走
        cur = trie->nodes[cur].next[idx];
    }
    // cur 已經走到這個 word 的最後，標記 cnt + 1
    trie->nodes[cur].cnt++;
}


int main() {
    scanf("%s", string);
    scanf("%d", &k);

    // 建 trie
    Trie trie;
    trie_init(&trie, 1000);
    for (int i = 0; i < k; i++) {
        char word[5001];
        scanf("%s", word);
        trie_insert(&trie, word);
    }

    // Dynamic Programming 找出 f("string")
    int n = (int)strlen(string);
    
    // dp[i] = string 前 i 個字母有幾種組合方法 f("string[i]")
    int *dp = (int*)calloc(n + 1, sizeof(int));
    dp[0] = 1;

    // 從左到右算完整個 string n (dp[n] == answer)
    for (int i = 1; i <= n; i++) {
        int node = 0;

        // dp 核心：將問題拆成子問題：dp[0...j-1] 和最後一個字母 j
        for (int j = i - 1; j >= 0; j--) {
            // 從 trie 找後綴 c("T2")
            int c = string[j] - 'a';
            int nxt = trie.nodes[node].next[c];
            if (nxt == -1) break;      
            node = nxt;

            if (trie.nodes[node].cnt > 0) {
                // 用 long long 才不會 overflow
                long long add = (long long)dp[j] * (long long)trie.nodes[node].cnt;
                dp[i] = (dp[i] + (int)(add % MOD)) % MOD;
            }
        }
    }

    printf("%d\n", dp[n]);
}
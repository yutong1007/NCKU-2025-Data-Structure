#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_KEY 10000

// F heap
// 持續輸入四種 operation，並維持 F heap 特性
// insert key：插入一個 key 值的 node（不會觸發 Cascating Cut）
// delete key：刪除這個 key 值的 node（有機會觸發 Cascating Cut）
// decrease key value：將 key 值得 node 的 key 減少 value（有機會觸發 Cascating Cut）
// extract-min：刪除最小 key 值的 node（有機會觸發 Cascating Cut）
// exit
// 最後輸出 F heap 的結構，格式：
// 每一棵樹使用層序遍歷 (level-order traversal) 輸出節點的 key 值。
// 每一棵樹的遍歷順序為依照度數由小到大，度數相同則依照 key 值由小到大
// 每一棵樹的遍歷都輸出一行，每一行的第一個數字表示 root 的 key 值，接下來依序輸出每一層子樹的 key 值，同一層子樹的 key 值須由小到大輸出。
// 如果 F-heap 裡面沒有任何節點，則什麼都不需要輸出。


// F heap node structure
// degree：child 數量
// childcut：0/1 代表是否曾經失去過 child
// left right：sibling

typedef struct Fheap_Node {
    int key;
    int degree;
    int childcut;
    struct Fheap_Node* parent;
    struct Fheap_Node* child;
    struct Fheap_Node* left;
    struct Fheap_Node* right;
} Fheap_Node;

// F heap structure
// 很多 tree，每個 tree 的 root 互相串聯成一個 circular doubly linked list
// min：指向 root list 中 key 最小的 root
// n：node 數量
typedef struct Fheap {
    Fheap_Node* min;
    int n;
} Fheap;

// 存指向這個 key value (index) 的 pointer 的一個表格
Fheap_Node* key_table[MAX_KEY + 1];

// 將 node 加入 root list 的 function（insert, cascading cut, extract min 等都會用）
void insert_rootlist(Fheap* F_heap, Fheap_Node* node) {
    
    // 確保只有自己
    node->left = node;
    node->right = node;

    // 如果 F heap 是空的，直接把 node 設為 min
    if (F_heap->min == NULL) {
        F_heap->min = node;
        return;
    }

    // insert node 到 root list
    Fheap_Node* min = F_heap->min;
    node->left = min;
    node->right = min->right;
    min->right->left = node;
    min->right = node;

    // 更新 min
    if (node->key < F_heap->min->key) {
        F_heap->min = node;
    }
}

// insert
// 建一個新的 node，key 為輸入的 key
Fheap_Node* create_node(int key) {
    Fheap_Node* new_node = (Fheap_Node*)malloc(sizeof(Fheap_Node));
    new_node->key = key;
    new_node->degree = 0;
    new_node->childcut = 0;
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    return new_node;
}

void insert(int key, Fheap* F_heap) {
    Fheap_Node* new_node = create_node(key);
    insert_rootlist(F_heap, new_node);
    F_heap->n++;
    key_table[key] = new_node;
}

// cascading cut（decrease 和 delete 可能會用到）
// cut 把某個 node 從 parent 的 child list 拿掉，然後加回到 root list
void cut(Fheap* F_heap, Fheap_Node* node, Fheap_Node* parent) {
    if (node->right == node)
        parent->child = NULL;
    // 刪掉 node
    else {
        if (parent->child == node)
            parent->child = node->right;
        node->left->right = node->right;
        node->right->left = node->left;
    }
    parent->degree--;

    node->parent = NULL;
    node->childcut = 0;

    // 把 node 加回 root list
    insert_rootlist(F_heap, node);
}

void cascading_cut(Fheap* F_heap, Fheap_Node* node) {
    Fheap_Node* parent = node->parent;
    if (parent != NULL) {
        if (node->childcut == 0) {
            node->childcut = 1;
        } 
        else {
            cut(F_heap, node, parent);
            // 持續遞迴做 cascading cut
            cascading_cut(F_heap, parent);
        }
    }
}

// 做 consolidate, 輸出時都會用到
int compare_key(const void* a, const void* b) {
    Fheap_Node* x = *(Fheap_Node**)a;
    Fheap_Node* y = *(Fheap_Node**)b;
    return x->key - y->key;
}

// consolidate（degree 相同的 tree 合併、有多個 degree 相同的 tree 時先 merge key 較小的）
void consolidate(Fheap* F_heap) {
    // 用一個 array 紀錄 degree (index) 對應的 root node
    Fheap_Node* A[60];
    memset(A, 0, sizeof(A));

    // root list 的所有 root
    int roots_num = 0;
    Fheap_Node* roots[1000];
    Fheap_Node* root = F_heap->min;
    if (root != NULL) {
        do {
            roots[roots_num++] = root;
            //printf("root before consolidate: %d (deg=%d)\n", root->key, root->degree);
            root = root->right;
        } while (root != F_heap->min);
    }

    // 先按照 key 排序，這樣有多個 degree 相同的 tree 時就可以先 merge key 較小的
    qsort(roots, roots_num, sizeof(Fheap_Node*), compare_key);

    // 逐一處理每個 root
    for (int i = 0; i < roots_num; i++) {
        Fheap_Node* one_root = roots[i]; 

        // 已經被合併成別的 tree 的話跳過
        if (one_root->parent != NULL) continue;

        int d = one_root->degree;
        // 和其它同 degree 的 root 合併
        while (A[d] != NULL) {
            Fheap_Node* other_root = A[d]; 

            if (other_root->key < one_root->key) {
                Fheap_Node* tmp = one_root;
                one_root = other_root;
                other_root = tmp;
            }

            // 把 key 比較大的 node 從 root list 移除
            other_root->left->right = other_root->right;
            other_root->right->left = other_root->left;

            // 把 key 比較大的 node 變成比較小的 node 的 child
            other_root->parent = one_root;
            other_root->childcut = 0;

            // 原本沒有 child
            if (one_root->child == NULL) {
                one_root->child = other_root;
                other_root->left = other_root;
                other_root->right = other_root;
            } 
            // 原本有其他 child
            else {
                Fheap_Node* other_child = one_root->child;
                other_root->left = other_child;
                other_root->right = other_child->right;
                other_child->right->left = other_root;
                other_child->right = other_root;
            }

            // 把這個 degree 的資訊都刪掉（合併後不會出現這個 degree 的 tree）
            one_root->degree++;
            A[d] = NULL;
            d++;
        }
        A[d] = one_root;
    }

    // 重新建立 root list 並更新 min
    F_heap->min = NULL;

    for (int i = 0; i < 60; i++) {
        if (A[i] != NULL) {
            Fheap_Node* new_root = A[i];
            insert_rootlist(F_heap, new_root);
        }
    }
}

// decrease（把某個 node 的 key 減少 value）
void decrease(Fheap* F_heap, int key, int value) {
    // 從 key table 找到要 decrease 的 node
    Fheap_Node* decrease_node = key_table[key];
    if (decrease_node == NULL) return;

    int old_key = decrease_node->key;
    int new_key = old_key - value;
    
    decrease_node->key -= value;

    // 看使否發生 cascading cut 
    Fheap_Node* parent = decrease_node->parent;
    if (parent != NULL && decrease_node->key < parent->key) {
        cut(F_heap, decrease_node, parent);
        cascading_cut(F_heap, parent);
    }
    if (F_heap->min == NULL || decrease_node->key < F_heap->min->key) {
        F_heap->min = decrease_node;
    }
}

// extract-min（把 min node 刪掉）
void extract_min(Fheap* F_heap) {
    // 如果 F heap 是空的不能 extract-min
    if (F_heap->min == NULL) return; 
    Fheap_Node* min_node = F_heap->min;

    // 從 key_table 刪掉 *指向這個節點* 的那一格
    for (int i = 0; i <= MAX_KEY; i++) {
        if (key_table[i] == min_node) {
            key_table[i] = NULL;
            break;
        }
    }

    // 把 min_node 的 child 加回 root list
    if (min_node->child != NULL) {
        Fheap_Node* child = min_node->child;
        Fheap_Node* start = child;

        do {
            Fheap_Node* next_child = child->right;
            child->parent = NULL;
            child->childcut = 0;

            // 把 child 加回 root list
            insert_rootlist(F_heap, child);
            child = next_child;
        } while (child != start);
    }

    // 從 root list 刪掉 min_node
    if (min_node->right == min_node) {
        F_heap->min = NULL;
    } 
    else {
        min_node->left->right = min_node->right;
        min_node->right->left = min_node->left;
        F_heap->min = min_node->right;
        consolidate(F_heap);
    }
    F_heap->n--;
    free(min_node);
}

// delete（把要 delete 的 node 的 key 減到比目前 min 還小，然後 extract-min）
void delete(int key, Fheap* F_heap) {
    // 如果 F heap 是空的不能 delete
    if (F_heap->min == NULL) return;
    Fheap_Node* delete_node = key_table[key];
    // 如果要 delete 的 node 根本不存在
    if (delete_node == NULL) return; 

    // 想要把 target 的 key 降到「目前 min->key - 1」
    int current_min_key = F_heap->min->key;
    int target_key = delete_node->key;
    int gap = target_key - (current_min_key - 1);  // 要減掉多少

    // 先 decrease 一次（這版 decrease 不會動 key_table，所以安全）
    decrease(F_heap, key, gap);

    // extract-min 就可以把這個 node 刪掉
    extract_min(F_heap);
}

// 輸出
// 先把 root list 的 root 存到一個 array 裡面，然後依照 degree 排序
// 對每棵樹做 level-order traversal (BFS)

int compare_degree(const void* a, const void* b) {
    Fheap_Node* x = *(Fheap_Node**)a;
    Fheap_Node* y = *(Fheap_Node**)b;
    
    // 先比 degree
    if (x->degree != y->degree)
        return x->degree - y->degree;
    // degree 相同再比 key
    else
        return x->key - y->key;
}

void bfs(Fheap_Node* root) {

    // BFS 用到 queue
    Fheap_Node* queue[10000];
    int front = 0, rear = 0;
    queue[rear++] = root;

    int is_first = 1;

    while (front < rear) {

        // 取出一層的所有節點（要先知道目前層有多少節點）
        int level_size = rear - front;
        Fheap_Node* level_nodes[100];
        int level_cnt = 0;

        // 取出這層的所有節點，並把 child 加到下一層 queue
        for (int i = 0; i < level_size; i++) {
            Fheap_Node* cur = queue[front++];
            level_nodes[level_cnt++] = cur;

            if (cur->child != NULL) {
                Fheap_Node* c = cur->child;
                Fheap_Node* start = c;
                do {
                    queue[rear++] = c;
                    c = c->right;
                } while (c != start);
            }
        }

        // 對這層子節點（這個 tree）依 key 排序
        qsort(level_nodes, level_cnt, sizeof(Fheap_Node*), compare_key);

        // 輸出這層（這個 tree）的所有 node
        for (int i = 0; i < level_cnt; i++) {
            if (is_first) {
                printf("%d", level_nodes[i]->key);
                is_first = 0;
            } else {
                printf(" %d", level_nodes[i]->key);
            }
        }
    }
    printf("\n");
}


int main() {
    // 初始化 F heap
    Fheap* F_heap = (Fheap*)malloc(sizeof(Fheap));
    F_heap->min = NULL;
    F_heap->n = 0;
    memset(key_table, 0, sizeof(key_table));

    char operation[15];
    int key, value;

    // 持續輸入 operation
    while (1) {
        scanf("%s", operation);
        
        // insert
        if (strcmp(operation, "insert") == 0) {
            scanf("%d", &key);
            insert(key, F_heap);
        }

        // delete
        else if (strcmp(operation, "delete") == 0) {
            scanf("%d", &key);
            delete(key, F_heap);
        }

        // decrease
        else if (strcmp(operation, "decrease") == 0) {
            scanf("%d %d", &key, &value);
            decrease(F_heap, key, value);
        }

        // extract-min
        else if (strcmp(operation, "extract-min") == 0) {
            extract_min(F_heap);
        }

        // exit
        else if (strcmp(operation, "exit") == 0) {
            break;
        }
    }

    // 如果最後 F heap 是空的
    if (F_heap->min == NULL) {
        return 0;
    }
    else {
    
        // 輸出
        // 把 root list 的 root 存到一個 array 裡面
        Fheap_Node* roots[100];
        Fheap_Node* cur = F_heap->min;
        int roots_cnt = 0;
        do {
            roots[roots_cnt++] = cur;
            cur = cur->right;
        } while (cur != F_heap->min);

        // call qsort 依 degree 排序
        qsort(roots, roots_cnt, sizeof(Fheap_Node*), compare_degree);

        // 對每棵樹做 BFS 輸出
        for (int i = 0; i < roots_cnt; i++) {
            bfs(roots[i]);
        }
    }
    
    return 0;
}
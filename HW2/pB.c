#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 有一個 Binary Search Tree (BST)
// operation 有三種
// insert x：加入 x 到 BST
// delete x：從 BST 刪除 x
// exit：結束操作並按順序輸入 BST

// BST node
typedef struct Node {
    long long key;
    struct Node* left;
    struct Node* right;
} Node;
Node* root = NULL;

// insert (如果 key 已經在的話就不 insert)
Node* bst_insert(Node* cur, long long x) {
    // 遞迴一直走走到可以 insert 的位置(NULL)
    if (cur == NULL) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->key = x;
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }
    // 左邊比右邊小，所以如果 x 比較小就往左走，比較大就往右走
    if (x < cur->key) {
        cur->left = bst_insert(cur->left, x);
    } 
    else if (x > cur->key) {
        cur->right = bst_insert(cur->right, x);
    }

    // 回傳的是這個插入後的 tree 的 root (可能改變可能不變)
    // 每一層遞迴都有可能修改子數的根，所以要一直回傳，傳到遞迴第一層時代表這個 BST 已經把整棵樹都整理好
    return cur;
}

// delete
Node* bst_delete(Node* cur, long long x) {
    // 遞迴一直走找到要刪除的 node
    if (x < cur->key) {
        cur->left = bst_delete(cur->left, x);
        return cur;
    } 
    else if (x > cur->key) {
        cur->right = bst_delete(cur->right, x);
        return cur;
    }

    // No child (直接把這個 node 刪掉)
    if (cur->left == NULL && cur->right == NULL) {
        free(cur);
        return NULL;
    }

    // One child (用 child 取代這個 node)
    if (cur->left == NULL) {
        Node* temp = cur->right;
        free(cur);
        return temp;
    } 
    else if (cur->right == NULL) {
        Node* temp = cur->left;
        free(cur);
        return temp;
    }
    // Two child (用右子樹的最小節點取代這個 node)
    Node* successor_parent = cur;
    Node* successor = cur->right;

    // 一路走到最小值
    while (successor->left != NULL) {
        successor_parent = successor;
        successor = successor->left;
    }

    cur->key = successor->key;

    // 刪掉右子樹的最小節點
    // 就是右子數
    if (successor_parent == cur)
        successor_parent->right = bst_delete(successor_parent->right, successor->key);
    // 不是右子數
    else
        successor_parent->left = bst_delete(successor_parent->left, successor->key);
    return cur;
}

// exit
void bst_exit(Node* cur) { 
    if (root == NULL) {
        printf("\n");
        return;
    }

    // 要 Level-order 順序輸出
    // 用一個 queue 來輸出 BST，因為 FIFO 的特性，所以一定會先輸出完一整層後才到下一層
    Node* queue[3000];
    int front = 0, rear = 0;
    queue[rear++] = cur;
    
    while (front < rear) {
        Node* cur = queue[front++];
        printf("%lld ", cur->key); 

        // 把 left 和 right child 加到 queue 裡面
        if (cur->left != NULL)
            queue[rear++] = cur->left;
        if (cur->right != NULL)
            queue[rear++] = cur->right;
    }
    printf("\n");
}

int main() {
    // 開始構建 BST
    while(1) {
        char operation[10];
        scanf("%s", operation);

        if (strcmp(operation, "insert") == 0) {
            long long x;
            scanf("%lld", &x);
            root = bst_insert(root, x);
        } 
        else if (strcmp(operation, "delete") == 0) {
            long long x;
            scanf("%lld", &x);
            root = bst_delete(root, x);
        } 
        else if (strcmp(operation, "exit") == 0) {
            bst_exit(root);
            break;
        }
    }
}
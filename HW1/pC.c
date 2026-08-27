#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAXN (2200000 + 256)

// 輸入一串字串
// 輸入一個數字代表要進行幾次 operation
// 有一個 cursor（字元之間），初始位置在字串的最右邊
// operation 有四種 L向左 R向右 Ic插入c D刪掉cursor左邊的
// 用兩個 stack 分別代表 cursor 左邊和右邊的字元
// L：把左邊 stack pop 出來的字元 push 進右邊 stack
// R：把右邊 stack pop 出來的字元 push 進左邊 stack
// IC：把 c push 進左邊 stack
// D：把左邊 stack pop 出來

// pA 的 code
typedef struct {
    char *data;
    int top;
    int capacity;
} stack;

// create
stack* stack_create(int capacity) {
    stack *s = (stack *)malloc(sizeof(stack));
    s->data = (char *)malloc(capacity * sizeof(char));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

// pop
void stack_pop(stack *s) {
    if (s == NULL || s->top == -1) 
        return;
    s->top--;
}

// push
void stack_push(stack *s, char character) {
    if (s == NULL || s->top == s->capacity - 1) 
        return;
    s->data[++(s->top)] = character;
}


// top
int stack_top(const stack *s) {
    if (s == NULL || s->top == -1) 
        return -1;
    return s->data[s->top];
}

// is empty
bool stack_isempty(const stack *s) {
    if (s == NULL || s->top == -1) 
        return true;
    return false;
}

// free
void stack_free(stack *s) {
    if (s == NULL) 
        return;
    free(s->data);
    free(s);
}

int main() {
    char str[256];
    int M;
    char operation;

    scanf("%s", str);
    scanf("%d", &M);

    stack *left = stack_create(MAXN);
    stack *right = stack_create(MAXN);

    // 一開始 cursor 在最右邊，全部字元都在 left stack
    for (int i = 0; str[i] != '\0'; i++) {
        stack_push(left, str[i]);
    }

    // 做 M 次 operation
    for (int i = 0; i < M; i++) {
        scanf(" %c", &operation);
        if (operation == 'L') {
            if (!stack_isempty(left)) {
                char c = stack_top(left);
                stack_pop(left);
                stack_push(right, c);
            }
        } 
        else if (operation == 'R') {
            if (!stack_isempty(right)) {
                char c = stack_top(right);
                stack_pop(right);
                stack_push(left, c);
            }
        } 
        else if (operation == 'D') {
            if (!stack_isempty(left)) {
                stack_pop(left);
            }
        } 
        else if (operation == 'I') {
            char c;
            scanf(" %c", &c);
            stack_push(left, c);
        }
    }

    // 把 left right print 出來
    for (int i = 0; i <= left->top; i++)
        printf("%c", left->data[i]);

    for (int i = right->top; i >= 0; i--)
        printf("%c", right->data[i]);
   
    printf("\n");
    
    stack_free(left);
    stack_free(right);
}
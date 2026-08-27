#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAXLEN 10005
#define MAXTOK 10005

// 輸入一串運算式，包含數字、加減乘除、括號
// 輸出運算結果和運算字串的前序表示法
// 前序表示法：運算子在前面
// 用兩個 stack 分別放數字和運算子(括號)來計算結果
// 從 input[] 左到右看每一個 Token，遇到數字的話 push 到數字 stack，遇到左括號的話 push 到運算子 stack
// 接著一直運算到遇到右括號為止，然後把左括號 pop 掉
// 運算時先處理括號內的，再處理括號外的乘除，最後處理括號外的加減

// 把每個數字、運算子、括號當成一個 token，存在一個 array
typedef struct Token {
    int type;   // 1 = number, 2 = operator, 3 = parentheses
    char ch;    // operator or parentheses
    int num;    // number
} Token;
Token input[MAXTOK];
int input_len = 0;
void init() {
    for (int i = 0; i < MAXTOK; i++) {
        input[i].type = 0;
        input[i].ch = '\0';
        input[i].num = 0;
    }
}

// 把輸入的字串拆成一個一個 Token
void getInput() {
    char line[MAXLEN];
    fgets(line, sizeof(line), stdin);
    int len = strlen(line);
    for (int i = 0; i < len; i++) {
        char c = line[i];
        if (c == ' ')
            input_len += 1;
        else if ('0' <= c && c <= '9') {
            input[input_len].type = 1; // number
            input[input_len].num = input[input_len].num * 10 + (c - '0');
        } else if (c == '+' || c == '-' || c == '*' || c == '/') {
            input[input_len].type = 2; // operator
            input[input_len].ch = c;
        } else if (c == '(' || c == ')') {
            input[input_len].type = 3; // parentheses
            input[input_len].ch = c;
        } else {
            break; // end of line
        }
    }
    ++input_len;
}

// stack (用 array stack)
// 存數字
int num_stack[MAXTOK];
int num_top = 0;

// 存運算子和括號
char ch_stack[MAXTOK];
int ch_top = 0;

// 存前序表示法
char prefix_stack[MAXTOK][MAXLEN];
int prefix_top = 0;
char prefix_output[MAXLEN];

// stack function
// push
void push_num(int x) {
    num_stack[num_top++] = x;
}
void push_ch(char c) {
    ch_stack[ch_top++] = c;
}

// pop
int pop_num() {
    return num_stack[--num_top];
}
char pop_ch() {
    return ch_stack[--ch_top];
}

// 運算
// 判斷運算順序
int priority(char c) {
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
    return 0;
}

// 算
int operate(int a, int b, char operation) {
    if (operation == '+') return a + b;
    if (operation == '-') return a - b;
    if (operation == '*') return a * b;
    if (operation == '/') return a / b; 
    return 0;

}

// 計算結果 (同時存前序表示法)
int caculate() {
    // 從運算式左到右運算
    for (int i = 0; i < input_len; i++) {
        // 遇到數字放進數字 stack
        if (input[i].type == 1) {
            push_num(input[i].num);

            // 同時把數字轉成字串放進前序表示法 stack
            char buf[32];
            sprintf(buf, "%d", input[i].num);
            strcpy(prefix_stack[prefix_top++], buf);
        }
        
        // 遇到左括號就放進運算子 stack
        else if (input[i].type == 3 && input[i].ch == '(') { 
            push_ch('(');
        }
        else if (input[i].type == 3 && input[i].ch == ')') {  
            // 一直計算值到遇到右括號
            while (ch_top > 0 && ch_stack[ch_top - 1] != '(') {
                char op = pop_ch();
                int b = pop_num();   
                int a = pop_num(); 

                // 每次計算結果再放回數字 stack  
                push_num(operate(a, b, op));

                // 把括號內的運算式轉成前序表示法
                char right[MAXLEN], left[MAXLEN], prefix[MAXLEN];
                strcpy(right, prefix_stack[--prefix_top]);
                strcpy(left, prefix_stack[--prefix_top]);
                sprintf(prefix, "%c %s %s", op, left, right);
                strcpy(prefix_stack[prefix_top++], prefix);
            }

            // 把 '(' 丟掉
            if (ch_top > 0 && ch_stack[ch_top - 1] == '(') pop_ch();
        }

        // 一般運算
        else if (input[i].type == 2) {           
            char op = input[i].ch;
            // 先把優先度 >= 當前的都算掉（但不超過左括號）
            while (ch_top > 0 && ch_stack[ch_top - 1] != '(' && priority(ch_stack[ch_top - 1]) >= priority(op)) {
                char topop = pop_ch();
                int b = pop_num();
                int a = pop_num();
                push_num(operate(a, b, topop));

                // 不在括號內的前序組合
                char right[MAXLEN], left[MAXLEN], prefix[MAXLEN];
                strcpy(right, prefix_stack[--prefix_top]);
                strcpy(left, prefix_stack[--prefix_top]);
                sprintf(prefix, "%c %s %s", topop, left, right);
                strcpy(prefix_stack[prefix_top++], prefix);
            }
            push_ch(op);
        }
    }
    // 把剩下優先度比較低的運算子全部算掉
    while (ch_top > 0 && num_top > 1)  {
        char op = pop_ch();
        int b = pop_num();
        int a = pop_num();
        push_num(operate(a, b, op));

        // 不在括號內的前序組合
        char right[MAXLEN], left[MAXLEN], prefix[MAXLEN];
        strcpy(right, prefix_stack[--prefix_top]);
        strcpy(left, prefix_stack[--prefix_top]);
        sprintf(prefix, "%c %s %s", op, left, right);
        strcpy(prefix_stack[prefix_top++], prefix);
    }

    // 最後把前序表示法的結果複製到字串
    strcpy(prefix_output, prefix_stack[0]);

    // 最後 數字 stack 的 top 就是最終運算結果
    return pop_num();
}

void printResult(int res) {
    printf("%d\n", res);
    printf("%s", prefix_output);
    printf("\n");
}

int main() {
    init();
    getInput();
    int res = 0;
    res = caculate();
    printResult(res);
    return 0;
}
#include "hash.h" 
#include <stdlib.h>
// 單純選一個比 10000 大很多的質數當 table size
#define TABLE_SIZE 20007 

// 實作 hash table

// data：裡面有 key（決定要放在哪）、value

// 一個 1D array 當 hash table
// 用 linear probing 解決 collision
// 遇到 collision 就往下一個位置找

int keys[TABLE_SIZE];
int values[TABLE_SIZE];

// 紀錄這個位置現在有沒有放 0:沒有 1:有 2:原本有但是被刪掉（2 是因為 linear probing 可能會讓某些 key 被擠到後面去）
int occupied[TABLE_SIZE] = {0};

void insert(Data *data) {
    int key = data->key;
    int value = data->value;

    // hash function
    int index = key % TABLE_SIZE;

    while (1) {
        // 如果是空的或是被刪掉的，就放進去
        if (occupied[index] == 0 || occupied[index] == 2) {
            keys[index] = key;
            values[index] = value;
            occupied[index] = 1;
            return;
        }
        // 如果這格有放了就往下一格找
        else if (occupied[index] == 1) {
            index = (index + 1) % TABLE_SIZE;
        }
    }
}

void remove(int key) {
    int index = key % TABLE_SIZE;
    
    // 從這個 key 應該要放的位置起始點開始往後找
    while (1) {
        // 如果是空的，代表這個 key 不存在，不是 delete 代表也不是被擠到後面了
        if (occupied[index] == 0) {
            return;
        }
        // 如果找到這個 key，就標記為被刪掉
        else if (occupied[index] == 1 && keys[index] == key) {
            occupied[index] = 2;
            return;
        }
        index = (index + 1) % TABLE_SIZE;
    }
}

int search(int key) {
    int index = key % TABLE_SIZE;

    // 從這個 key 應該要放的位置起始點開始往後找
    while (1) {
        // 如果是空的，代表這個 key 不存在，不是 delete 代表也不是被擠到後面了
        if (occupied[index] == 0) {
            return -1;
        }
        // 如果找到這個 key，就回傳 value
        else if (occupied[index] == 1 && keys[index] == key) {
            return values[index];
        }
        index = (index + 1) % TABLE_SIZE;
    }    
}
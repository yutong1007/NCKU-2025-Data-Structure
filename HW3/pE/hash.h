// hash.h
typedef struct Data {
    int key;
    int value;
} Data;

void insert(Data *data);
void remove(int key);
int search(int key);
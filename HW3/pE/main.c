#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int command_count;
    scanf("%d", &command_count);

    for (int i = 0; i < command_count; i++) {
        char command[8];
        scanf("%s", command);
        if (command[0] == 'i') {
            Data *data = malloc(sizeof(Data));
            scanf("%d %d", &data->key, &data->value);
            insert(data);
        } else if (command[0] == 'd') {
            int key;
            scanf("%d", &key);
            remove(key);
        } else if (command[0] == 's') {
            int key;
            scanf("%d", &key);
            printf("%d\n", search(key));
        }
    }

    return 0;
}
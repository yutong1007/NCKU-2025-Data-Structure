#define MAX_NODES 100000
#define MAX_EDGES 1200000

typedef struct Edge {
    struct Node *u;
    struct Node *v;
    int w;
    char keep;
    void *var;
} Edge;

typedef struct Node {
    int id;
    int edge_count;
    struct Edge **edges;
    void *var;
} Node;

void generate_mst(Node *node);
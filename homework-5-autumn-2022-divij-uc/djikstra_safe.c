#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct node {
    int node_id;
    int dist;
    struct node *next_edge;
};

typedef struct node node;

struct ans_row {
    int k;
    int p;
    int d;
};

int NUM_NODES = 0;

node** read_graph(FILE*graph_file){
    char read_line[200];
    int num_edges;
    node **graph = NULL;
    while(EOF != fscanf(graph_file, "%[^\n]\n", read_line)){
        if(read_line[0] == '#'){
        }
        else{
            sscanf(read_line,"%d %d",&NUM_NODES, &num_edges);
            break;
        }
    }
    printf("NUMNODES %d\n", NUM_NODES);
    graph = (node**)malloc(sizeof(node*)*NUM_NODES);
    int edge1, edge2, dist;
    for(int i=0; i<num_edges;i++){
        fscanf(graph_file, "%d %d %d\n", &edge1, &edge2, &dist);
        for(int j = 0; j < 2; j++){
            int temp = edge1;
            edge1 = edge2;
            edge2 = temp;
            if(graph[edge1] == NULL){
                graph[edge1] = (node*)malloc(sizeof(node));
                graph[edge1]->node_id = edge1;
                graph[edge1]->dist = 0;
                graph[edge1]->next_edge = (node*)malloc(sizeof(node));
            }
            node* cur_edge = graph[edge1]->next_edge;
            while(cur_edge->next_edge != NULL){
                cur_edge = cur_edge->next_edge;
            }
            cur_edge->next_edge = (node*)malloc(sizeof(node));
            cur_edge->dist = dist;
            cur_edge->node_id = edge2;
        }
    }
    fclose(graph_file);
    return graph;
}

int main(int argc, char **argv) {
    node **graph = NULL;
    FILE* graph_file;
    char* file_name = NULL;
    file_name = argv[1];
    graph_file = fopen(file_name, "r");
    int f = 0, num_edges;
    /* https://stackoverflow.com/questions/20108334/traverse-file-line-by-line-using-fscanf */

    graph = read_graph(graph_file);
    node** ans_graph = NULL;
    ans_graph = (node**)malloc(sizeof(node*)*NUM_NODES);
    for(int i=0; i<NUM_NODES;i++){
        ans_graph[i] = (node*)malloc(sizeof(node)*NUM_NODES);
    }

    /* for(int i = 0; i < NUM_NODES; i++){
        node* cur_edge = graph[i]->next_edge;
        while(cur_edge->next_edge != NULL){
            printf("%d: %d - %d\n", i, cur_edge->node_id, cur_edge->dist);
            cur_edge = cur_edge->next_edge;
        }
    } */

    for(int i = 0; i < 1; i++){
        struct ans_row ans[NUM_NODES];
        for(int j = 0; j < NUM_NODES; j++){
            ans[j].d = __INT_MAX__;
            ans[j].k = 0;
            ans[j].p = -1;
        }
        ans[i].d = 0;
        int cur_low = i, cur_dist = 0;
        while(ans[cur_low].k == 0){
            printf("cur low: %d\n", cur_low);
            ans[cur_low].k = 1;
            node* cur_edge = graph[cur_low]->next_edge;
            while(cur_edge != NULL){
                if (ans[cur_edge->node_id].d > cur_dist + cur_edge->dist)
                {
                    ans[cur_edge->node_id].p = cur_low;
                    ans[cur_edge->node_id].d = cur_edge->dist + cur_dist;
                }
                cur_edge = cur_edge->next_edge;
            }
            int min_dist = __INT_MAX__;
            for(int j = 0; j < NUM_NODES; j++){
                if(min_dist > ans[j].d & ans[j].k == 0){
                    cur_low = j;
                    min_dist = ans[j].d;
                }
            }
            cur_dist = min_dist;
        }

        for(int j = 0; j < 100; j++){
            printf("%d: %d - %d\n", i, j, ans[j].d);
        }
        printf("\n");
    }

    return 0;
}
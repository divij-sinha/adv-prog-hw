#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include <omp.h>
#include "timer.h"

struct node {
    int node_id;
    int dist;
    struct node *next_edge;
};

struct ans_struct {
    int arr[10][3];
};

typedef struct node node;

int NUM_NODES = 0;
int NUM_THREADS = 16;

void insert_edge(int edge1, int edge2, int dist, node**graph){
    if(graph[edge1] == NULL){
        graph[edge1] = (node*)malloc(sizeof(node));
        graph[edge1]->node_id = edge1;
        graph[edge1]->dist = 0;
        graph[edge1]->next_edge = (node*)malloc(sizeof(node));
        graph[edge1]->next_edge->dist = INT_MAX;
        graph[edge1]->next_edge->next_edge = NULL;
    }
    node* cur_edge = graph[edge1]->next_edge;
    while(cur_edge->next_edge != NULL){
        cur_edge = cur_edge->next_edge;
    }
    cur_edge->next_edge = (node*)malloc(sizeof(node));
    cur_edge->dist = dist;
    cur_edge->node_id = edge2;
    cur_edge->next_edge->next_edge = NULL;
}

node** read_graph(FILE*graph_file){
    char read_line[200];
    int num_edges;
    node **graph = NULL;
    while(EOF != fscanf(graph_file, "%[^\n]\n", read_line)){
        if(read_line[0] != '#'){
            sscanf(read_line,"%d %d",&NUM_NODES, &num_edges);
            break;
        }
    }
    graph = (node**)malloc(sizeof(node*)*NUM_NODES);
    for(int i=0; i<NUM_NODES; i++){
        graph[i] = NULL;
    }
    int edge1, edge2, dist;
    for(int i=0; i<num_edges;i++){
        fscanf(graph_file, "%d %d %d\n", &edge1, &edge2, &dist);
        insert_edge(edge1, edge2, dist, graph);
    }
    fclose(graph_file);
    return graph;
}

int insert_heap(int heap[][2], int data, int pos, int len){
    if(!((data == 0) & (pos == 0))){
        heap[len][0] = data;
        heap[len][1] = pos;
        int i = len;
        while((int)(i / 2) > 0){
            if(heap[i][0] < heap[(int)(i/2)][0]){
                int temp_data = heap[(int)(i/2)][0];
                heap[(int)(i/2)][0] = heap[i][0];
                heap[i][0] = temp_data;

                int temp_pos = heap[(int)(i/2)][1];
                heap[(int)(i/2)][1] = heap[i][1];
                heap[i][1] = temp_pos;
            }
            i = (int)(i/2);
        }
    }
    else{
        len--;
    }
    return len;
}

int* pop_heap(int heap[][2], int len){
    int* low_node = malloc(sizeof(int)*2);
    low_node[0] = heap[1][0];
    low_node[1] = heap[1][1];
    heap[1][0] = heap[len+1][0];
    heap[1][1] = heap[len+1][1];
    int i = 1;
    while((i * 2) <= len){
        int min_child;
        if((i * 2) + 1 > len) min_child = (i * 2);
        else if (heap[(i * 2)][0] < heap[(i * 2) + 1][0]) min_child = (i * 2);
        else min_child = (i * 2) + 1;
        if(heap[i][0] > heap[min_child][0]){
            int temp_data = heap[min_child][0];
            heap[min_child][0] = heap[i][0];
            heap[i][0] = temp_data;

            int temp_pos = heap[min_child][1];
            heap[min_child][1] = heap[i][1];
            heap[i][1] = temp_pos;
        } 
        i = min_child;
    }
        
    return low_node;
}

struct ans_struct omp_reduction(struct ans_struct omp_in, struct ans_struct omp_out){
    struct ans_struct merged;
    int max_d = INT_MIN, idx = 0, merged_idx = 0;
    while(merged_idx < 10){
        for(int i = 0; i < 20; i++){
            if(i < 10){
                if(omp_in.arr[i][0] > max_d){
                    max_d = omp_in.arr[i][0];
                    idx = i;
                }
            }
            else{
                if(omp_out.arr[i-10][0] > max_d){
                    max_d = omp_out.arr[i-10][0];
                    idx = i;
                }
            }
        }
        if(idx < 10){
            merged.arr[merged_idx][0] = omp_in.arr[idx][0];
            merged.arr[merged_idx][1] = omp_in.arr[idx][1];
            merged.arr[merged_idx][2] = omp_in.arr[idx][2];
            omp_in.arr[idx][0] = INT_MIN;
        }
        else{
            merged.arr[merged_idx][0] = omp_out.arr[idx-10][0];
            merged.arr[merged_idx][1] = omp_out.arr[idx-10][1];
            merged.arr[merged_idx][2] = omp_out.arr[idx-10][2];
            omp_out.arr[idx-10][0] = INT_MIN;
        }
        max_d = INT_MIN;
        merged_idx++;
    }
    return merged;
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
    struct ans_struct ans;
    for(int i = 0; i < 10; i++){
        ans.arr[i][0] = INT_MIN;
    }

    StartTimer();
    #pragma omp declare reduction(max_ans: struct ans_struct: omp_out = omp_reduction(omp_in, omp_out))
    #pragma omp parallel for default(none) firstprivate(NUM_NODES) shared(graph) schedule(dynamic, 1000) num_threads(32) reduction(max_ans: ans)
    for(int i = 0; i < NUM_NODES; i++){
        /* printf("%d\n",i); */
        if(graph[i] == NULL) continue;
        else {
            int thread_num = omp_get_thread_num();
            if((i % 1000) /10 == 0) printf("node %d\n", i);
            /* initialise answers */
            int dist[NUM_NODES];
            int proc[NUM_NODES];
            int par[NUM_NODES];
            int heap[NUM_NODES+1][2];
            heap[0][0] = 0;
            heap[0][1] = 0;
            int len = 0;
            for(int j = 0; j < NUM_NODES; j++){
                dist[j] = INT_MAX;
                proc[j] = 0;
                par[j] = -1;
            }

            dist[i] = 0;
            int cur_low = i, cur_dist = 0;
            if (i!=0) proc[0] = 1;
            /* printf("CURLOWWWW %d\n", cur_low); */
            /* update graph */
            while(proc[cur_low] == 0){
                
                proc[cur_low] = 1;
                node* cur_edge = graph[cur_low];
                /* printf("here!\n"); */
                while(cur_edge != NULL){
                    int cur_id = cur_edge->node_id;
                    if (dist[cur_id] > cur_dist + cur_edge->dist)
                    {
                        par[cur_id] = cur_low;
                        dist[cur_id] = cur_edge->dist + cur_dist;
                        if(proc[cur_id] == 0){
                            len++;
                            len = insert_heap(heap, dist[cur_id], cur_id, len);
                        }
                    }
                    /* printf("curid %d %d \n", cur_id, dist[cur_id]); */
                    cur_edge = cur_edge->next_edge;
                }
                /* printf("here! 3\n"); */
                int* low_node;
                len--;
                low_node = pop_heap(heap, len);
                while((proc[low_node[1]] == 1) & (low_node[1] != 0)){
                    free(low_node);
                    len--;
                    low_node = pop_heap(heap, len);
                }
                /* printf("lownode: %d %d\n", low_node[1], low_node[0]); */
                cur_dist = low_node[0];
                cur_low = low_node[1];
                int min = INT_MAX;
                int min_idx = -1;
                /* #pragma omp critical */                
                {
                    for(int k = 0; k < 10; k++){
                        if(ans.arr[k][0] < min ){
                            min_idx = k;
                            min = ans.arr[k][0];
                        }
                    }
                    if(ans.arr[min_idx][0] < low_node[0] & low_node[0] != INT_MAX){
                        ans.arr[min_idx][0] = low_node[0];
                        ans.arr[min_idx][1] = i;
                        ans.arr[min_idx][2] = low_node[1];
                    }
                }
                free(low_node);
            }
            /* for(int j = 0; j < 100; j++){
                printf("%d: %d - %d\n", i, j, dist[j]);
            }
            printf("\n"); */
        }
    }
    double totalTime = GetTimer()/1000;
    printf("\nTOTAL TIME: %.4f", totalTime);
    printf("\nTIME PER NODE: %.4f\n", totalTime/NUM_NODES);
    for(int i = 0; i < 10; i++){
        printf("%d %d %d\n", ans.arr[i][1], ans.arr[i][2], ans.arr[i][0]);
    }
    for(int i = 0; i < NUM_NODES; i++){
        node* cur_edge = graph[i];
        while(cur_edge!=NULL){
            node* last_edge = cur_edge;
            cur_edge = cur_edge->next_edge;
            free(last_edge);
        }
    }
    free(graph);

    return 0;
}

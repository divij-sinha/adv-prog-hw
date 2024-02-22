#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <string.h>


int insert_heap(int heap[][2], int data, int pos, int len){
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


int main(int argc, char **argv) {
    srand(1212);
    int heap[(100)+1][2];
    heap[0][0] = 0;
    heap[0][1] = 0;
    int len = 0;
    for(int i = 0; i < 10; i++){
        int k = (100*random())/RAND_MAX;
        int l = random();
        len++;
        insert_heap(heap, k, l, len);
        printf("pushed: %d\n", k);
        for(int j = 0; j < len+1; j++){
            printf("%d ", heap[j][0]);
        }
        printf("\n");
    }
    for(int i = 0; i < 10; i++){

        int* low_node;
        
        len--;
        low_node = pop_heap(heap, len);
        printf("popped: %d\n", low_node[0]);
        for(int j = 0; j < len+1; j++){
            printf("%d ", heap[j][0]);
        }
        printf("\n");

        int k = (100*random())/RAND_MAX;
        int l = random();
        len++;
        insert_heap(heap, k, l, len);
        printf("pushed: %d\n", k);
        for(int j = 0; j < len+1; j++){
            printf("%d ", heap[j][0]);
        }
        printf("\n");
    }

}
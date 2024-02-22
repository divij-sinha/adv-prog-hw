#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"hw1.h"

/*CLIENT FUNCTIONS*/

void element_print(void* element){
    printf("%d",*(int*)element);
}
int element_compar(const void * p1, const void* p2){
    int c;
    c = *(int*)p1 - *(int*)p2;
    return c;
}

void element_destroy(void* element){
    int* casted_element;
    casted_element = (int*)element;
    free(casted_element);
}

int main(int argc, char **argv) {

    List *list;
    int **arr, i, n = 20 ;
    int *temp;

    srand(time(NULL));

    arr = (int**)malloc(n*sizeof(int*));
    for(i = 0; i < n; i++){
        *(arr+i) = malloc(sizeof(int));
        **(arr+i) = ((float)rand()/(float)(RAND_MAX)) * 100;
        printf("%d ",**(arr+i));
    }
    list = list_create();
    list_add_element_print_func(list, element_print);
    list_add_element_compar_func(list, element_compar);
    list_add_element_destroy_func(list, element_destroy);
    printf("\nAppending value to list");
    list_append(list,*(arr+0));
    list_print(list);
    
    printf("Appending value to list");
    list_append(list,*(arr+1));
    list_print(list);

    printf("Prepending value to list");
    list_prepend(list,*(arr+2));
    list_print(list);

    printf("Appending value to list");
    list_append(list,*(arr+3));
    list_print(list);

    printf("Prepending value to list");
    list_prepend(list,*(arr+4));
    list_print(list);

    printf("Inserting value to list at pos 5");
    list_insert(list,*(arr+5), 5);
    list_print(list);

    printf("Inserting value to list at pos -3");
    list_insert(list,*(arr+6), -3);
    list_print(list);

    printf("Inserting value to list at pos 0");
    list_insert(list,*(arr+7), 0);
    list_print(list);

    printf("Inserting value to list at pos -1");
    list_insert(list,*(arr+8), -1);
    list_print(list);

    printf("Inserting value to list at pos 20 (fail)");
    list_insert(list,*(arr+9), 20);
    list_print(list);

    printf("Inserting value to list at pos -20 (fail)");
    list_insert(list,*(arr+10), -20);
    list_print(list);

    printf("Searching for pos of first value appended to list");
    printf("\n%d ", list_first_index(list,*(arr+0)));
    list_print(list);

    printf("Searching for pos of random value not explicitly appended to list");
    printf("\n%d ", list_first_index(list,*(arr+11)));
    list_print(list);

    printf("Getting element at pos 3");
    temp = list_element_at(list,3);
    printf("\n%p: %d", (void*)temp, *temp);
    list_print(list);

    printf("Getting element at pos 30 (fail)");
    temp = list_element_at(list,30);
    printf("\n%p", (void*)temp);
    list_print(list);

    printf("Removing element at pos 4");
    list_remove(list, 4);
    list_print(list);

    printf("Removing element at pos -2");
    list_remove(list, -2);
    list_print(list);

    printf("Removing element at pos 0");
    list_remove(list, 0);
    list_print(list);

    printf("Removing element at pos 23 (fail)");
    list_remove(list, 23);
    list_print(list);

    printf("Reversing list");
    list_reverse(list);
    list_print(list);

    list_clear_all(list);

    return 0;
}
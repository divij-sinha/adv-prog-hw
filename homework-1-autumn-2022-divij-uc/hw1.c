#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"hw1.h"

ListElement* create_element(void* new_data){
    ListElement *new_element;
    new_element = malloc(sizeof(ListElement));
    new_element->next = NULL;
    new_element->prev = NULL;
    new_element->data = new_data;
    return new_element;
}

/*function to create empty list*/
/*returns a pointer to a list*/
List * list_create(){
    List *list;
    list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_add_element_print_func(List *list, void (*element_print)(void* element)){
    list->element_print = element_print;
}

void list_add_element_compar_func(List *list, int (*element_compar)(const void * p1, const void* p2)){
    list->element_compar = element_compar;
}
void list_add_element_destroy_func(List *list, void (*element_destroy)(void* element)){
    list->element_destroy = element_destroy;
}

void list_print(List* list){
    int i = 0;
    ListElement * temp;
    temp = list->head;
    printf("%s","\n[ ");
    for(i = 0; i < list->size; i++){
        (*(*list).element_print)(temp->data);
        printf("%s"," ");
        temp = temp->next;
    }
    printf("%s","]\n");
}

void list_append(List* list, void* new_data){
    ListElement *new_element;
    new_element = create_element(new_data);
    if(list->size == 0){
        list->head = new_element;
        list->tail = new_element;
    }
    else{
        new_element->prev = list->tail;
        list->tail->next = new_element;
        list->tail = new_element;
    }
    ++list->size;
}

void list_prepend(List* list, void* new_data){
    ListElement *new_element;
    new_element = create_element(new_data);
    if(list->size == 0){
        list->head = new_element;
        list->tail = new_element;
    }
    else{
        new_element->next = list->head;
        list->head->prev = new_element;
        list->head = new_element;
    }
    list->size++;
}

int list_insert(List* list, void* new_data, int pos){
    if((pos == -1) || (pos == list->size)){
        list_append(list,new_data);
        return 0;
    }
    else if(pos == 0){
        list_prepend(list, new_data);
        return 0;
    }
    else if((pos > (-1 * list->size)) && (pos < list->size)){
        ListElement *temp, *new_element, *prev;
        int i;
        if(pos < 0){
            pos = list->size + pos + 1;
        }
        temp = list->head;
        for(i = 0; i < pos; i++){
            temp = temp->next;
        }
        prev = temp->prev;
        new_element = create_element(new_data);
        new_element->next = temp;
        temp->prev = new_element;
        prev->next = new_element;
        new_element->prev = prev;
        list->size++;
        return 0;
    }
    else{
        return -1;
    }
}

void* list_element_at(List* list, int pos){
    int i;
    ListElement *temp;
    if((pos > (-1 * list->size)) && (pos < list->size)){
        if(pos < 0){
            pos = list->size + pos;
        }
        temp = list->head;
        for(i = 0; i < pos; i++){
            temp = temp->next;
        }
        return temp->data;
    }
    else{
        return NULL;
    } 
}

int list_first_index(List* list, void* new_data){
    int i = 0;
    int flag = 0;
    ListElement *temp;
    temp = list->head;
    for(i=0; i < list->size; i++){
        if((*(*list).element_compar)(new_data,temp->data) == 0){
            flag = 1;
            break;
        }
        temp = temp->next;
    }
    if(flag == 1){
        return i;
    }
    else{
        return -1;
    }
}

void element_free(List* list, ListElement* element){
    (*(*list).element_destroy)(element->data);
    free(element);
}

int list_remove(List* list, int pos){
    if(pos == 0){
        ListElement *temp;
        temp = list->head;
        list->head->next->prev = NULL;
        list->head = list->head->next;
        element_free(list, temp);
        list->size--;
        return 0;
    }
    else if(pos == -1){
        ListElement *temp;
        temp = list->tail;
        list->tail->prev->next = NULL;
        list->tail = list->tail->prev;
        element_free(list, temp);
        list->size--;
        return 0;
    }
    else if((pos > (-1 * list->size)) && (pos < list->size)){
        ListElement *temp, *prev;
        int i;
        if(pos < 0){
            pos = list->size + pos;
        }
        temp = list->head;
        for(i = 0; i < pos; i++){
            temp = temp->next;
        }
        prev = temp->prev;
        prev->next = temp->next;
        temp->next->prev = prev;
        element_free(list, temp);
        list->size--;
        return 0;
    }
    else{
        return -1;
    }
}

void list_reverse(List * list){
    int i;
    ListElement * temp, *curr;
    curr = list->head;
    for(i = 0; i< list->size; i++){
        temp = curr->next;
        curr->next = curr->prev;
        curr->prev = temp;
        curr = curr->prev;
    }
    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

void list_clear_all(List* list){
    int i;
    ListElement *temp;
    temp = list->head;
    for(i=1;i<list->size;i++){
        temp = temp->next;
        element_free(list, temp->prev);
    }
    element_free(list, temp);
    free(list);
}
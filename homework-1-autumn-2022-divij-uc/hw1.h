/*struct of list element*/
typedef struct ListElement_ {
    void *data;
    struct ListElement_ *next;
    struct ListElement_ *prev;
} ListElement;

/*struct of list*/
typedef struct List_{
    ListElement *head;
    ListElement *tail;
    int size;

    /* Function pointers to client functions */
    void (*element_print)(void* element);
    int (*element_compar)(const void * p1, const void* p2);
    void (*element_destroy)(void* element);

} List;
ListElement* create_element(void* new_data);
List * list_create();
void list_add_element_print_func(List *list, void (*element_print)(void* element));
void list_add_element_compar_func(List *list, int (*element_compar)(const void * p1, const void* p2));
void list_add_element_destroy_func(List *list, void (*element_destroy)(void* element));
void list_print(List* list);
void list_append(List* list, void* new_data);
void list_prepend(List* list, void* new_data);
int list_insert(List* list, void* new_data, int pos);
void* list_element_at(List* list, int pos);
int list_first_index(List* list, void* new_data);
void element_free(List* list, ListElement* element);
int list_remove(List* list, int pos);
void list_reverse(List * list);
void list_clear_all(List* list);
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#include"hw1.h"

/*CLIENT FUNCTIONS*/
struct Point_{
  char *color;
  float x;
  float y;
  float z;
};

typedef struct Point_ Point;

float dist (Point *a){
  return sqrt((a->x)*(a->x)+(a->y)*(a->y)+(a->z)*(a->z));
}

Point* create_ran_point(){
    float x,y,z,col_r;
    float a = 10.0;
    char* col;
    Point *p;

    p = (Point *) malloc(sizeof(Point));
    x = ((float)rand()/(float)(RAND_MAX)) * a - a/2.0;
    y = ((float)rand()/(float)(RAND_MAX)) * a - a/2.0;
    z = ((float)rand()/(float)(RAND_MAX)) * a - a/2.0;
    col_r = ((float)rand()/(float)(RAND_MAX));

    if(col_r < 0.33){
        col = "red";
    }
    else if(col_r < 0.67){
        col = "blue";
    }
    else{
        col = "yellow";
    }

    p->x = x; p->y = y; p->z = z;
    p->color = col;
    return p;
}

void element_print(void* element){
    Point *point;
    point = (Point*)element;
    printf("\n%s: %f, %f, %f",point->color, point->x, point->y, point->z);
}

void element_destroy(void* element){
    Point *point;
    point = (Point*)element;
    free(point);
}

int element_compar(const void *a, const void *b){
  Point **pa = (Point **) a;
  Point **pb = (Point **) b;

  float dista = dist(*pa);
  float distb = dist(*pb);

  if (dista < distb)
    return 1;
  else if (dista == distb) 
    return 0;
  else 
    return -1;
}

void test_list(List *list){
    ListElement* temp;
    int cnt = 0;
    for(temp = list->head; temp->next->next != NULL; temp = temp->next){
        cnt = cnt + element_compar(temp->data, temp->next->data);
    }
    printf("\n%d", cnt);
}

void test_array(Point **p, int n){
    int i = 0;
    int cnt = 0;
    for(i = 0; i < n-1; i++){
        cnt = cnt + element_compar(*(p+i),*(p+i+1));
    }
    printf("\n%d", cnt);
}

int main(int argc, char **argv) {

    Point **p;
    Point *new_p;
    List *list;
    int i = 0;
    int n = 500;
    clock_t start, end;
    double list_time, array_time; 
    
    if(argc > 1){
        n = atoi(argv[1]);
    }

    p = (Point **) malloc(n*sizeof(Point*));
    list = list_create();
    list_add_element_print_func(list, element_print);
    list_add_element_compar_func(list, element_compar);
    list_add_element_destroy_func(list, element_destroy);
    srand(time(NULL));

    start = clock();
    for (i=0;i<n;++i){
        new_p = create_ran_point();
        p[i] = new_p;
    }
    end = clock();
    list_time = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    for (i=0;i<n;++i){
        new_p = create_ran_point();
        list_append(list,new_p);
    }
    end = clock();
    array_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nList init time: %f",list_time);
    printf("\nArray init time: %f\n\n",array_time);

    start = clock();
    test_list(list);
    end = clock();
    list_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    start = clock();
    test_array(p,n);
    end = clock();
    array_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("\nList time: %f",list_time);
    printf("\nArray time: %f\n\n",array_time);

    for (i=0;i<n;++i){
        free(*(p+i));
    }
    free(p);
    list_clear_all(list);
    return 0;
}
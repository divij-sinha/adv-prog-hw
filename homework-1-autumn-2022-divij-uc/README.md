# Submission Requirements

Name - Divij Sinha  
cnet - divijs  
UchicagoID - 12364171

# Files

 list with point example in `hw1_point.c`  
 to complile use `gcc -Wall -ansi -pedantic hw1_point.c -o hw1_point -lm` (-lm to add math modules)  
 for running the tests (mentioned below), run `./hw1_point n` where n is an integer for the number of point elements to be created

 list with int example in `hw1_int.c`  
 to complile use `gcc -Wall -ansi -pedantic hw1_int.c -o hw1_int -lm` (-lm to add math modules)
 for running, run `.hw1_int` it will run and demonstrate all the functions.
 

# Features of list

The struct `List` is a doubly linked list with the following members  
 - `int size`  
 - `ListElement * head`  
 - `ListElement * tail`  

`head` and `tail` are pointers to the first and last element of the list respectively.

The underlying `ListElement` struct has the following members
 - `void * data`
 - `ListElement *next`
 - `ListElement *prev`

 `data` is a pointer which stores the pointer to the actual data passed by the CLIENT.  
 `next` and `prev` are respectively pointers to the next and previous ListElements

To create the list, the client must initialise the list pointer using the following syntax and call the following function to initialise the list.  
`List *list;`  
`list = list_create()`


The actual manipulation of the list can be done in the following functions - 
 - `list_print`  
    arguments - `list`  
    returns - (void)  
    behaviour - prints the whole list starting from head  
    _Note: The `void *data` pointer is passed to the client function `element_print` to print a single element!!!!_

- `list_append`  
    arguments - `list, new_data`  
    returns - (void)  
    behaviour - adds an element to the end of the list (after tail)  
    _Note: The new_data is to be passed as a pointer to the data!!!!_

- `list_prepend`  
    arguments - `list, new_data`  
    returns - (void)  
    behaviour - adds an element to the beginning of the last (before head)  
    _Note: The new_data is to be passed as a pointer to the data!!!!_

- `list_insert`  
    arguments - `list, new_data, pos`  
    returns - 0 on success, -1 on failure  
    behaviour - adds an element to the index specified by the int `pos`. `pos` can take values between `-size` and `size` of the list. 

    0 would prepend to the first postion.  
    A positive value would insert at the index specified from the beginning of the list.  
    A negative value would insert relatively from the end of the list. (Eg. -2 would insert the element to the second last position in the list)  
    _Note: The new_data is to be passed as a pointer to the data!!!!_

- `list_element_at`  
    arguments - `list, pos`  
    returns - void pointer to the data on success, NULL on failure  
    behaviour - returns an element at the index specified by the int `pos`. `pos` can take values between `-pos` and `size` of the list. 

    0 would return the element at the first postion.  
    A positive value would return the element at the index specified from the beginning of the list.  
    A negative value would return the element relatively from the end of the list. (Eg. -2 would return the element at the second last position in the list)  
    _Note: The void pointer must be cast to the reqd data type on return!!!_

- `list_first_index`  
    arguments - `list, new_data`  
    returns - index of the position where the data is stored if found, -1 otherwise  
    behaviour - index of the position where the data is stored  
    _Note: The new_data is to be passed as a pointer to the data!!!!_
    _Note: The `void *data` pointers are compared by passing to the client function `element_compar` !!!!_

- `list_remove`  
    arguments - `list, pos`  
    returns - 0 on success, -1 on failure
    behaviour - removes an element from the index specified by the int `pos`. `pos` can take values between `-size` and `size` of the list. 

    0 would remove the element to the first postion.  
    A positive value would remove from the index specified from the beginning of the list.  
    A negative value would remove relatively from the end of the list. (Eg. -2 would remove the element at the second last position in the list)  
    _Note: The `void *data` pointer is passed to the client function e;`element_destroy` for final handling!!!!_

- `list_reverse`  
    arguments - `list`  
    returns - (void)  
    behaviour - reverses the list, swaps `head` and `tail`, and each elements' `next` and `prev`

- `list_clear_all`  
    arguments - `list`  
    returns - (void)  
    behaviour - removes all elements from the list and the list itself.
    _Note: The `void *data` pointer for each variable passed to the client function `element_destroy` for final handling!!!!_


The program needs the following 3 client functions - 

    - void element_print(void* element)
        /* prints the element */

    - int element_compar(const void * p1, const void* p2)
        /* compares two elements and returns >0 if p1 > p2, 0 if p1 == p2 and < 0 if p1 < p2 */

    - void element_destroy(void* element)
        /* handles the destruction of the element if necessary */

These must be passed to the list through the following functions respectively

- `list_add_element_print_func`  
    arguments - `list, void (*element_print)(void* element))` 
    returns - (void)  
    behaviour - adds the function to the list. can be updated at any time  

- `list_add_element_compar_func`  
    arguments - `list, int (*element_compar)(const void * p1, const void* p2))` 
    returns - (void)  
    behaviour - adds the function to the list. can be updated at any time  

- `list_add_element_destroy_func`  
    arguments - `list, void void (*element_destroy)(void* element))` 
    returns - (void)  
    behaviour - adds the function to the list. can be updated at any time  


# Test for point vs array

We use two tests -

 - appending to list
 - comparing consecutive elements of list

For the first one, we randomly generate points and add the same points to an array and to the list structure, and calculate the time taken for the task. This task is faster through the list structure than the array.

For the second one, we access two consecutive elements, and using the user defined functions, compare the values and print the sum of the comparisons. This task takes almost the same time through both (primarily because we are accessing them in order though). 

## Thoughts
I feel like both of these tests play to the strengths of the list structure somewhat. The structure does allow quick sequential access, while random access to an index would be a lot slower. In cases of unexpected appends, the list would be a good alternative to the array. It also helps that by not having to preallocate a lot of memory. (Although in the case of the Point example, it does not allocate memory for a Point struct in the beginning, only the pointers to the Point struct, it still needs some certainty about how many Points can be expected to be used.)

We could have also had other tests like something to insert and remove random values from random indices. This kind of test would probably perform better for an array than the list structure.

This list structure is a great way to look at sequential data structures, especially stacks, queues, graphs, trees etc. while maybe being suboptimal for something like a dataframe which involves a lot of looking at a particular row etc.

# Shortcomings

## Design
Currently it is a very high level implementation. The client, while they can specify a few things about what to do with the data itself, cannot really change anything about the way the list is managed. The underlying structures are opaque in this case and the client is expected to use only the functionality we provide. Except the 3 client functions exposed, we rely on our structure to provide all the support. While this increases security somewhat (assuming we are doing the right things), it also massively reduces the client's ability to customise the structure/add functions. 

We could open things up to the client further. We could allow them to manipulate the underlying elements, add more functions that make use of the underlying structure, without locking them into our implementation. However, we must consider that this would increase the risk of memory being badly managed (assuming we manage to not do that ourselves!), or changing things in a way that is not predictable to the current structure and functions.

On the other hand, we could go further down the route of locking things up for the client. We could make the API look cleaner, not expect the client to manage pointers etc. In this case we could add custom functions as the client wants them, but not let them change anything directly.

Both these approaches have their pros and cons, and my approach here seems to me to be a more conservative, lets have the client do very little with the list structure approach.

## Code 
Currently the list uses pointers to data to add, compare etc. this could be replaced to be just the data being passed instead.    
The code itself is probably not as efficient as it could be, and can be further optimised to increase performance.  
While it is a doubly linked list, we almost don't take advantage of it anywhere. 


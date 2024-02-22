# Homework 4

[PDF with question of HW4](MPCS_51100_2022_Fractional_Cascade.pdf)

# Submission Requirements

Name - Divij Sinha  
cnet - divijs  
UchicagoID - 12364171

# Files

The 4 implementations are in the `hw4.c` file.


## Function declarations


 - `void parse_file(double **p, int *psize, FILE*datafile);`  
    Reads file pointed by `datafile` and returns `double**p` which is a 2-D array of all elements with size of each row stored in `psize`

 - `int binary_search(int l, int h, double*array, double q);`  
    For array of doubles `array` and query `q` given lower and higher bounds `l` and `h`, returns the index of the element equal to or first element greater than `q`. If `array[h] > q`, returns `h+1`

    METHOD 1  
    The first method is raw searching. The following method(s) are involved in its implementation.
 - `int* raw_search(double **p, int *psize, double q, int* results);`  
    For 2D array of doubles `p`, with each row `i` of size `psize[i]` and query `q`, stores the result of the binary search on `q` in `results[i]` for each row `i`

    METHOD 2  
    The second method creates a union of all rows of the 2D array, and for each element in this array stores the result of the binary search. 
 - `double** create_union(double **p, int *psize);`  
    returns `double** p_new`, an array with `p[0]` containing all the elements of each array, with `p[1]` to `p[NUM_LINES]` containing the results of the binary search for that element.
 - `int* union_search(double **p_new, double q, int *results);`  
    For union array `p_new`, and query `q`, stores the result of the binary search on `q` in `results[i]` for each row `i`

    METHOD 3  
    The third method is implemented as described in the linked PDF.
 - `double ***create_augarray(double **p, int*psize, int*paugsize);`  
    Returns `double***p_aug` which contains the augmented array as described. `p_aug[i][0]` refers to $M_i$. `p_aug[i][1]` are the pointers $p_1$ which are the indices of the value $x$ stored at `p_aug[i][0][x]` in $L_i$. `p_aug[i][2]` are the pointers $p_2$ which are the approximate indices of the value $x$ stored at `p_aug[i][0][x]` in $M_{i+1}$.  
 - `int* aug_search(double*** p_aug, double**p, int*paugsize, int*psize, double q, int *results);`  
    For augmented array `p_aug`, original array `p`, array of size of augmented rows `paugsize`, array of size of original rows `psize`, and query `q`, stores the result of the binary search on `q` in `results[i]` for each row `i`

    METHOD 4  
    The fourth method using integer hashing to decide which bucket a value falls in to reduce the search space.
 - `int*** create_U_array(double**p, int*psize, int m_bins, double* bin_edges);`  
    Create $U$ array as mentioned of bin size `m_bins`. Returns  `int***p_u_array` with `p_u_array[i][m]` referring to a 2-element array of the search bound indices for each bucket `m` and row `i`. `p_u_array[i][m][0]` is the lower bound, and `p_u_array[i][m][1]` is the higher bound.  
 - `int* u_array_search(int***p_u_array, double**p, int m_bins, double q, int* results);`  
    For $U$ array `p_u_array`, original array `p`, number of bins `m_bins`, and query `q`, stores the result of the binary search on `q` in `results[i]` for each row `i`


# Tests

## Accuracy  

Here, firstly we need to check if our search results are "correct". Ideally, they all return the same result. To ensure this is the case, we check the naive search results (method 1) against all other search results (methods 2, 3, and 4) and log if there is a mismatch. There is no logging therefore we can assume are methods work correctly.

Now we shall compare the first 3 methods first before delving deeper into the 4th method

## Space and Time Complexity  

Comparing the time complexity, we can divide them primarily into 2 types of time - 
 - Time to create array(s)
 - Time to search array(s)

### Time taken to create arrays

Here, between methods 1, 2, and 3, we see the basic 2-D array used in naive searching is the fastest to be created, taking ~1-2s. The Union array used in method 2 takes an order of magnitude higher at ~40-45s while the augmented array in method 3 takes another order of magnitude higher at ~350-375s.

Personally, I do feel that the algorithm for the last case can be made faster but regardless, the almost 10x time taken tells us it is a fairly slow algorithm to get started with

### Time taken to search arrays

We perform 10,000,000 (10E7) random searches in the problem search. For this number of lookups, the naive algorithm (method 1) takes twice as long as the union search algorithm (method 2) at ~500-550s vs ~250-275s. The time taken for the 3rd augmented array algorithm is ~300-330s, so it is comparable to the union search algorithm however it is slightly slower.

Comparing the total time taken, for our large arrays, we see the naive algorithm at ~500-550ss, the union search algorithm at ~300-325s and the augmented array algorithm at ~650-700s. As we can see, for 10E7 lookups, the second algorithm does quite well. For 10E8 lookups, the numbers would be closer to ~5200s, ~3250s and ~3500s approximately. At that scale, we can see the cost of the creation of the array in the 3rd method is amortised and it becomes faster than the naive search option, even in our not perfect implementation.

### Space taken

While the union search algorithm is faster, it does take substantially more memory with the elements stored in the order of ~10E8 elements. Compared to this, given our arrays, the naive method only takes up as much space as the number of elements, i.e. in the order of ~10E6. The union search algorithm takes `NUM_LINES` times as much space as the naive method. The third augmented array method is a great compromise bewteen these methods, with the space taken only being 2 times as much as in the naive method with a massive speedup. The cost of the creation of the array can be amortised over a large number of lookups, or even parallelised.

### The hashing into buckets method.

To compare this method, first we compare it to itself across multiple bucket sizes. We go through the following bucket sizes - [10, 50, 100, 500, 1000, 5000, 10000]

As we increase the number of buckets, our search space gets smaller and smaller however the cost of access creation goes higher and higher as the arrays get larger. The optimal number for our arrays seems to be in the range 500-1000 from the tries we did with the time for creation being negligible, and the total search time being ~180-200s, way faster than any of our previous methods! In terms of space too, we use the original array with ~10E6 elements, and along with that, we add NUM_LINES*2*m_bins elements which is a negligible number for 1000 buckets!!

# Original log -

## Run 1

```
Total time read: 2.387803
Total time create union: 42.569514
Total time create aug: 359.862657
Total time naive: 503.128099
Total time union: 244.107595
Total time aug: 301.541383
Total elements stored in naive method:  1432452
Total elements stored in union method: 97406736
Total elements stored in aug method:    2771074


 Trying U method 
bin size: 10
Total time create u: 0.000994
Total time u: 243.695893Total elements stored in u method: 1360

bin size: 50
Total time create u: 0.015677
Total time u: 193.798635Total elements stored in u method: 6800

bin size: 100
Total time create u: 0.004318
Total time u: 195.170080Total elements stored in u method: 13600

bin size: 500
Total time create u: 0.044658
Total time u: 189.948911Total elements stored in u method: 68000

bin size: 1000
Total time create u: 0.110040
Total time u: 183.867284Total elements stored in u method: 136000

bin size: 5000
Total time create u: 0.439600
Total time u: 270.670956Total elements stored in u method: 680000

bin size: 10000
Total time create u: 2.189197
Total time u: 472.334587Total elements stored in u method: 1360000
```

# Run 2

```
Total time read: 1.468655
Total time create union: 43.078335
Total time create aug: 371.007272
Total time naive: 534.624268
Total time union: 264.098089
Total time aug: 320.698149
Total elements stored in naive method: 1432452
Total elements stored in union method: 97406736
Total elements stored in aug method: 2771074


 Trying U method 
bin size: 10
Total time create u: 0.001624
Total time u: 272.152436
Total elements stored in u method: 1360

bin size: 50
Total time create u: 0.003646
Total time u: 227.002996
Total elements stored in u method: 6800

bin size: 100
Total time create u: 0.010121
Total time u: 235.123098
Total elements stored in u method: 13600

bin size: 500
Total time create u: 0.060907
Total time u: 230.258996
Total elements stored in u method: 68000

bin size: 1000
Total time create u: 0.107089
Total time u: 258.096477
Total elements stored in u method: 136000

bin size: 5000
Total time create u: 1.178349
Total time u: 581.967771
Total elements stored in u method: 680000

bin size: 10000
Total time create u: 2.395196
Total time u: 579.401734
Total elements stored in u method: 1360000

```

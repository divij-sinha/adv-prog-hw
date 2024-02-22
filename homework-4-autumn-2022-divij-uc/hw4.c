#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "timer.h"

#define NUM_LINES 68
#define N_ITERS 10000000

int TOTAL_UNION = 0; /* total number of elements */
double MIN = __DBL_MAX__; /* min value across all elements */
double MAX = __DBL_MIN__; /* max value across all elements */

double** parse_file(double **p, int *psize, FILE*datafile); /* read text file to array */
int binary_search(int l, int h, double*array, double q); /* function to perform binary search */
/* METHOD 1 */
int* raw_search(double **p, int *psize, double q, int* results); /* brute force search for query on each array */
/* METHOD 2 */
double** create_union(double **p, int *psize); /* create large array of all elements */
int* union_search(double **p_new, double q, int *results); /* perform search for method 2 */
/* METHOD 3 */
double ***create_augarray(double **p, int*psize, int*paugsize); /* create augmented array of as described */
int* aug_search(double*** p_aug, double**p, int*paugsize, int*psize, double q, int *results); /* perform search for method 3 */
/* METHOD 4 */
int*** create_U_array(double**p, int*psize, int m_bins, double* bin_edges); /* create buckets array */
int* u_array_search(int***p_u_array, double**p, int m_bins, double q, int* results); /* perform search for method 4 */

int main(int argc, char **argv) {
    
    srand(234343);
    char * filename = NULL;
    FILE * datafile;

    double **p;
    int *psize;
    psize = (int*)malloc(sizeof(int)*(NUM_LINES));

    double q, totalTime_naive  = 0.0, totalTime_read = 0.0;
    double totalTime_createunion = 0.0, totalTime_union = 0.0;
    double totalTime_createaug = 0.0, totalTime_aug = 0.0;

    int **results_naive = (int**)malloc(sizeof(int*)*N_ITERS);
    int **results_union = (int**)malloc(sizeof(int*)*N_ITERS);
    int **results_aug = (int**)malloc(sizeof(int*)*N_ITERS);
    int **results_u = (int**)malloc(sizeof(int*)*N_ITERS);
    for(int i = 0; i < N_ITERS; i ++) {
        results_naive[i] = (int*)malloc(sizeof(int)*NUM_LINES);
        results_union[i] = (int*)malloc(sizeof(int)*NUM_LINES);
        results_aug[i] = (int*)malloc(sizeof(int)*NUM_LINES);
        results_u[i] = (int*)malloc(sizeof(int)*NUM_LINES);
    }

    filename = argv[1];
    datafile = fopen(filename,"r");
    StartTimer();
    p = parse_file(p, psize, datafile);             /* read file */
    fclose(datafile);
    totalTime_read = GetTimer() / 1000.0;
    
    StartTimer();
    double** p_new;
    p_new = create_union(p, psize);             /* create arr for method 2 */
    totalTime_createunion = GetTimer() / 1000.0;     

    StartTimer();
    double*** p_aug;
    int*paugsize;
    paugsize = (int*)malloc(sizeof(int)*(NUM_LINES+1));
    p_aug = create_augarray(p, psize, paugsize);    /* create arr for method 3 */
    totalTime_createaug = GetTimer() / 1000.0;

    /* MAIN SEARCH LOOP*/
    double* querys;
    querys = malloc(sizeof(double)*N_ITERS);
    for(int search_iter = 0; search_iter < N_ITERS; search_iter++){

        q = MIN + ((float)rand()/RAND_MAX)*(MAX-MIN);
        querys[search_iter] = q;

        StartTimer();
        raw_search(p, psize, q, results_naive[search_iter]);
        const double tElapsed_naive = GetTimer() / 1000.0;
        if (search_iter > 1) {                          
            totalTime_naive += tElapsed_naive;
        }

        StartTimer();
        union_search(p_new, q, results_union[search_iter]);
        const double tElapsed_union = GetTimer() / 1000.0;
        if (search_iter > 1) {                          
            totalTime_union += tElapsed_union;
        }

        StartTimer();
        aug_search(p_aug, p, paugsize, psize, q, results_aug[search_iter]);
        const double tElapsed_aug = GetTimer() / 1000.0;
        if (search_iter > 1) {                          
            totalTime_aug += tElapsed_aug;
        }

        for(int j = 0; j < NUM_LINES; j++){
            if (results_naive[search_iter][j] != results_union[search_iter][j]){
                printf("\nFAIL! 1");
            }
            if (results_naive[search_iter][j] != results_aug[search_iter][j]){
                printf("\nFAIL! 2");
            }
        }
    }


    printf("\n\nTotal time read: %f\n", totalTime_read);
    printf("Total time create union: %f\n", totalTime_createunion);
    printf("Total time create aug: %f\n", totalTime_createaug);
    printf("Total time naive: %f\n", totalTime_naive);
    printf("Total time union: %f\n", totalTime_union);
    printf("Total time aug: %f\n", totalTime_aug);

    printf("Total elements stored in naive method: %d\n", TOTAL_UNION);
    printf("Total elements stored in union method: %d\n", TOTAL_UNION*NUM_LINES);
    int s = 0;
    for(int i =0; i < NUM_LINES; i++) s+= paugsize[i];
    printf("Total elements stored in aug method: %d\n", s);
    

    /* perform method 4 for variable bin sizes */
    printf("\n\n Trying U method ");
    int mbins_sizes[7] = {10, 50, 100, 500, 1000, 5000, 10000};
    for(int binidx = 0; binidx < 7; binidx++){
        double totalTime_createu = 0.0, totalTime_u = 0.0;
        printf("\nbin size: %d", mbins_sizes[binidx]);
        StartTimer();
        int m_bins = mbins_sizes[binidx];
        int***p_U_array;
        double*bin_edges;
        bin_edges = malloc(sizeof(double)*(m_bins+1));
        p_U_array = create_U_array(p, psize, m_bins, bin_edges);
        totalTime_createu = GetTimer() / 1000.0;
        printf("\nTotal time create u: %f", totalTime_createu);

        for(int search_iter = 0; search_iter < N_ITERS; search_iter++){
            StartTimer();
            q = querys[search_iter];
            u_array_search(p_U_array, p, m_bins, q, results_u[search_iter]);
            const double tElapsed_u = GetTimer() / 1000.0;
            if (search_iter > 1) {                          
                totalTime_u += tElapsed_u;
            }

            for(int j = 0; j < NUM_LINES; j++){
                if (results_naive[search_iter][j] != results_u[search_iter][j]){
                printf("\nFAIL! 3");
                }
            }
        }
        printf("\nTotal time u: %f", totalTime_u);
        printf("\nTotal elements stored in u method: %d\n", m_bins*NUM_LINES*2);

        /* free pu array */
        for(int i=0; i < m_bins; i++){
            for(int j = 0; j < NUM_LINES; j++){
                free(p_U_array[i][j]);
            }
            free(p_U_array[i]);
        }
        free(p_U_array);
        free(bin_edges);
    }
    
    /* free memory */
    /* free results */
    for(int i = 0; i < N_ITERS; i++){
        free(results_naive[i]);
        free(results_aug[i]);
        free(results_union[i]);
        free(results_u[i]);
    }
    free(results_naive);
    free(results_aug);
    free(results_union);
    free(results_u);
    free(psize);
    free(paugsize);
    /* free arrays */
    free(p_new[NUM_LINES]);
    for(int i =0; i < NUM_LINES; i++){
        free(p[i]);
        for(int j = 0; j < 3; j++){
            free(p_aug[i][j]);
        }
        free(p_aug[i]);
        free(p_new[i]);
    }
    free(p);
    free(p_aug);
    free(p_new);
    free(querys);

}


/* read text file to array */
double** parse_file(double **p, int *psize, FILE*datafile){

    p = (double**)malloc(sizeof(double*)*(NUM_LINES));
    int n_ctr=0,l_ctr=0,e_ctr=0,cur_line=0,e_flag=0, cur_char = '0';
    char cur_str[100], *end_ptr, e_str[100];
    double cur;
    
    while(cur_char != EOF){
        cur_char = fgetc(datafile);
        if(cur_char == ' '){
            cur_str[l_ctr] = '\0';
            if(n_ctr == 0){
                p[cur_line] = malloc(atoi(cur_str)*sizeof(double));
                psize[cur_line] = atoi(cur_str);
                TOTAL_UNION += psize[cur_line];
                n_ctr++;
            }
            else{
                cur = strtod(cur_str, &end_ptr);
                if(e_flag == 1){
                    cur *= powl(10.0,atoi(e_str));
                }
                p[cur_line][n_ctr-1] = cur;
                n_ctr++;
                if(MAX < cur) MAX = cur;
                if(MIN > cur) MIN = cur;

            } 
            l_ctr = 0; e_ctr = 0; e_flag = 0;
        }
        else if(cur_char == '\n'){
            cur_line++; n_ctr = 0; e_flag = 0;
        }
        else if(cur_char == 'E'){
            e_flag = 1;
        }
        else if(e_flag == 0){
            cur_str[l_ctr] = cur_char;
            l_ctr++;
        }
        else{
            e_str[e_ctr] = cur_char;
            e_ctr++;
        }
    }
    return p;
}

/* function to perform binary search */
int binary_search(int l, int h, double*array, double q){
    int cur_idx = (int)((h+l)/2);
    int window = h-l;
    if(window<=1){
        if(array[l] >= q) return l;
        else return h;
    } 
    else if(array[cur_idx] >= q) 
        cur_idx = binary_search(l,cur_idx,array,q);
    else cur_idx = binary_search(cur_idx,h,array,q);
    return cur_idx;
}

/* METHOD 1 */
/* brute force search for query on each array */
int* raw_search(double **p, int *psize, double q, int* results){
    int i,n;
    for(i=0; i<NUM_LINES;i++){
        n = psize[i];
        results[i] = binary_search(0, n, p[i], q);
    }
}

/* METHOD 2 */
/* create large array of all elements */
double** create_union(double **p, int *psize){
    double** p_new;
    p_new = malloc(sizeof(double*)*(NUM_LINES + 1));
    int i, min_idx = 0;
    double cur_arr[NUM_LINES];      /* array of current min val across NUM_LINES arrays */
    int cur_idx[NUM_LINES];         /* array of idx of current min val across NUM_LINES arrays */
    double min = __DBL_MAX__;
    for(i = 0; i < NUM_LINES; i++) {
        p_new[i] = malloc(sizeof(double)*TOTAL_UNION);
        cur_idx[i] = 0;
        cur_arr[i] = p[i][0]; /* set each current idx to 0, and each value to the first value */
    }
    p_new[NUM_LINES] = malloc(sizeof(double)*TOTAL_UNION); /* allocate extra row to hold values along with previously allocated rows to hold NUM_LINES indices */
    
    for(i = 0; i < TOTAL_UNION; i++){
        min = __DBL_MAX__;
        for(int k = 0; k < NUM_LINES; k++) 
            if(cur_arr[k] < min){
                min_idx = k;
                min = cur_arr[min_idx];
            } 
        p_new[0][i] = cur_arr[min_idx]; /* store next value in array */
        cur_idx[min_idx]++;
        if(cur_idx[min_idx] == psize[min_idx]) cur_arr[min_idx] = __DBL_MAX__; /* mark array as completed */
        else cur_arr[min_idx] = p[min_idx][cur_idx[min_idx]];
        for(int j = 0; j < NUM_LINES; j++){
            p_new[j+1][i] = binary_search(0, psize[j], p[j], p_new[0][i]);  /* store all indices for value */
        }
    }
    return p_new;
}

/* perform search for method 2 */
int* union_search(double **p_new, double q, int *results){
    int j = binary_search(0, TOTAL_UNION, p_new[0], q);
    for(int i = 0; i<NUM_LINES; i++){
        results[i] = p_new[i+1][j];
    } 
}

/* METHOD 3 */
/* create augmented array of as described */
double ***create_augarray(double **p, int*psize, int*paugsize){
    paugsize[NUM_LINES] = 0;
    double ***p_aug = (double***)malloc(sizeof(double**)*NUM_LINES);
    for(int i = NUM_LINES-1; i >= 0; i--){
        paugsize[i] = psize[i] + (int)((paugsize[i+1]+1)/2);  /* find size of row */
        p_aug[i] = malloc(sizeof(double*)*3);
        p_aug[i][0] = malloc(sizeof(double)*paugsize[i]);
        p_aug[i][1] = malloc(sizeof(double)*paugsize[i]);
        p_aug[i][2] = malloc(sizeof(double)*paugsize[i]);
        for(int j = 0; j < psize[i]; j++){  /* populate Mi with Li */
            p_aug[i][0][j] = p[i][j];
            p_aug[i][1][j] = (double)j;
            if(i != NUM_LINES-1)
                p_aug[i][2][j] = (double)binary_search(0, paugsize[i+1], p_aug[i+1][0], p[i][j]);
            else
                p_aug[i][2][j] = 0;

        }
        for(int j = psize[i]; j < paugsize[i]; j++){    /* populate Mi with values from Mi+1 */
            int k = (j-psize[i])*2+1; /* get every second index */
            if (paugsize[i+1] % 2 == 1) k -=1; /* ensure every second index includes the largest value of row */
            double ins = p_aug[i+1][0][k];
            double idx = (double)binary_search(0, psize[i], p[i], ins);  /* find ptr1 */
            double idx2 = (double)binary_search(0, paugsize[i+1], p_aug[i+1][0], ins);  /* find ptr2 */

            int ins_idx = binary_search(0, j, p_aug[i][0], ins); /* find pos of where to insert val from Mi+1 */
            for(int l = j-1; l >= ins_idx; l--){
                    p_aug[i][0][l+1] = p_aug[i][0][l];
                    p_aug[i][1][l+1] = p_aug[i][1][l];
                    p_aug[i][2][l+1] = p_aug[i][2][l];
            }
            p_aug[i][0][ins_idx] = ins;
            p_aug[i][1][ins_idx] = idx;
            p_aug[i][2][ins_idx] = idx2;
        }
    }
    return p_aug;
}

/* perform search for method 3 */
int* aug_search(double*** p_aug, double**p, int*paugsize, int*psize, double q, int *results){
    int idx = binary_search(0, paugsize[0]-1, p_aug[0][0], q);
    int cur_row = (int)p_aug[0][1][idx];
    int next_row = (int)p_aug[0][2][idx];
    results[0] = cur_row;
 
    for(int i = 1; i < NUM_LINES; i++){
        if(p_aug[i][0][next_row-1] >= q | next_row == paugsize[i])
            idx = next_row-1;
        else
            idx = next_row;
        
        cur_row = (int)p_aug[i][1][idx];
        next_row = (int)p_aug[i][2][idx];

        results[i] = cur_row;

    }
}

/* METHOD 4 */
/* create buckets array */
int*** create_U_array(double**p, int*psize, int m_bins, double* bin_edges){
    int***p_u_array;
    p_u_array = malloc(sizeof(double**)*(m_bins));
    bin_edges[0] = MIN;
    
    for(int i=1; i < m_bins+1; i++){
        bin_edges[i] = MIN + (MAX-MIN)*i/m_bins; /* calc bin edges */
        p_u_array[i-1] = malloc(sizeof(double*)*NUM_LINES);
        for(int j = 0; j < NUM_LINES; j++){
            int b1, b2;
            b1 = binary_search(0, psize[j], p[j], bin_edges[i-1]);  /* find lower bound */
            b2 = binary_search(0, psize[j], p[j], bin_edges[i]);    /* find upper bound */
            p_u_array[i-1][j] = malloc(sizeof(double)*2);
            p_u_array[i-1][j][0] = b1;
            p_u_array[i-1][j][1] = b2;
        }
    }
    return p_u_array;
}

/* perform search for method 4 */
int* u_array_search(int***p_u_array, double**p, int m_bins, double q, int* results){
    int bin = (int)(((q - MIN)*m_bins)/(MAX-MIN));
    for(int i = 0; i < NUM_LINES; i++){
        results[i] = binary_search(p_u_array[bin][i][0], p_u_array[bin][i][1], p[i], q);
    }
}
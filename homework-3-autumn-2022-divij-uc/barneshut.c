#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "quadtree.h"

/* randomly initialize particle positions and momenta */
void ran_init(Particle *p, int n) {
  for (int i = 0; i < n; i++) {
    p[i].x = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].y = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].vx = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].vy = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].mass = 1.0f;
  }
}

void qtree_fprint(FILE *datafile, QuadTreeNode *node, int depth){
    fprintf(datafile,"\n%d %.4f %.4f %.4f %.4f %d %.3f %.3f", depth, node->bounds[0], node->bounds[1], node->bounds[2], node->bounds[3], node->n_of_children, node->center_of_mass.x, node->center_of_mass.y);
    if(node->n_of_children == 1){
    fprintf(datafile, " %.3f %.3f", node->part->x, node->part->y);
  }
  if(node->n_of_children == 4){
    qtree_fprint(datafile, node->children[0], depth+1);
    qtree_fprint(datafile, node->children[1], depth+1);
    qtree_fprint(datafile, node->children[2], depth+1);
    qtree_fprint(datafile, node->children[3], depth+1);
  }
}

int main(const int argc, const char** argv) {
  FILE *datafile = NULL, *datafile_Q = NULL;      /* output file for particle positions */
  int   nParticles  = 3000;      /* number of particles */

  int seed = 1234;
  srand(seed);

  if (argc > 1)
    nParticles      = atoi(argv[1]);

  const float dt    = 0.0001f; /* time step   */
  const int nIters  = 200;   /* number of steps in simulation */
  float BOUND = 1.00f;
  float thresh = 1.00f;

  Particle  *p          = (Particle *) malloc(nParticles*sizeof(Particle));
  Particle center_of_mass;
  QuadTreeNode *root_node = malloc(sizeof(QuadTreeNode));

  ran_init(p, nParticles); /* Init pos and vel data */

  qtree_init_node(root_node, 1.00f, -1.00f, -1.00f, 1.00f);
  for(int i = 0; i < nParticles; i++){
    qtree_add(root_node, &p[i]);
  }

  center_of_mass = qtree_center_of_mass(root_node);

  datafile = fopen("init_grid.dat", "w");
  qtree_fprint(datafile, root_node, 0);
  fclose(datafile);

  double totalTime  = 0.0;

  datafile_Q        = fopen("particles_bh.dat","w");
  fprintf(datafile_Q,"%d %d %d\n", nParticles, nIters, 0);


  /* ------------------------------*/
  /*     MAIN LOOP                 */
  /* ------------------------------*/
  printf("\ndt: %.6f, thresh: %.3f, n: %d, niter: %d ", dt, thresh, nParticles, nIters);
  for (int iter = 1; iter <= nIters; iter++) {
    /* printf("iteration:%d\n", iter); */

    for (int i = 0;i < nParticles; ++i){
      /* fprintf(datafile, "%f %f \n", p[i].x, p[i].y); */
      fprintf(datafile_Q, "%f %f \n", p[i].x, p[i].y);
    }
    
    StartTimer();

    qtree_calc_force_find_node(root_node, root_node, dt, thresh);
    BOUND = qtree_update(root_node, dt, BOUND);
    if (iter == nIters-1){
        datafile = fopen("final_grid.dat", "w");
        qtree_fprint(datafile, root_node, 0);
        fclose(datafile);
    }
    qtree_free(root_node);
    root_node = malloc(sizeof(QuadTreeNode));
    qtree_init_node(root_node, BOUND, -1*BOUND, -1*BOUND, BOUND);
    for(int i=0;i<nParticles;i++){
      qtree_add(root_node, &p[i]);
      }

    center_of_mass = qtree_center_of_mass(root_node);

    const double tElapsed = GetTimer() / 1000.0;
    if (iter > 1) {                          /* First iter is warm up */
      totalTime += tElapsed;
    }
  }

  fclose(datafile_Q);
  double avgTime = totalTime / (double)(nIters-1);

  printf("avgTime: %f   totTime: %f \n", avgTime, totalTime);
  printf("\nBOUND %.2f\n", BOUND);

  free(p);
  qtree_free(root_node);
}

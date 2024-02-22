#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

#define SOFTENING 1e-9f

int BOUND_X = 1;
int BOUND_Y = 1;

typedef struct {
  float x, y, z;        /* particle positions */
  float vx, vy, vz;     /* particle momenta */
  float mass;
} Particle;

typedef struct {
  float Fx, Fy, Fz;
} Force;

typedef struct quad_tree_node
{
  Particle *part;
  int n_of_children;
  float bounds[4]; /* up down left right */
  Particle center_of_mass;
  struct quad_tree_node *(children[4]); /* up-left up-right down-right down-left */

} QuadTreeNode;

void qtree_add(QuadTreeNode* node, Particle* part);

void qtree_null_child(QuadTreeNode* node){
  node->children[0] = NULL;
  node->children[1] = NULL;
  node->children[2] = NULL;
  node->children[3] = NULL;
}

void qtree_init_node(QuadTreeNode* node, float up, float down, float left, float right){
    node->n_of_children = 0;
    node->bounds[0] = up; /* up */
    node->bounds[1] = down; /* down */
    node->bounds[2] = left; /* left */
    node->bounds[3] = right; /* right */
    node->children[0] = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    node->children[1] = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    node->children[2] = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    node->children[3] = (QuadTreeNode*)malloc(sizeof(QuadTreeNode));
    qtree_null_child(node->children[0]);
    qtree_null_child(node->children[1]);
    qtree_null_child(node->children[2]);
    qtree_null_child(node->children[3]);
    node->part = NULL;
    node->center_of_mass.mass = 0;
    node->center_of_mass.x = 0;
    node->center_of_mass.y = 0;
    node->center_of_mass.vx = 0;
    node->center_of_mass.vy = 0;
}

int qtree_find_child_idx(QuadTreeNode* node, Particle* part){
    float ud_center = (node->bounds[0] + node->bounds[1])/2;
    float lr_center = (node->bounds[2] + node->bounds[3])/2;
    if (part->x > lr_center)
        if (part->y > ud_center)
            return 1;
        else
            return 2;
    else
        if (part->y > ud_center)
            return 0; 
        else
            return 3;
}

void qtree_add(QuadTreeNode* node, Particle* part){
  /* CHANGE ORDER TO 4 1 0 */
    int idx = -1;
    if(node->n_of_children == 0){
        node->part = part;
        node->n_of_children = 1;
    }
    else if(node->n_of_children == 1){
        float ud_center = (node->bounds[0] + node->bounds[1])/2;
        float lr_center = (node->bounds[2] + node->bounds[3])/2;
        qtree_init_node(node->children[0], node->bounds[0], ud_center, node->bounds[2], lr_center);
        qtree_init_node(node->children[1], node->bounds[0], ud_center, lr_center, node->bounds[3]);
        qtree_init_node(node->children[2], ud_center, node->bounds[1], lr_center, node->bounds[3]);
        qtree_init_node(node->children[3], ud_center, node->bounds[1], node->bounds[2], lr_center);
        node->n_of_children = 4;
        idx = qtree_find_child_idx(node, node->part);
        qtree_add(node->children[idx], node->part);
        idx = qtree_find_child_idx(node, part);
        qtree_add(node->children[idx], part);
        node->part = NULL;

    }
    else{
        idx = qtree_find_child_idx(node, part);
        qtree_add(node->children[idx], part);
    }
}


Particle qtree_center_of_mass(QuadTreeNode *node){
  /* CHANGE ORDER TO 4 1 0 */
  if(node->n_of_children == 1){
    node->center_of_mass.mass = node->part->mass;
    node->center_of_mass.x = node->part->x;
    node->center_of_mass.y = node->part->y;
    node->center_of_mass.vx = node->part->vx;
    node->center_of_mass.vy = node->part->vy;
    return *(node->part);
  }
  else if(node->n_of_children == 0){
    node->center_of_mass.x = 0.0f;
    node->center_of_mass.y = 0.0f;
    node->center_of_mass.mass = 0.0f;
    node->center_of_mass.vx = 0.0f;
    node->center_of_mass.vy = 0.0f;
    return node->center_of_mass;
  }
  else{
    Particle child_part;
    for(int i = 0; i < 4; i++){
      child_part = qtree_center_of_mass(node->children[i]);
      node->center_of_mass.mass += child_part.mass;
      node->center_of_mass.x += (child_part.x*child_part.mass);
      node->center_of_mass.y += (child_part.y*child_part.mass);
    }
    node->center_of_mass.x /= node->center_of_mass.mass;
    node->center_of_mass.y /= node->center_of_mass.mass;
    return node->center_of_mass;
  }
}

/* randomly initialize particle positions and momenta */
void ran_init(Particle* p, int n) {
  for (int i = 0; i < n; i++) {
    p[i].x = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].y = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].z = 0.0f;
    p[i].vx = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].vy = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
    p[i].vz = 0.0f;
    p[i].mass = 1.0f;
  }
}


void qtree_update(QuadTreeNode* node, float dt, int depth){
  if(node->n_of_children == 4){
    for(int i=0; i < 4; i++){
      qtree_update(node->children[i], dt, depth+1);
    }
  }
  if(node->n_of_children == 1){
      node->part->x += node->part->vx*dt;
      node->part->y += node->part->vy*dt;
      node->part->z += node->part->vz*dt;
      float x_lim, y_lim;
      x_lim = ceil(fabs(node->part->x)); 
      y_lim = ceil(fabs(node->part->y)); 
      if(x_lim > BOUND_X) BOUND_X = x_lim; 
      if(y_lim > BOUND_Y) BOUND_Y = y_lim;
      (BOUND_X > BOUND_Y) ? (BOUND_Y = BOUND_X) : (BOUND_X = BOUND_Y);
    }
}

void qtree_free(QuadTreeNode* node){
  /* printf("D:%d noc:%d bounds udlr: %f %f %f %f\n", depth, node->n_of_children, node->bounds[0], node->bounds[1], node->bounds[2], node->bounds[3]); */
  for(int i=0; i<4;i++){
    if(node->children[i] != NULL)
      qtree_free(node->children[i]);
  }
  free(node);
}


/* calculate all interparticle forces and update instantaneous velocities */
void calc_force(Particle *p, float dt, int n) {
  for (int i = 0; i < n; i++) {
    float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;

    for (int j = 0; j < n; j++) {
      /* calculate net particle force on i'th particle */
      if (j != i) {
        float dx = p[j].x - p[i].x;
        float dy = p[j].y - p[i].y;
        float dz = p[j].z - p[i].z;
        float distSqr = dx*dx + dy*dy + dz*dz + SOFTENING;
        float invDist = 1.0f / sqrtf(distSqr);
        float invDist3 = invDist * invDist * invDist;

        Fx += dx * invDist3; Fy += dy * invDist3; Fz += dz * invDist3;
      }
    }
    /* update instantaneous velocity based on force and timestep */
    p[i].vx += dt*Fx; p[i].vy += dt*Fy; p[i].vz += dt*Fz;
  }
}

Force qtree_calc_force(QuadTreeNode* cur_node, QuadTreeNode* force_node, float dt, float thresh){
  /* TODO: ADD MASS */
  Force F;
  F.Fx = 0;
  F.Fy = 0;
  F.Fz = 0;
  if (force_node->n_of_children != 0) {
    float dist, dx, dy;
    Particle p1, p2;
    p1 = cur_node->center_of_mass;
    p2 = force_node->center_of_mass;
    dx = (p2.x-p1.x);
    dy = (p2.y-p1.y);
    dist = sqrtf(dx*dx + dy*dy + SOFTENING);
    if(((dist > thresh) | (force_node->n_of_children == 1)) & (cur_node != force_node)){
      float invDist = 1/dist;
      float invDist3 = invDist * invDist * invDist;
      F.Fx += (p1.mass * p2.mass * dx * invDist3); F.Fy += (p1.mass * p2.mass * dy * invDist3);
    }
    else if (force_node->n_of_children == 4){
      Force Fd;
      for(int i = 0; i < 4; i++){
        Fd = qtree_calc_force(cur_node, force_node->children[i], dt, thresh);
        F.Fx += Fd.Fx;
        F.Fy += Fd.Fy;
      }
    }
  }
  return F;
}


void qtree_calc_force_find_node(QuadTreeNode* cur_node, QuadTreeNode* root_node, float dt, float thresh) {
  if(cur_node->n_of_children == 4){
    qtree_calc_force_find_node(cur_node->children[0], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[1], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[2], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[3], root_node, dt, thresh);
  }
  else if (cur_node->n_of_children == 1)
  {
    Force F;
    F = qtree_calc_force(cur_node, root_node, dt, thresh);
    cur_node->part->vx += dt*F.Fx; cur_node->part->vy += dt*F.Fy;
  }
  
}

float x=0, y=0;


void qtree_print(QuadTreeNode *node, int depth){
  printf("\nDepth: %d, children: %d, CoM: %.3f, %.3f", depth, node->n_of_children, node->center_of_mass.x, node->center_of_mass.y);
  if(node->n_of_children == 1){
    printf(" part: %.3f, %.3f", node->part->x, node->part->y);
  }
  else if(node->n_of_children == 4){
    qtree_print(node->children[0], depth+1);
    qtree_print(node->children[1], depth+1);
    qtree_print(node->children[2], depth+1);
    qtree_print(node->children[3], depth+1);
  }
}

float MAX_X = 0; float MAX_Y = 0;

int main(const int argc, const char** argv) {
  FILE *datafile = NULL, *datafile_P    = NULL;      /* output file for particle positions */
  int   nParticles  = 3000;      /* number of particles */
  int nIters  = 200;   /* number of steps in simulation */

  int seed = 839428479;
  srand(seed);

  QuadTreeNode *root_node = NULL;
  Particle center_of_mass;
  int i;

  root_node = malloc(sizeof(QuadTreeNode));
  qtree_init_node(root_node, 1.00f, -1.00f, -1.00f, 1.00f);

  if (argc > 1)
    nParticles      = atoi(argv[1]);

  const float dt    = 0.01f; /* time step   */

  Particle  *p          = malloc(nParticles*sizeof(Particle));
  Particle  *p_qt          = malloc(nParticles*sizeof(Particle));

  ran_init(p, nParticles); /* Init pos and vel data */
  memcpy(p_qt, p, nParticles*sizeof(Particle));

  for(i=0;i<nParticles;i++){
    qtree_add(root_node, &p_qt[i]);
    }
  /* center_of_mass = qtree_center_of_mass(root_node); */

  double totalTime  = 0.0;

  datafile          = fopen("particles_serial.dat","w");
  fprintf(datafile,"%d %d %d\n", nParticles, nIters, 0);

  datafile_P        = fopen("particles_bh.dat","w");
  fprintf(datafile_P,"%d %d %d\n", nParticles, nIters, 0);

  center_of_mass = qtree_center_of_mass(root_node);
  qtree_print(root_node, 0);

  /* ------------------------------*/
  /*     MAIN LOOP                 */
  /* ------------------------------*/
  for (int iter = 1; iter <= nIters; iter++) {
    printf("\niteration:%d", iter);

    for (int i = 0;i < nParticles; ++i){
      fprintf(datafile, "%f %f %f \n", p[i].x, p[i].y, p[i].z);
      fprintf(datafile_P, "%f %f %f \n", p_qt[i].x, p_qt[i].y, p_qt[i].z);
    }

    StartTimer();

    calc_force(p, dt, nParticles);           /* compute interparticle forces and update vel */

    x = 0; y = 0;
    for (int i = 0 ; i < nParticles; i++) {  /* compute new position */
      p[i].x += p[i].vx*dt;
      p[i].y += p[i].vy*dt;
      p[i].z += p[i].vz*dt;
      
      x+=p[i].x; y+=p[i].y;
      if(p[i].x > MAX_X) MAX_X = p[i].x;
      if(p[i].y > MAX_Y) MAX_Y = p[i].y;
    }

    qtree_calc_force_find_node(root_node, root_node, dt, __FLT_MAX__);
    qtree_update(root_node, dt, 0);
    qtree_free(root_node);
    root_node = malloc(sizeof(QuadTreeNode));
    qtree_init_node(root_node, BOUND_X, -1*BOUND_X, -1*BOUND_Y, BOUND_Y);
    for(i=0;i<nParticles;i++){
      qtree_add(root_node, &p_qt[i]);
      }

    center_of_mass = qtree_center_of_mass(root_node);


    const double tElapsed = GetTimer() / 1000.0;
    if (iter > 1) {                          /* First iter is warm up */
      totalTime += tElapsed;
    }
  }

  fclose(datafile);
  double avgTime = totalTime / (double)(nIters-1);

  printf("\n\n\nfc com: %.4f %.4f", x/(nParticles), y/(nParticles));
  printf("\nq tree: %.4f %.4f", center_of_mass.x, center_of_mass.y);


  printf("\n\navgTime: %f   totTime: %f \n", avgTime, totalTime);
  printf("\nMAX X %.2f, MAX Y %.2f", MAX_X, MAX_Y);

  free(p);
  /* qtree_free(root_node); */
}

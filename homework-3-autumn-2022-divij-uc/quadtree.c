#include "quadtree.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SOFTENING 1e-6f

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
    /* node->center_of_mass.z = 0; */
    node->center_of_mass.vx = 0;
    node->center_of_mass.vy = 0;
    /* node->center_of_mass.vz = 0; */
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
    /* node->center_of_mass.z = 0.0f; */
    node->center_of_mass.mass = 0.0f;
    node->center_of_mass.vx = 0.0f;
    node->center_of_mass.vy = 0.0f;
    /* node->center_of_mass.vz = 0.0f; */
    return node->center_of_mass;
  }
  else{
    Particle child_part;
    for(int i = 0; i < 4; i++){
      child_part = qtree_center_of_mass(node->children[i]);
      node->center_of_mass.mass += child_part.mass;
      node->center_of_mass.x += (child_part.x*child_part.mass);
      node->center_of_mass.y += (child_part.y*child_part.mass);
      /* node->center_of_mass.z += (child_part.z*child_part.mass); */
    }
    node->center_of_mass.x /= node->center_of_mass.mass;
    node->center_of_mass.y /= node->center_of_mass.mass;
    /* node->center_of_mass.z /= node->center_of_mass.mass; */
    return node->center_of_mass;
  }
}

float qtree_update(QuadTreeNode* node, float dt, float BOUND){
  if(node->n_of_children == 4){
    for(int i=0; i < 4; i++){
      BOUND = qtree_update(node->children[i], dt, BOUND);
    }
  }
  if(node->n_of_children == 1){
      node->part->x += node->part->vx*dt;
      node->part->y += node->part->vy*dt;
      /* node->part->z += node->part->vz*dt; */
      float x_lim, y_lim, z_lim;
      x_lim = ceil(fabs(node->part->x)); 
      y_lim = ceil(fabs(node->part->y));
      /* z_lim = ceil(fabs(node->part->z)); */
      if(x_lim > BOUND) BOUND = x_lim; 
      if(y_lim > BOUND) BOUND = y_lim;
      /* if(z_lim > BOUND) BOUND = z_lim; */
    }
    return BOUND;
}

void qtree_free(QuadTreeNode* node){
  /* printf("D:%d noc:%d bounds udlr: %f %f %f %f\n", depth, node->n_of_children, node->bounds[0], node->bounds[1], node->bounds[2], node->bounds[3]); */
  for(int i=0; i<4;i++){
    if(node != NULL)
      qtree_free(node->children[i]);
  }
  free(node);
}

Force qtree_calc_force(QuadTreeNode* cur_node, QuadTreeNode* force_node, float dt, float thresh){
  Force F;
  F.Fx = 0.0f;
  F.Fy = 0.0f;
  /* F.Fz = 0.0f; */
  if (force_node->n_of_children != 0) {
    float dist, dx, dy, dz;
    Particle p1, p2;
    p1 = cur_node->center_of_mass;
    p2 = force_node->center_of_mass;
    dx = (p2.x-p1.x);
    dy = (p2.y-p1.y);
    /* dz = (p2.z-p1.z); */
    /* dist = sqrtf(dx*dx + dy*dy + dz*dz + SOFTENING); */
    dist = sqrtf(dx*dx + dy*dy + SOFTENING);
    if(((dist > thresh) | (force_node->n_of_children == 1)) & (cur_node != force_node)){
      float invDist = 1/dist;
      float invDist3 = invDist * invDist * invDist;
      F.Fx += (p1.mass * p2.mass * dx * invDist3); F.Fy += (p1.mass * p2.mass * dy * invDist3); /* F.Fz += (p1.mass * p2.mass * dz * invDist3); */
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
  if (cur_node->n_of_children == 1)
  {
    Force F;
    F = qtree_calc_force(cur_node, root_node, dt, thresh);
    cur_node->part->vx += dt*F.Fx; cur_node->part->vy += dt*F.Fy; /* cur_node->part->vz += dt*F.Fz; */
  }
  else if(cur_node->n_of_children == 4){
    qtree_calc_force_find_node(cur_node->children[0], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[1], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[2], root_node, dt, thresh);
    qtree_calc_force_find_node(cur_node->children[3], root_node, dt, thresh);
  }
  
  
}

void qtree_print_node(QuadTreeNode* node, int depth){
  printf("\nDepth: %d, Bounds: [%.4f, %.4f, %.4f, %.4f], children: %d, CoM: %.3f, %.3f", depth, node->bounds[0], node->bounds[1], node->bounds[2], node->bounds[3], node->n_of_children, node->center_of_mass.x, node->center_of_mass.y);
  if(node->n_of_children == 1){
    printf(" part: %.3f, %.3f", node->part->x, node->part->y);
  }
}

void qtree_print(QuadTreeNode *node, int depth){
  qtree_print_node(node, depth);
  if(node->n_of_children == 4){
    qtree_print(node->children[0], depth+1);
    qtree_print(node->children[1], depth+1);
    qtree_print(node->children[2], depth+1);
    qtree_print(node->children[3], depth+1);
  }
}

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <pthread.h>

#define SOFTENING 1e-9f

typedef struct {
  float x, y, z;        /* particle positions */
  float vx, vy, vz;     /* particle momenta */
} Particle;

typedef struct {
    int nstart;
    int nend;
    int ntotal;
    Particle *p;
    float dt;
} Params;

pthread_mutex_t mutexsum;

/* randomly initialize particle positions and momenta */
void ran_init(float *data, int n) {
  for (int i = 0; i < n; i++) {
    data[i] = 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
  }
}

/* calculate all interparticle forces and update instantaneous velocities */
void* calc_force(void * params_void) {
  Params params;
  params = *(Params*)(params_void);
  Particle *p;
  float dt;
  int nstart, nend, ntotal;

  p = params.p;
  nstart = params.nstart;
  nend = params.nend;
  dt = params.dt;
  ntotal = params.ntotal;


  for (int i = nstart; i < nend; i++) {
    float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;

    for (int j = 0; j < ntotal; j++) {
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
    /* pthread_mutex_lock(&mutexsum); */
    p[i].vx += dt*Fx; p[i].vy += dt*Fy; p[i].vz += dt*Fz;
    /* pthread_mutex_unlock(&mutexsum); */
  }
  return NULL;
}

int main(const int argc, const char** argv) {
  FILE *datafile    = NULL, *datafile_serial = NULL;      /* output file for particle positions */
  int   nParticles  = 3000;      /* number of particles */
  int seed = 1234;
  srand(seed);

  char char_omp, char_serial;
  int check = 0;

  const float dt    = 0.01f; /* time step   */
  const int nIters  = 200;   /* number of steps in simulation */

  float *buf        =  malloc(nParticles*sizeof(Particle));
  Particle  *p          = (Particle *) buf;

  int nthreads = 16;

  if (argc > 1)
    nParticles      = atoi(argv[1]);

  if (argc == 3)
    nthreads = atoi(argv[2]);

  pthread_t threads[nthreads];
  Params params[nthreads];
  int tn=0;
  void *status;

  ran_init(buf, 6*nParticles); /* Init pos and vel data */

  double totalTime  = 0.0;

  datafile          = fopen("particles_pt.dat","w");
  fprintf(datafile,"%d %d %d\n", nParticles, nIters, 0);

  /* ------------------------------*/
  /*     MAIN LOOP                 */
  /* ------------------------------*/

  for(tn = 0; tn<nthreads; tn++){
      params[tn].dt = dt;
      params[tn].nstart = (float)((nParticles*tn)/nthreads);
      if(tn != nthreads-1)
        params[tn].nend = (float)((nParticles*(tn+1))/nthreads);
      else
        params[tn].nend = nParticles;
      params[tn].p = p;
      params[tn].ntotal = nParticles;
  }

  for (int iter = 1; iter <= nIters; iter++) {
    /* printf("iteration:%d\n", iter); */

    for (int i = 0;i < nParticles; ++i)
      fprintf(datafile, "%f %f %f \n", p[i].x, p[i].y, p[i].z);

    StartTimer();


    /* pthread_mutex_init(&mutexsum, NULL); */

    for(tn = 0; tn<nthreads; tn++){
      pthread_create(&threads[tn], NULL, calc_force, (void*)(params+tn));
    }

    for (tn=0;tn<nthreads;tn++) {
      pthread_join(threads[tn], &status);
    }

   /*  pthread_mutex_destroy(&mutexsum); */       
    
    /* compute interparticle forces and update vel */

    for (int i = 0 ; i < nParticles; i++) {  /* compute new position */
      p[i].x += p[i].vx*dt;
      p[i].y += p[i].vy*dt;
      p[i].z += p[i].vz*dt;
    }

    const double tElapsed = GetTimer() / 1000.0;
    if (iter > 1) {                          /* First iter is warm up */
      totalTime += tElapsed;
    }
  }

  fclose(datafile);
  double avgTime = totalTime / (double)(nIters-1);

  printf("avgTime: %f   totTime: %f     ", avgTime, totalTime);
  free(buf);
  datafile          = fopen("particles_pt.dat","r");
  datafile_serial   = fopen("particles_serial.dat","r");
  do
  {
    // Input character from both files
    char_omp = fgetc(datafile);
    char_serial = fgetc(datafile_serial);
    if (char_omp != char_serial)
        check = -1;

  } while (char_omp != EOF && char_serial != EOF);

  if (check == -1)
    printf("check failed!\n");
  else
    printf("check succeeded!\n");
  fclose(datafile);
  fclose(datafile_serial);
}

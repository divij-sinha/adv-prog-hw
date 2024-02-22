#!/bin/bash
touch test_results
date >> test_results
make
echo "serial run 1: n=3000" >> test_results
./nbody >> test_results
echo "serial run 2: n=3000" >> test_results
./nbody >> test_results
echo "serial run 3: n=3000" >> test_results
./nbody >> test_results
echo "serial run 4: n=3000" >> test_results
./nbody >> test_results
echo "serial run 5: n=3000" >> test_results
./nbody >> test_results

echo "pthreads run 1: n=3000 threads=1" >> test_results
./nbody_pt 3000 1 >> test_results
echo "pthreads run 2: n=3000 threads=1" >> test_results
./nbody_pt 3000 1 >> test_results
echo "pthreads run 3: n=3000 threads=1" >> test_results
./nbody_pt 3000 1 >> test_results
echo "pthreads run 4: n=3000 threads=1" >> test_results
./nbody_pt 3000 1 >> test_results
echo "pthreads run 5: n=3000 threads=1" >> test_results
./nbody_pt 3000 1 >> test_results

echo "pthreads run 1: n=3000 threads=2" >> test_results
./nbody_pt 3000 2 >> test_results
echo "pthreads run 2: n=3000 threads=2" >> test_results
./nbody_pt 3000 2 >> test_results
echo "pthreads run 3: n=3000 threads=2" >> test_results
./nbody_pt 3000 2 >> test_results
echo "pthreads run 4: n=3000 threads=2" >> test_results
./nbody_pt 3000 2 >> test_results
echo "pthreads run 5: n=3000 threads=2" >> test_results
./nbody_pt 3000 2 >> test_results

echo "pthreads run 1: n=3000 threads=4" >> test_results
./nbody_pt 3000 4 >> test_results
echo "pthreads run 2: n=3000 threads=4" >> test_results
./nbody_pt 3000 4 >> test_results
echo "pthreads run 3: n=3000 threads=4" >> test_results
./nbody_pt 3000 4 >> test_results
echo "pthreads run 4: n=3000 threads=4" >> test_results
./nbody_pt 3000 4 >> test_results
echo "pthreads run 5: n=3000 threads=4" >> test_results
./nbody_pt 3000 4 >> test_results

echo "pthreads run 1: n=3000 threads=8" >> test_results
./nbody_pt 3000 8 >> test_results
echo "pthreads run 2: n=3000 threads=8" >> test_results
./nbody_pt 3000 8 >> test_results
echo "pthreads run 3: n=3000 threads=8" >> test_results
./nbody_pt 3000 8 >> test_results
echo "pthreads run 4: n=3000 threads=8" >> test_results
./nbody_pt 3000 8 >> test_results
echo "pthreads run 5: n=3000 threads=8" >> test_results
./nbody_pt 3000 8 >> test_results

echo "pthreads run 1: n=3000 threads=16" >> test_results
./nbody_pt 3000 16 >> test_results
echo "pthreads run 2: n=3000 threads=16" >> test_results
./nbody_pt 3000 16 >> test_results
echo "pthreads run 3: n=3000 threads=16" >> test_results
./nbody_pt 3000 16 >> test_results
echo "pthreads run 4: n=3000 threads=16" >> test_results
./nbody_pt 3000 16 >> test_results
echo "pthreads run 5: n=3000 threads=16" >> test_results
./nbody_pt 3000 16 >> test_results

echo "pthreads run 1: n=3000 threads=32" >> test_results
./nbody_pt 3000 32 >> test_results
echo "pthreads run 2: n=3000 threads=32" >> test_results
./nbody_pt 3000 32 >> test_results
echo "pthreads run 3: n=3000 threads=32" >> test_results
./nbody_pt 3000 32 >> test_results
echo "pthreads run 4: n=3000 threads=32" >> test_results
./nbody_pt 3000 32 >> test_results
echo "pthreads run 5: n=3000 threads=32" >> test_results
./nbody_pt 3000 32 >> test_results

echo "pthreads run 1: n=3000 threads=64" >> test_results
./nbody_pt 3000 64 >> test_results
echo "pthreads run 2: n=3000 threads=64" >> test_results
./nbody_pt 3000 64 >> test_results
echo "pthreads run 3: n=3000 threads=64" >> test_results
./nbody_pt 3000 64 >> test_results
echo "pthreads run 4: n=3000 threads=64" >> test_results
./nbody_pt 3000 64 >> test_results
echo "pthreads run 5: n=3000 threads=64" >> test_results
./nbody_pt 3000 64 >> test_results

echo "pthreads run 1: n=3000 threads=128" >> test_results
./nbody_pt 3000 128 >> test_results
echo "pthreads run 2: n=3000 threads=128" >> test_results
./nbody_pt 3000 128 >> test_results
echo "pthreads run 3: n=3000 threads=128" >> test_results
./nbody_pt 3000 128 >> test_results
echo "pthreads run 4: n=3000 threads=128" >> test_results
./nbody_pt 3000 128 >> test_results
echo "pthreads run 5: n=3000 threads=128" >> test_results
./nbody_pt 3000 128 >> test_results

echo "omp run 1: n=3000 threads=1" >> test_results
./nbody_omp 3000 1 >> test_results
echo "omp run 2: n=3000 threads=1" >> test_results
./nbody_omp 3000 1 >> test_results
echo "omp run 3: n=3000 threads=1" >> test_results
./nbody_omp 3000 1 >> test_results
echo "omp run 4: n=3000 threads=1" >> test_results
./nbody_omp 3000 1 >> test_results
echo "omp run 5: n=3000 threads=1" >> test_results
./nbody_omp 3000 1 >> test_results

echo "omp run 1: n=3000 threads=2" >> test_results
./nbody_omp 3000 2 >> test_results
echo "omp run 2: n=3000 threads=2" >> test_results
./nbody_omp 3000 2 >> test_results
echo "omp run 3: n=3000 threads=2" >> test_results
./nbody_omp 3000 2 >> test_results
echo "omp run 4: n=3000 threads=2" >> test_results
./nbody_omp 3000 2 >> test_results
echo "omp run 5: n=3000 threads=2" >> test_results
./nbody_omp 3000 2 >> test_results

echo "omp run 1: n=3000 threads=4" >> test_results
./nbody_omp 3000 4 >> test_results
echo "omp run 2: n=3000 threads=4" >> test_results
./nbody_omp 3000 4 >> test_results
echo "omp run 3: n=3000 threads=4" >> test_results
./nbody_omp 3000 4 >> test_results
echo "omp run 4: n=3000 threads=4" >> test_results
./nbody_omp 3000 4 >> test_results
echo "omp run 5: n=3000 threads=4" >> test_results
./nbody_omp 3000 4 >> test_results

echo "omp run 1: n=3000 threads=8" >> test_results
./nbody_omp 3000 8 >> test_results
echo "omp run 2: n=3000 threads=8" >> test_results
./nbody_omp 3000 8 >> test_results
echo "omp run 3: n=3000 threads=8" >> test_results
./nbody_omp 3000 8 >> test_results
echo "omp run 4: n=3000 threads=8" >> test_results
./nbody_omp 3000 8 >> test_results
echo "omp run 5: n=3000 threads=8" >> test_results
./nbody_omp 3000 8 >> test_results

echo "omp run 1: n=3000 threads=16" >> test_results
./nbody_omp 3000 16 >> test_results
echo "omp run 2: n=3000 threads=16" >> test_results
./nbody_omp 3000 16 >> test_results
echo "omp run 3: n=3000 threads=16" >> test_results
./nbody_omp 3000 16 >> test_results
echo "omp run 4: n=3000 threads=16" >> test_results
./nbody_omp 3000 16 >> test_results
echo "omp run 5: n=3000 threads=16" >> test_results
./nbody_omp 3000 16 >> test_results

echo "omp run 1: n=3000 threads=32" >> test_results
./nbody_omp 3000 32 >> test_results
echo "omp run 2: n=3000 threads=32" >> test_results
./nbody_omp 3000 32 >> test_results
echo "omp run 3: n=3000 threads=32" >> test_results
./nbody_omp 3000 32 >> test_results
echo "omp run 4: n=3000 threads=32" >> test_results
./nbody_omp 3000 32 >> test_results
echo "omp run 5: n=3000 threads=32" >> test_results
./nbody_omp 3000 32 >> test_results

echo "omp run 1: n=3000 threads=64" >> test_results
./nbody_omp 3000 64 >> test_results
echo "omp run 2: n=3000 threads=64" >> test_results
./nbody_omp 3000 64 >> test_results
echo "omp run 3: n=3000 threads=64" >> test_results
./nbody_omp 3000 64 >> test_results
echo "omp run 4: n=3000 threads=64" >> test_results
./nbody_omp 3000 64 >> test_results
echo "omp run 5: n=3000 threads=64" >> test_results
./nbody_omp 3000 64 >> test_results

echo "omp run 1: n=3000 threads=128" >> test_results
./nbody_omp 3000 128 >> test_results
echo "omp run 2: n=3000 threads=128" >> test_results
./nbody_omp 3000 128 >> test_results
echo "omp run 3: n=3000 threads=128" >> test_results
./nbody_omp 3000 128 >> test_results
echo "omp run 4: n=3000 threads=128" >> test_results
./nbody_omp 3000 128 >> test_results
echo "omp run 5: n=3000 threads=128" >> test_results
./nbody_omp 3000 128 >> test_results
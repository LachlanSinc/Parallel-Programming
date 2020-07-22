#!/bin/bash
#SBATCH -J Project5
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o Project6.out
#SBATCH -e Project6.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=sinclala@oregonstate.edu
for t in 8 16 32 64 128 256 512
do
	for x in 1024 2048 4096 16384 32768 65536 131072 262144 1048576 2097152 4194304 8388608
	do
		g++ -o first first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -DLOCAL_SIZE=$t -DNMB=$x -w
		./first
	done
done

for t in 8 16 32 64 128 256 512
do
	for x in 1024 2048 4096 16384 32768 65536 131072 262144 1048576 2097152 4194304 8388608
	do
		g++ -o second second.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -DLOCAL_SIZE=$t -DNMB=$x -w
		./second
	done
done

for t in 32 64 128 256
do
	for x in 1024 2048 4096 16384 32768 65536 131072 262144 1048576 2097152 4194304 8388608
	do
		g++ -o third third.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp -DLOCAL_SIZE=$t -DNMB=$x -w
		./third
	done
done
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
	for x in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
	do
		/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DLOCAL_SIZE=$t -DNMB=$x -o first first.cpp
		./first
	done
done

for t in 8 16 32 64 128 256 512
do
	for x in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
	do
		/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DLOCAL_SIZE=$t -DNMB=$x -o second second.cpp
		./second
	done
done

for t in 8 16 32 64 128 256 512
do
	for x in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576
	do
		/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DLOCAL_SIZE=$t -DNMB=$x -o third third.cpp
		./third
	done
done
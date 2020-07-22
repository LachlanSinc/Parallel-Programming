import os

for t in [1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 33554432, 67108864, 134217728]:
	comd = "g++ Project4.cpp -o proj4 -lm -fopenmp"
	os.system(comd)
	cmd = "./proj4 " + str(t)
	os.system(cmd)

print("\n\n")

for t in [1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 33554432, 67108864, 134217728]:
		for x in [1, 2, 4, 8]:
			comd = "g++ Project4mul.cpp -o proj4mul -lm -fopenmp"
			os.system(comd)
			cmd = "./proj4mul " + str(t) + " " + str(x)
			os.system(cmd)

nvcc   -rdc=true -c -o temp.o lib.cu
nvcc -dlink -o lib.o temp.o -lcudart
rm -f libgpu.a
ar cru libgpu.a lib.o temp.o
ranlib libgpu.a
nvcc main.cu -L. -lgpu -o main -L/usr/local/cuda/lib64 -lcudart

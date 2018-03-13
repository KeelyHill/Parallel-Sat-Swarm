
CC = /usr/local/opt/llvm/bin/clang
OPENMP = -openmp
CFLAGS = -O3

LDFLAGS="-L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib" # needed for mac?

# CC = icpc
# OPENMP = -qopenmp
# CFLAGS = -O3

build:
	$(CC) $(OPENMP) -o main.out main.cpp

clean:
	rm -f main.out



CC = /usr/local/opt/llvm/bin/clang++  # macOS CC (after `brew install llvm`)

OPENMP = -fopenmp
CFLAGS = -O3

LDFLAGS="-L/usr/local/opt/llvm/lib" # needed for macOS

# CC = icpc
# OPENMP = -qopenmp
# CFLAGS = -O3

#-rpath /usr/local/opt/llvm/lib/clang/6.0.0/include/ -Wl,-rpath,/usr/local/opt/llvm/lib

build:
	$(CC) $(OPENMP) $(LDFLAGS) -o main.out main.cpp

clean:
	rm -f main.out test.out

test:
	$(CC) -o test.out test.cpp && ./test.out

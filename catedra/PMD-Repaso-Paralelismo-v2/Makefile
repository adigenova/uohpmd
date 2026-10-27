CC ?= cc
CFLAGS ?= -O2 -std=c11 -Wall -Wextra -Wpedantic
.PHONY: all pdf handout examples benchmark check clean
all: pdf examples
pdf:
	latexmk -pdf -interaction=nonstopmode -halt-on-error repaso-v2.tex
handout:
	latexmk -pdf -interaction=nonstopmode -halt-on-error repaso-handout.tex
examples: build/serial build/pthreads build/openmp
build:
	mkdir -p build
build/serial: examples/serial.c examples/common.h | build
	$(CC) $(CFLAGS) $< -o $@
build/pthreads: examples/pthreads.c examples/common.h | build
	$(CC) $(CFLAGS) -pthread $< -o $@
build/openmp: examples/openmp.c examples/common.h | build
	$(CC) $(CFLAGS) -fopenmp $< -o $@
benchmark: examples
	python3 benchmark.py
check: examples
	python3 check.py
clean:
	latexmk -c repaso-v2.tex

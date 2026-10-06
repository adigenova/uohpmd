SHELL := /bin/sh
LATEXMK ?= latexmk
MPICC ?= mpicc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c11
DECK := PMD-W4-C01-MPI-II-Hibrido-beamer
MPI_NAMES := mpi_suma mpi_pi mpi_matrices mpi_tipo
OMP_NAMES := hybrid_hola hybrid_producto
PTHREAD_NAMES := hybrid_pthreads
BINS := $(addprefix build/examples/,$(MPI_NAMES) $(OMP_NAMES) $(PTHREAD_NAMES))

.PHONY: all pdf handout examples check zip clean
all: pdf handout
pdf:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(DECK).tex
handout:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(DECK)-handout.tex
examples: $(BINS)
$(addprefix build/examples/,$(MPI_NAMES)): build/examples/%: examples/%.c examples/common.h
	mkdir -p build/examples
	$(MPICC) $(CFLAGS) $< -o $@ -lm
$(addprefix build/examples/,$(OMP_NAMES)): build/examples/%: examples/%.c examples/common.h
	mkdir -p build/examples
	$(MPICC) $(CFLAGS) -fopenmp $< -o $@
$(addprefix build/examples/,$(PTHREAD_NAMES)): build/examples/%: examples/%.c examples/common.h
	mkdir -p build/examples
	$(MPICC) $(CFLAGS) -pthread $< -o $@
check: examples
	python3 scripts/check_examples.py
zip: pdf handout
	python3 scripts/package.py
clean:
	$(LATEXMK) -c $(DECK).tex
	$(LATEXMK) -c $(DECK)-handout.tex

SHELL := /bin/sh
LATEXMK ?= latexmk
CC = gcc
MPICC ?= mpicc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c11
DECK := PMD-W4-C02-OpenMP-MPI-beamer
OMP_SOURCES := $(wildcard examples/[0-9]*.c)
MPI_SOURCES := $(wildcard examples/mpi_*.c)
OMP_BINS := $(patsubst examples/%.c,build/examples/%,$(OMP_SOURCES))
MPI_BINS := $(patsubst examples/%.c,build/examples/%,$(MPI_SOURCES))

.PHONY: all pdf examples mpi check check-mpi zip clean
all: pdf examples
pdf:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(DECK).tex
examples: $(OMP_BINS)
mpi: $(MPI_BINS)
build/examples/mpi_%: examples/mpi_%.c
	mkdir -p build/examples
	$(MPICC) $(CFLAGS) $< -o $@
build/examples/%: examples/%.c
	mkdir -p build/examples
	$(CC) $(CFLAGS) -fopenmp $< -o $@ -lm
check: examples
	python3 scripts/check_examples.py
check-mpi: mpi
	python3 scripts/check_mpi.py
zip: pdf
	python3 scripts/package.py
clean:
	$(LATEXMK) -c $(DECK).tex

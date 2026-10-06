SHELL := /bin/sh
LATEXMK ?= latexmk
CC = gcc
CFLAGS ?= -O2 -Wall -Wextra -Wpedantic -std=c11
PROGRAMS := hello for_replicado for_repartido sections single nested critical
BINARIES := $(addprefix build/examples/,$(PROGRAMS))

.PHONY: all pdf examples clean
all: pdf

pdf:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error PMD-W3-C01-OpenMP-beamer.tex

examples: $(BINARIES)

build/examples/%: examples/%.c examples/work.h
	mkdir -p build/examples
	$(CC) $(CFLAGS) -fopenmp $< -o $@

clean:
	$(LATEXMK) -c PMD-W3-C01-OpenMP-beamer.tex

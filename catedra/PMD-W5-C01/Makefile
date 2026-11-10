SHELL := /bin/sh
LATEXMK ?= latexmk
DECK := PMD-W6-C01-Hadoop-beamer
.PHONY: all pdf handout check zip clean
all: pdf handout
pdf:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(DECK).tex
handout:
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(DECK)-handout.tex
check:
	python3 scripts/check_examples.py
	bash -n examples/run_hadoop.sh
zip: pdf handout
	python3 scripts/package.py
clean:
	$(LATEXMK) -c $(DECK).tex
	$(LATEXMK) -c $(DECK)-handout.tex

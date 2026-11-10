MAIN = PMD-W6-C01-Nextflow-beamer
.PHONY: all clean
all:
	latexmk -pdf -interaction=nonstopmode -halt-on-error $(MAIN).tex
	latexmk -pdf -interaction=nonstopmode -halt-on-error $(MAIN)-handout.tex
clean:
	latexmk -c $(MAIN).tex $(MAIN)-handout.tex

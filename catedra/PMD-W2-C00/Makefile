LATEXMK ?= latexmk
SOURCE := Linux_sed_awk.tex
PDF := $(SOURCE:.tex=.pdf)
STYLE_DIR := styles

# Let TeX discover packages below styles/ while retaining its standard search
# path. The double slash makes the lookup recursive.
export TEXINPUTS := $(CURDIR)/$(STYLE_DIR)//:$(TEXINPUTS)

.PHONY: all sed-awk template clean

all: $(PDF)

JACQUENETTA := $(wildcard $(STYLE_DIR)/jacquenetta/*.sty) \
	$(wildcard $(STYLE_DIR)/uoh/*.sty)

$(PDF): $(SOURCE) $(JACQUENETTA)
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error $(SOURCE)

sed-awk: $(PDF)

template: uoh-template-example.pdf

uoh-template-example.pdf: uoh-template-example.tex $(JACQUENETTA)
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error uoh-template-example.tex

clean:
	$(LATEXMK) -c $(SOURCE)
	$(LATEXMK) -c uoh-template-example.tex

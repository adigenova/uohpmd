# Estilos LaTeX

Los estilos están separados por responsabilidad:

- `jacquenetta/`: tema Beamer de terceros, conservado como una unidad.
- `uoh/`: personalizaciones institucionales y componentes del curso.

Compile los documentos desde la raíz mediante `make`, `make sed-awk` o
`make template`. El `Makefile` configura `TEXINPUTS` para que TeX encuentre
ambos grupos sin añadir rutas a cada archivo `.tex`.

"""Crea un ZIP portable para Overleaf o compilacion local."""
from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED

root = Path(__file__).resolve().parents[1]
stem = "PMD-W4-C02-OpenMP-MPI-beamer"
paths = [root / f"{stem}.tex", root / f"{stem}.pdf", root / "Makefile", root / "README.md"]
for folder in ("assets", "examples", "scripts"):
    paths.extend(p for p in (root / folder).rglob("*") if p.is_file() and "__pycache__" not in p.parts)
with ZipFile(root / f"{stem}.zip", "w", ZIP_DEFLATED) as archive:
    for path in sorted(paths):
        archive.write(path, path.relative_to(root))
print(f"Creado {stem}.zip ({len(paths)} archivos)")

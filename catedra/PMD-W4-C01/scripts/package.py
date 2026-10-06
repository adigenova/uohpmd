"""Package editable sources, assets, examples and the compiled deck."""
from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED

root = Path(__file__).resolve().parents[1]
name = "PMD-W4-C01-MPI-II-Hibrido-beamer"
files = [root / (name + ext) for ext in (".tex", ".pdf")]
files += [root / (name + "-handout" + ext) for ext in (".tex", ".pdf")]
files += [root / "README.md", root / "Makefile"]
for folder in ("assets", "examples", "scripts"):
    files.extend(p for p in (root / folder).rglob("*") if p.is_file())
with ZipFile(root / (name + ".zip"), "w", ZIP_DEFLATED) as archive:
    for path in sorted(files):
        archive.write(path, Path(name) / path.relative_to(root))
print(root / (name + ".zip"))

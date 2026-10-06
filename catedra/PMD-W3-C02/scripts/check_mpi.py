"""Comprobaciones opcionales: requieren una implementacion MPI y mpirun."""
import os
from pathlib import Path
import re
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[1]
launcher = shlex.split(os.environ.get("MPIEXEC", "mpirun"))
flags = shlex.split(os.environ.get("MPIEXEC_FLAGS", ""))


def run(program, processes, expected_code=0):
    result = subprocess.run(
        [*launcher, *flags, "-np", str(processes), str(ROOT / "build/examples" / program)],
        text=True, capture_output=True, timeout=30,
    )
    if expected_code == 0:
        assert result.returncode == 0, (program, result.stdout, result.stderr)
    else:
        assert result.returncode != 0 and "exactamente 2 procesos" in result.stderr
    return result.stdout


for size in (1, 2, 4):
    output = run("mpi_01_hola", size)
    rows = re.findall(r"Hola desde rank (\d+) de (\d+) en", output)
    assert len(rows) == size and {int(r[0]) for r in rows} == set(range(size))
    assert all(int(r[1]) == size for r in rows)
    assert "Suma global = 55" in run("mpi_03_reduce", size)
assert "Rank 1 recibio 42" in run("mpi_02_mensaje", 2)
run("mpi_02_mensaje", 1, expected_code=2)
print("OK: 8 ejecuciones MPI; saludos, mensaje, reduccion y validacion del numero de procesos.")

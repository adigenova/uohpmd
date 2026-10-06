"""Validate example results using a real MPI runtime, after `make examples`."""
import math
import os
from pathlib import Path
import re
import shlex
import subprocess

ROOT = Path(__file__).resolve().parents[1]
launcher = shlex.split(os.environ.get("MPIEXEC", "mpirun"))
flags = shlex.split(os.environ.get("MPIEXEC_FLAGS", ""))


def run(name, processes, threads=2, success=True):
    env = dict(os.environ, OMP_NUM_THREADS=str(threads), OMP_DYNAMIC="FALSE")
    command = launcher + flags + ["-np", str(processes), str(ROOT / "build/examples" / name)]
    result = subprocess.run(command, env=env, text=True, capture_output=True, timeout=60)
    if success and result.returncode:
        raise RuntimeError(f"{command}:\n{result.stdout}\n{result.stderr}")
    if not success and not result.returncode:
        raise AssertionError(f"{name} should reject this process count")
    return result.stdout


for count in (1, 4):
    assert "Suma global = 499500" in run("mpi_suma", count)
run("mpi_suma", 3, success=False)

for count in (1, 3, 4):
    output = run("mpi_pi", count)
    value = float(re.search(r"pi = ([0-9.]+)", output).group(1))
    assert abs(value - math.pi) < 4e-9, output

for count in (1, 4):
    values = [int(v) for v in run("mpi_matrices", count).split()]
    assert len(values) == 64 and set(values) == {16}, values
run("mpi_matrices", 3, success=False)

assert "Punto recibido: (45, 36, 0)" in run("mpi_tipo", 2)
run("mpi_tipo", 1, success=False)

output = run("hybrid_hola", 4, threads=3)
observed = re.findall(r"MPI rank (\d+): OpenMP hilo (\d+) de (\d+)", output)
expected = {(str(p), str(t), "3") for p in range(4) for t in range(3)}
assert len(observed) == 12 and set(observed) == expected, output
assert len(re.findall(r"Hola desde proceso \d+ de 4", output)) == 4, output

for count in (1, 3, 4):
    for threads in (1, 4):
        assert "Producto global = 2000.0" in run("hybrid_producto", count, threads)
    assert "Suma global = 4500000" in run("hybrid_pthreads", count)

print("OK: 7 examples; numeric results, threads and invalid process counts.")

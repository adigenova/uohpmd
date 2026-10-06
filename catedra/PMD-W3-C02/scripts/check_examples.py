"""Verifica resultados y casos limite, sin exigir tiempos ni orden de stdout."""
from collections import Counter
import os
from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[1]
runs = 0


def run(program, threads, *args, schedule="static", expected_code=0):
    global runs
    env = os.environ.copy()
    env.update(OMP_NUM_THREADS=str(threads), OMP_DYNAMIC="FALSE",
               OMP_THREAD_LIMIT="64", OMP_MAX_ACTIVE_LEVELS="2",
               OMP_SCHEDULE=schedule)
    result = subprocess.run(
        [str(ROOT / "build/examples" / program), *map(str, args)],
        env=env, text=True, capture_output=True, timeout=30,
    )
    assert result.returncode == expected_code, (program, args, result.stdout, result.stderr)
    runs += 1
    return result.stdout


for threads in (1, 2, 4):
    out = run("01_hola", threads)
    rows = re.findall(r"Hola desde hilo (\d+) de (\d+)", out)
    assert len(rows) == threads and {int(r[0]) for r in rows} == set(range(threads))
    assert all(int(r[1]) == threads for r in rows)
    assert out.splitlines()[-1] == "Fin de la region paralela"
    assert list(map(int, run("02_vectores", threads).split())) == [3*i for i in range(12)]
    assert "Suma = 55" in run("03_suma", threads)
    assert "Suma=26 maximo=9" in run("05_secciones", threads)
    for n in (1, 3, 10, 10003):
        out = run("04_promedio", threads, n)
        assert float(re.search(r"promedio=([\d.]+)", out)[1]) == (n-1)/2
    for value in ("", "a", "banana bandana", "abcde", "áéñ🙂", "a"*10001):
        out = run("06_histograma", threads, value)
        bins = {int(b, 16): int(c) for b, c in re.findall(r"byte 0x([0-9A-F]+): (\d+)", out)}
        assert bins == dict(Counter(value.encode("utf-8")))
        assert f"Total={len(value.encode('utf-8'))} bytes" in out
    for policy in ("static", "static,3", "dynamic,64", "guided,64"):
        assert "primos=17984 referencia=17984" in run("07_primos_schedule", threads, schedule=policy)
    assert "primos=25 referencia=25" in run("07_primos_schedule", threads, 100)
    assert "primos=1 referencia=1" in run("07_primos_schedule", threads, 2)
    assert "C[0][0]=0 C[47][47]=188 verificacion=OK" in run("08_matriz", threads)
    for n in (1, 2048, 2049, 10000):
        for mode in ("aleatorio", "ordenado", "inverso", "iguales"):
            assert "ordenado=OK" in run("09_quicksort_tasks", threads, n, mode)
    assert "ordenado=OK" in run("09_quicksort_tasks", threads, 100000)

out = run("10_anidado", 2)
rows = re.findall(r"exterior=(\d+) interior=(\d+) equipo=(\d+) nivel_activo=(\d+)", out)
assert len(rows) == 4 and {(int(a), int(b)) for a, b, _, _ in rows} == {(0,0), (0,1), (1,0), (1,1)}
assert all(c == "2" and d == "2" for _, _, c, d in rows)
for program in ("04_promedio", "07_primos_schedule", "09_quicksort_tasks"):
    for invalid in ("0", "-1", "abc", "999999999999999999999999"):
        run(program, 2, invalid, expected_code=2)
print(f"OK: {runs} ejecuciones; 10 programas OpenMP, equipos de 1, 2 y 4 hilos.")
print("Comprobados: reducciones, limites, UTF-8 por bytes, politicas, matrices y ordenacion.")

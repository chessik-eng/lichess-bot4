#!/usr/bin/env python3
"""Protocol and rules regression tests for AA Engine."""
import re
import subprocess

def run(commands, timeout=45):
    result = subprocess.run(
        ["./aa-engine"], input="\n".join(commands + ["quit"]) + "\n",
        text=True, capture_output=True, timeout=timeout, check=True
    )
    return result.stdout.splitlines()

def check(condition, message):
    if not condition:
        raise AssertionError(message)

lines = run(["uci", "isready", "position startpos", "go depth 2"])
check("uciok" in lines, f"UCI handshake missing: {lines!r}")
check("readyok" in lines, f"Ready handshake missing: {lines!r}")
best = [x for x in lines if x.startswith("bestmove ")]
check(len(best) == 1 and re.fullmatch(r"bestmove [a-h][1-8][a-h][1-8][qrbn]?", best[0]), f"Bad bestmove: {best!r}")

lines = run(["position startpos", "perft 1", "perft 2", "perft 3", "perft 4"])
for depth, count in [(1, 20), (2, 400), (3, 8902), (4, 197281)]:
    check(f"perft {depth} nodes {count}" in lines, f"Perft depth {depth} failed: {lines!r}")

# A position with only kings must have exactly five king moves.
lines = run(["position fen 8/8/8/8/8/8/8/K6k w - - 0 1", "perft 1"])
check("perft 1 nodes 3" in lines, f"King movement failed: {lines!r}")

lines = run(["position startpos", "go movetime 150"])
check(any(x.startswith("bestmove ") for x in lines), "Timed search produced no move")
print("AA Engine UCI and perft regression tests passed")

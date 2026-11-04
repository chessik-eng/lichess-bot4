# Alokhin-Alekhine Engine (AA Engine) — v0.2

Experimental **from-scratch** C++17 UCI chess engine. This is an early prototype, **not** a Stockfish 19 competitor.

## Build

```sh
g++ -std=c++17 -O2 main.cpp -o aa-engine
```

## UCI smoke test

```sh
printf 'uci\nisready\nposition startpos\ngo depth 2\nquit\n' | ./aa-engine
```

## Implemented

- Board representation, FEN parsing, move generation with king-safety filtering
- Castling, en passant and promotions (initial implementations, not yet exhaustively tested)
- Material and basic positional evaluation; alpha-beta negamax\n- Quiescence search for tactical captures and check evasions\n- Capture/promotion move ordering
- UCI commands: `uci`, `isready`, `ucinewgame`, `position`, `go depth`, `quit`
- GitHub Actions compile and smoke test

## Limitations

- No repetition/50-move draw handling, transposition table, quiescence search, or clock management
- `go` only supports fixed depth up to 5; no `wtime`/`btime` or asynchronous `stop`
- **Not ready for unattended Lichess games**; this repository does not yet run a bot

## Roadmap

Perft validation, time controls, move ordering, iterative deepening, quiescence, transposition tables, stronger evaluation and engine-vs-engine testing.

## Version 0.2 status

This version improves tactical search compared with v0.1, but strength gains have **not** been measured. The engine is **not proven stronger than Stockfish 19**. UCI clock handling, repetition detection, perft and match testing remain required before Lichess deployment.

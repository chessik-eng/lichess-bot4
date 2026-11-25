# Alokhin-Alekhine Engine (AA Engine)

Experimental from-scratch C++17 UCI chess engine. It is an educational prototype and is **not currently proven stronger than Stockfish 19**. No verified Lichess Elo rating is available.

## Build

```sh
g++ -std=c++17 -O2 -Wall -Wextra main.cpp -o aa-engine
```

## Current search features

- Alpha-beta negamax with iterative deepening
- Quiescence search
- Transposition table with stored best moves
- TT / capture / killer / history move ordering
- Principal-variation search (PVS)
- Late-move reductions (LMR), with full-depth verification
- Null-move pruning, guarded against check and pawn-only positions
- Aspiration windows with full-window re-search
- UCI clock-based search and fixed-depth search
- Basic material, center, and pawn-advance evaluation

## Validation

GitHub Actions compiles the engine, runs UCI smoke tests, checks start-position perft through depth 4, checks clock-based search, and runs the rules regression suite.

Passing these checks validates compilation and selected correctness properties; it does **not** establish playing strength or an Elo rating.

## Important limitations

- Evaluation is still very basic; there is no trained NNUE, tapered piece-square evaluation, robust pawn-structure model, or king-safety model.
- No verified engine-vs-Stockfish 19 match results exist yet.
- Threefold repetition and complete draw adjudication are not yet implemented.
- Search-strength improvements must be measured with paired games at fixed hardware, time control, and settings.

## Goal

The long-term experimental target is to build a competitive original chess engine and test it against Stockfish 19. A 5000 Lichess Elo target is aspirational, not a demonstrated or guaranteed result.

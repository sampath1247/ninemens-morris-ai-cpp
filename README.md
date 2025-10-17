# ninemens-morris-ai-cpp
C++ AI agent for Nine Men’s Morris (Variant-D) with Minimax, Alpha-Beta, improved heuristics, and compact 64-bit board representation.
# Nine Men's Morris AI (C++): Minimax, Alpha-Beta, and Heuristics

AI agent for Nine Men's Morris (Variant-D) built in C++ for the Concepts of Artificial Intelligence course. Implements Minimax and Alpha-Beta Pruning with improved, phase-aware heuristics and a compact 64-bit board representation. Supports opening, midgame, and endgame with legal move generation (placement, sliding, hopping, removals) and symmetric Black/White play.

## Key Features
- Compact board state: two 32-bit integers encode 23 positions at 2 bits each ('W', 'B', 'x') for fast copying and passing.
- Search algorithms:
  - Minimax (Opening, Midgame/Endgame)
  - Alpha-Beta Pruning (Opening, Game)
  - Color-swap reuse for Black (Opening/Game)
  - Optional: transposition tables, killer moves, move ordering, iterative deepening
- Move generation: GenerateAdd (placement), GenerateMove (sliding), GenerateHopping, GenerateRemove with mill detection.
- Heuristics (improved):
  - Mill formation and threats (+15 for White mills, -10 for Black threats)
  - Mobility differences and blocking (+5 for blocking mills)
  - Phase-aware weighting (e.g., higher mill emphasis in endgame)
- Outputs: best move, Minimax estimate, and number of positions evaluated.

## Repo Structure
```
.
├─ include/
│  └─ classboard.h
├─ src/
│  ├─ classboard.cpp
│  ├─ MiniMaxOpening.cpp
│  ├─ MiniMaxGame.cpp
│  ├─ MiniMaxOpeningBlack.cpp
│  ├─ MiniMaxGameBlack.cpp
│  ├─ ABOpening.cpp
│  ├─ ABGame.cpp
│  ├─ MiniMaxOpeningImproved.cpp
│  └─ MiniMaxGameImproved.cpp
├─ tests/
│  └─ unittesting.cpp
├─ data/
│  └─ sample_board.txt
├─ docs/
│  ├─ Report.docx (or Report.pdf)
│  └─ demo.gif (optional)
├─ CMakeLists.txt
├─ .github/workflows/ci.yml
├─ .gitignore
├─ LICENSE
└─ README.md
```

## Build

Prerequisites:
- C++11-compatible compiler (GCC/Clang/MinGW)
- CMake 3.18+ (recommended)

Build and test:
```bash
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j
ctest -j 2 --output-on-failure
```

Produced executables are in `build/bin/`:
- MiniMaxOpening
- MiniMaxGame
- MiniMaxOpeningBlack
- MiniMaxGameBlack
- ABOpening
- ABGame
- MiniMaxOpeningImproved
- MiniMaxGameImproved

## Input/Output Format

Board file format:
- A single line string of 23 characters using:
  - 'W' for White, 'B' for Black, 'x' for empty
- Example (see [data/sample_board.txt](data/sample_board.txt)).

CLI usage examples:
```bash
# Minimax - Opening
./build/bin/MiniMaxOpening data/sample_board.txt 3 out_opening.txt

# Minimax - Mid/Endgame
./build/bin/MiniMaxGame data/sample_board.txt 4 out_game.txt

# Black versions (internally swap colors to reuse White logic)
./build/bin/MiniMaxOpeningBlack data/sample_board.txt 3 out_opening_black.txt
./build/bin/MiniMaxGameBlack data/sample_board.txt 4 out_game_black.txt

# Alpha-Beta (Opening and Game)
./build/bin/ABOpening data/sample_board.txt 3 out_ab_opening.txt
./build/bin/ABGame data/sample_board.txt 4 out_ab_game.txt

# Improved heuristics
./build/bin/MiniMaxOpeningImproved data/sample_board.txt 3 out_opening_improved.txt
./build/bin/MiniMaxGameImproved data/sample_board.txt 4 out_game_improved.txt
```

Program output:
- Writes best board configuration to the output file
- Prints positions evaluated and static estimate to stdout, e.g.:
```
Board Position: WBx... (23 chars)
Positions evaluated by static estimation: 15234
MINIMAX estimate: 3
```

## Algorithms and Heuristics

- Minimax: searches game tree with depth limit, separate executables for opening and game phases.
- Alpha-Beta: prunes branches without affecting exact Minimax estimate.
- Move generation:
  - Opening: `GenerateAdd`
  - Midgame: `GenerateMove` (sliding)
  - Endgame (<=3 pieces): `GenerateHopping`
  - Removals on mills: `GenerateRemove`
- Improved static evaluation:
  - Piece count difference baseline
  - Mills/triples and imminent threats
  - Mobility and blocking opportunities
  - Phase-aware weighting: increased mill weight in endgame

## Board Representation

- Encoded in 64 bits using two 32-bit integers:
  - Each of the 23 positions uses 2 bits for 'W'/'B'/'x'
- Fast encode/decode operations and string conversions
- Verified by unit tests for bitwise integrity and accessors

## Testing

Run unit tests:
```bash
ctest -j 2
```

Unit tests (tests/unittesting.cpp):
- Board encoding/decoding integrity
- String conversions
- Position setters/getters
- Loading from file

## Performance

- Alpha-Beta with move ordering (mills first), killer moves, and transposition caching reduces nodes evaluated while matching Minimax estimates.
- Iterative deepening optional for better time management.

## Reproducibility

- Compiler flags: `-std=c++11 -O2`
- CMake config checked into repo
- Sample inputs and expected outputs included in docs

## How to Cite
If you use this work, please cite:
```
@software{sampath_ninemensmorris_ai,
  title = {Nine Men's Morris AI (C++)},
  author = {Sampath},
  year = {2025},
  url = {https://github.com/sampath1247/ninemens-morris-ai-cpp}
}
```

## License
MIT — see [LICENSE](LICENSE).

## Acknowledgments
Course: Concepts of Artificial Intelligence. Classic references on Minimax, Alpha-Beta pruning, and Nine Men's Morris.

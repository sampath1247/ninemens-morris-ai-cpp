# Developer Notes

- File names match course submission:
  - MiniMaxOpening.cpp, MiniMaxGame.cpp, MiniMaxOpeningBlack.cpp, MiniMaxGameBlack.cpp
  - ABOpening.cpp, ABGame.cpp
  - MiniMaxOpeningImproved.cpp, MiniMaxGameImproved.cpp
  - classboard.h / classboard.cpp
  - tests/unittesting.cpp

- Build: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j`

- Add more shared code to `morris` library if needed (e.g., neighbor tables, mill checks, move generators).

- If you used transposition tables and killer moves:
  - Prefer standard containers in headers (unordered_map, vector).
  - Keep deterministic seeds.

- For Windows (MinGW):
  - Install CMake and MinGW-w64
  - Use: `cmake -G "MinGW Makefiles" -S . -B build && cmake --build build -j`

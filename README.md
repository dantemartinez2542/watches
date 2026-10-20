# Watch collection

Build a Debug version with debug symbols:

```sh
cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build/debug
```

Build an optimized Release version:

```sh
cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
cmake --build build/release
```

Debug is the default for Makefiles/Ninja when no build type is specified.
CMake provides the compiler flags for each configuration; Release also defines
`NDEBUG`, disabling standard `assert` checks. Separate build directories let
both versions coexist.

For a multi-configuration generator such as Xcode or Ninja Multi-Config, select
the configuration with `cmake --build build --config Debug` or `--config Release`
instead of setting `CMAKE_BUILD_TYPE`. These generators typically place the
executable in a configuration subdirectory such as `build/Debug/watches`.

Pipe the JSON watch data into either executable:

```sh
python3 dataStream.py | ./build/debug/watches
python3 dataStream.py | ./build/release/watches
```

`dataStream.py` reads `watches.json` beside the script and writes one watch per
line, in this order: quoted name, quoted brand, price, year, quoted movement.
For example:

```text
"PRX Powermatic 80 T1374071135100" "Tissot" 725.0 2024 "Powermatic 80"
```

`convertInput(std::cin)` reads the pipe until EOF and returns watches keyed by
name. Duplicate names keep the last record, and the collection appears sorted
by name. Invalid records report their line number and exit with an error.

After loading, the menu reads commands from `/dev/tty` on macOS/Linux: enter a
watch number, `draw`, or `quit`. When output is redirected or no controlling
terminal is available, the program prints the loaded collection and exits:

```sh
python3 dataStream.py | ./build/release/watches > collection.txt
```

Running either executable directly opens an empty collection.

# Graph

Graph class with several graph algorithms including depth-first search,
breadth-first search, dijkstra's shortest path, minimum spanning tree


## Included Files

- `graph.h, graph.cpp`: Graph class

- `graphtest.cpp`: Test functions

- `main.cpp`: A generic main file to call testAll() to run all tests

- `runit.sh`: Unix bash script file to compile and run

- `.clang-tidy`: Specify the options for clang-tidy program, so we do
  not have to enter them on the command line each time.
  Usage: `clang-tidy *.cpp`

- `.gitattributes`: Options for git. Making sure that simplecompile.sh
  always has the correct line endings when moving between Windows and
  unix systems

- `.gitignore`: Files that should not be checked into git. Mostly ide
  files and executables.


## Compile and Run
```
./runit.sh
```


## Style check
Based on options defined in `.clang-tidy`
```
clang-tidy *.cpp
```

#### An implementation of Conway's Game of Life, a cellular automaton, zero-player game that works on a few simple rules. 
#### To know more: https://en.wikipedia.org/wiki/Conway's_Game_of_Life

___
### Key Features:
  1. Simple, light-weight, **finite** grid based zero-player game, where you can provide an initial configuration.
  2. Displays information about the "alive" cells as well as "generation" of the cells.
  3. Cells are coloured from a warmer(Red) to cooler(Dark Blue) colour based on the generation of cells.
  4. Manually toggle a cell's state, or randomly populate the grid - The project comes with a sample implementation of the Gosper Glider Gun.


___
### Compiling/Running:
#### You'll need [raylib](https://github.com/raysan5/raylib/releases) to compile/run the program
#### You can also use [w64devkit](https://github.com/skeeto/w64devkit)

#### Compile:
``` g++ -o game_of_life.exe main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm -Wall ```

#### Run:
``` ./game_of_life.exe ```

___
### Important commands:
  1. Press **P**, to **Play/Pause Simulation**
  2. Press **G**, to set **Gosper Glider Gun**
  3. Press **C**, to **Clear Grid**
  4. Press **R**, to **Randomly Populate Grid**
  5. To **Manually toggle cell state**, hover a cell and press **Left Mouse Button**

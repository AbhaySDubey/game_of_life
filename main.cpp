#include <iostream>
#include <vector>
#include <raylib.h>
#include <string>
#include <sstream>
#include <cstring>
#include <utility>
#include <cstdlib>
#include <ctime>

#define screen_width 1400
#define screen_height 850

#define grid_width 800
#define grid_height 800

// #define cell_side 30
#define cell_margin 0.5
#define font_size 10


struct cell {
    Color color;
    Vector2 pos;
    bool alive;
    cell() : color(BLACK), alive(false) {};
    cell(Color c) : color(c), alive(false) {};
};


class Grid {
    private:
        // need to define a function that allows a user to change the value of cell side

        // dimension of the 5 x 5 cells
        float atomic_cell_side = 16;
        int num_atomicCells_1d = (grid_width / atomic_cell_side);
        std::vector<std::vector<cell*>> grid;

        std::vector<std::vector<int>> cellAges;
        int generation = 0;
        int totalLiveCells = 0;
        
        bool runSimulation = false;
        int frameCounter = 0;
        int updateInterval = 10;

        Color default_color = WHITE;

        // colors to indicate the generation of the cells
        Color cell_colors[20] = {
            {255, 0, 0, 255},   // Red
            {255, 69, 0, 255},  // Red-Orange
            {255, 105, 0, 255}, // Bright Orange
            {255, 140, 0, 255}, // Orange
            {255, 175, 0, 255}, // Gold
            {255, 210, 0, 255}, // Yellow-Orange
            {255, 245, 0, 255}, // Yellow
            {223, 255, 0, 255}, // Light Yellow-Green
            {191, 255, 0, 255}, // Yellow-Green
            {159, 255, 0, 255}, // Green-Yellow
            {127, 255, 0, 255}, // Light Green
            {95, 255, 0, 255},  // Green
            {63, 255, 127, 255}, // Aqua Green
            {0, 255, 255, 255}, // Cyan
            {0, 223, 255, 255}, // Sky Blue
            {0, 191, 255, 255}, // Light Blue
            {0, 159, 255, 255}, // Blue
            {0, 127, 255, 255}, // Dark Blue
            {0, 95, 255, 255},  // Deeper Blue
            {0, 58, 190, 255}, // Dark Blue (towards black)
        };

    public:
        Grid() {
            Vector2 pos = {0, 0};
            grid.resize(num_atomicCells_1d, std::vector<cell*>(num_atomicCells_1d, NULL));
            for (int i = 0; i < num_atomicCells_1d; i++) {
                pos.x = 0;
                for (int j = 0; j < num_atomicCells_1d; j++) {
                    grid[i][j] = new cell();
                    grid[i][j]->pos = pos;
                    pos.x += atomic_cell_side;
                }
                pos.y += atomic_cell_side;
            }
            cellAges.resize(grid_width, std::vector<int>(grid_height, 0));
        }

        void clear_grid() {
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[0].size(); j++) {
                    grid[i][j]->alive = false;
                    std::vector<std::vector<int>> cell_ages(grid_width, std::vector<int>(grid_height, 0));
                    cellAges = cell_ages;
                }
            }
        }

        void run(int cell_side_choice) {
            draw(cell_side_choice);
            if (IsKeyPressed(KEY_C)) {
                clear();
            }
            if (!runSimulation && IsKeyPressed(KEY_G)) {
                setGosperGliderGun(5, 5);
            }

            if (!runSimulation && IsKeyPressed(KEY_R)) {
                generation = 0;
                totalLiveCells = 0;
                randomly_populate();
            }
            if (IsKeyPressed(KEY_C)) {
                clear_grid();
                runSimulation = false;
                generation = 0;
                totalLiveCells = 0;
            }
            if (IsKeyPressed(KEY_P)) {
                runSimulation = !runSimulation;
            }
            if (runSimulation) {
                if (frameCounter % updateInterval == 0) {
                    update();
                    frameCounter = 0;
                }
                frameCounter++;
            }
        }


        void update() {
            std::vector<std::vector<cell*>> newGrid(grid.size(), std::vector<cell*>(grid[0].size(), nullptr));

            // Initialize newGrid with copies of cells
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    newGrid[i][j] = new cell(*grid[i][j]); // Assuming 'cell' has a copy constructor
                }
            }

            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    int liveNeighbors = 0;

                    // Check all the neighbors of grid[i][j]
                    for (int x = -1; x <= 1; x++) {
                        for (int y = -1; y <= 1; y++) {
                            if (x == 0 && y == 0) continue; // Skip the cell itself
                            int ni = i + x;
                            int nj = j + y;
                            if (ni >= 0 && nj >= 0 && ni < grid.size() && nj < grid[i].size() && grid[ni][nj]->alive) {
                                liveNeighbors++;
                            }
                        }
                    }

                    // Apply the Game of Life rules
                    if (grid[i][j]->alive && (liveNeighbors < 2 || liveNeighbors > 3)) {
                        newGrid[i][j]->alive = false;
                    } else if (!grid[i][j]->alive && liveNeighbors == 3) {
                        newGrid[i][j]->alive = true;
                    }
                }
            }

            // Reset totalLiveCells and update cellAges based on newGrid
            totalLiveCells = 0;
            for (int i = 0; i < newGrid.size(); i++) {
                for (int j = 0; j < newGrid[i].size(); j++) {
                    if (newGrid[i][j]->alive) {
                        totalLiveCells++;
                        cellAges[i][j] = (grid[i][j]->alive) ? cellAges[i][j] + 1 : 1;
                    } else {
                        cellAges[i][j] = 0;
                    }
                }
            }

            // Clean up old grid if necessary to avoid memory leaks
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[i].size(); j++) {
                    delete grid[i][j]; // Assuming dynamic allocation of cells
                }
            }

            generation++;
            grid = newGrid;
        }

        void randomly_populate() {
            std::srand(std::time(0)); // use current time as seed for random generator
            for (int r = 0; r < grid.size(); r++) {
                for (int c = 0; c < grid[0].size(); c++) {
                    grid[r][c]->alive = false;
                    cellAges[r][c] = 0;
                    grid[r][c]->alive = std::rand() % 2; // generate random number (0 or 1)
                }
            }
        }

        void clear() {
            for (int i = 0; i < grid.size(); i++) {
                for (int j = 0; j < grid[0].size(); j++) {
                    grid[i][j]->alive = false;
                }
            }
        }

        void fill_cells(int cell_side_choice, Vector2 mouse_pos) {
            int col, row;
            if (cell_side_choice == 1) {
                col = mouse_pos.x / atomic_cell_side;
                row = mouse_pos.y / atomic_cell_side;
                if (col < num_atomicCells_1d && row < num_atomicCells_1d) {
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        grid[row][col]->alive = !grid[row][col]->alive;
                        grid[row][col]->color = (grid[row][col]->alive == true ? WHITE : BLACK);
                    }
                }
            }
        }


        void draw(int cell_side_choice) {
            for (int i = 0; i < num_atomicCells_1d; i++) {
                for (int j = 0; j < num_atomicCells_1d; j++) {
                    int age = (cellAges[i][j] / 10);
                    if (age >= 20) age = 19; // Use the darkest color for age 20 and above
                    Color cell_color = cell_colors[age];
                    cell* curr = grid[i][j];
                    curr->color = cell_color;

                    Rectangle curr_rect = {curr->pos.x, curr->pos.y, atomic_cell_side, atomic_cell_side};
                    if (!curr->alive) {
                        DrawRectangleLinesEx(curr_rect, cell_margin, WHITE);
                    }
                    else {
                        DrawRectangle(curr->pos.x, curr->pos.y, atomic_cell_side, atomic_cell_side, curr->color);
                    }
                }
            }

            Vector2 mouse_pos = GetMousePosition();
            fill_cells(cell_side_choice, mouse_pos);

            // Display information regarding generation, total live cells and total cells ever alive 
            char gen[20];
            sprintf(gen, "Generation: %d", generation);
            DrawText(gen, 1000, 60, 25, SKYBLUE);

            char liveCells[20];
            sprintf(liveCells, "Live cells: %d", totalLiveCells);
            DrawText(liveCells, 1000, 100, 25, SKYBLUE);

            // create Gosper Glider Gun
            DrawText("To set Gosper Glider Gun, press G", 1000, 240, 20, GRAY);

            // clear grid
            DrawText("To CLEAR GRID, press C", 1000, 400, 20, WHITE);

            // randomly populate grid
            DrawText("To RANDOMLY POPULATE GRID,", 1000, 440, 20, WHITE);
            DrawText("press R", 1000, 470, 20, WHITE);

            // play/pause simulation
            DrawText("To PLAY / PAUSE Simulation,", 1000, 510, 20, WHITE);
            DrawText("press P", 1000, 540, 20, WHITE);

            // manually toggle cell state
            DrawText("To manually toggle cell state,", 1000, 570, 20, WHITE);
            DrawText("hover a cell", 1000, 600, 20, WHITE);
            DrawText("and press the LEFT MOUSE KEY", 1000, 630, 20, WHITE);
        }

        // predefined sets of patterns

        // Gosper Glider Gun
        void setGosperGliderGun(int startX, int startY) {
            // Clear the grid first if needed
            clear_grid();

            std::vector<std::pair<int, int>> gosperGliderGunPattern = {
                {0, 24}, {1, 22}, {1, 24}, {2, 12}, {2, 13}, {2, 20}, {2, 21}, {2, 34}, {2, 35},
                {3, 11}, {3, 15}, {3, 20}, {3, 21}, {3, 34}, {3, 35}, {4, 0}, {4, 1}, {4, 10},
                {4, 16}, {4, 20}, {4, 21}, {5, 0}, {5, 1}, {5, 10}, {5, 14}, {5, 16}, {5, 17},
                {5, 22}, {5, 24}, {6, 10}, {6, 16}, {6, 24}, {7, 11}, {7, 15}, {8, 12}, {8, 13}
            };

            for (const auto& cell : gosperGliderGunPattern) {
                int x = startX + cell.first;
                int y = startY + cell.second;
                if (x >= 0 && x < grid.size() && y >= 0 && y < grid[0].size()) {
                    grid[x][y]->alive = true;
                }
            }
        }
};


class Screen {
    private:
        Grid* grid = new Grid();
        Color available_colors = {};
        Color default_color = WHITE;
        std::string hex_val;
        int max_input_chars = 6;
        int curr_char_count = 0;
        int font_size_hex = 15;

        // choice for cell dimensions
        int cell_side_choice = 1;

    public:
        bool mouse_over_box(Vector2 mouse_pos, Rectangle box) {
            return ((mouse_pos.x > box.x && mouse_pos.y < (box.y + box.height) && mouse_pos.y > box.y && mouse_pos.x < (box.x + box.width)));
        }

        void screen_manager(int frame_counter) {
            grid->run(cell_side_choice);
        }

};


int main()
{
    InitWindow(screen_width, screen_height, "Pixel Art");
    SetTargetFPS(60);
    int frame_counter = 0;

    Screen* screen = new Screen();

    while (!WindowShouldClose()) {
        // Draw
        BeginDrawing();
            ClearBackground(BLACK);
            screen->screen_manager(frame_counter);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
#include "cellular_automaton.h"

CellularAutomaton::CellularAutomaton(int w, int h)
    : width(w), height(h) {

    grid = new Matrix<uint8_t>(width, height);
    nextGrid = new Matrix<uint8_t>(width, height);

    // Random initialization with ~30% density
    randomSeed(analogRead(0));  // Seed with noise from analog pin
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            (*grid)(y, x) = (random(100) < 30) ? 1 : 0;
        }
    }
}

CellularAutomaton::~CellularAutomaton() {
    delete grid;
    delete nextGrid;
}

uint8_t* CellularAutomaton::state() {
    return grid->raw();
}

void CellularAutomaton::computeNext() {
    uint8_t* current = grid->raw();
    uint8_t* next = nextGrid->raw();
    
    // Optimized: direct pointer access, unrolled neighbor checks, minimal branching
    for (int y = 0; y < height; y++) {
        int yPrev = (y == 0) ? height - 1 : y - 1;
        int yNext = (y == height - 1) ? 0 : y + 1;
        
        for (int x = 0; x < width; x++) {
            int xPrev = (x == 0) ? width - 1 : x - 1;
            int xNext = (x == width - 1) ? 0 : x + 1;
            
            // Direct array access - unrolled neighbor sum
            int count = 
                current[yPrev * width + xPrev] +
                current[yPrev * width + x]     +
                current[yPrev * width + xNext] +
                current[y * width + xPrev]     +
                current[y * width + xNext]     +
                current[yNext * width + xPrev] +
                current[yNext * width + x]     +
                current[yNext * width + xNext];
            
            int idx = y * width + x;
            uint8_t cell = current[idx];
            
            // Conway's Game of Life rules
            next[idx] = (count == 3 || (cell && count == 2)) ? 1 : 0;
        }
    }
}

uint8_t* CellularAutomaton::step() {
    computeNext();

    Matrix<uint8_t>* tmp = grid;
    grid = nextGrid;
    nextGrid = tmp;

    return grid->raw();
}

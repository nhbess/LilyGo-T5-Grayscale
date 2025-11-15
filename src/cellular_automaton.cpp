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
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int count = 0;

            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {

                    if (dx == 0 && dy == 0) continue;

                    int nx = (x + dx + width) % width;
                    int ny = (y + dy + height) % height;

                    count += (*grid)(ny, nx);
                }
            }

            uint8_t cell = (*grid)(y, x);

            uint8_t newCell =
                (cell == 1 && (count == 2 || count == 3)) ||
                (cell == 0 && count == 3) ? 1 : 0;

            (*nextGrid)(y, x) = newCell;
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

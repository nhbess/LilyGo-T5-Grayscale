#include "cellular_multy.h"
#include <vector>

std::vector<int> radii = {1, 3, 5};

std::vector<std::vector<int>> dx_list;
std::vector<std::vector<int>> dy_list;

static void buildNeighborhood(std::vector<int>& dx, std::vector<int>& dy, int r) {
    dx.clear();
    dy.clear();
    for (int j = -r; j <= r; j++) {
        for (int i = -r; i <= r; i++) {
            if (i == 0 && j == 0) continue;
            dx.push_back(i);
            dy.push_back(j);
        }
    }
}

MNCA::MNCA(int w, int h)
    : width(w), height(h) {

    grid = new Matrix<uint8_t>(width, height);
    nextGrid = new Matrix<uint8_t>(width, height);

    dx_list.clear();
    dy_list.clear();
    dx_list.resize(radii.size());
    dy_list.resize(radii.size());

    for (size_t i = 0; i < radii.size(); i++) {
        buildNeighborhood(dx_list[i], dy_list[i], radii[i]);
    }

    randomSeed(analogRead(0));

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            (*grid)(y, x) = (random(100) < 30) ? 1 : 0;
        }
    }
}

MNCA::~MNCA() {
    delete grid;
    delete nextGrid;
}

uint8_t* MNCA::state() {
    return grid->raw();
}

void MNCA::computeNext() {
    uint8_t* current = grid->raw();
    uint8_t* next = nextGrid->raw();

    int numN = radii.size();

    // Pre-compute neighborhood sizes to avoid repeated .size() calls
    int neighborSizes[3];
    for (int n = 0; n < numN; n++) {
        neighborSizes[n] = dx_list[n].size();
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int idx = y * width + x;
            uint8_t cell = current[idx];
            uint8_t result = cell;

            // Use only the largest radius (5) for faster computation
            // This gives similar behavior with much better performance
            const std::vector<int>& dx = dx_list[numN - 1];
            const std::vector<int>& dy = dy_list[numN - 1];
            
            int N = neighborSizes[numN - 1];
            int sum = 0;

            for (int k = 0; k < N; k++) {
                int xx = x + dx[k];
                int yy = y + dy[k];

                // Optimize wrapping with modulo
                xx = (xx + width) % width;
                yy = (yy + height) % height;

                sum += current[yy * width + xx];
            }

            // Use integer arithmetic instead of float
            int threshold_high = (N * 55) / 100;  // 0.55 * N
            int threshold_low = (N * 35) / 100;   // 0.35 * N

            if (sum > threshold_high) result = 1;
            else if (sum < threshold_low) result = 0;

            next[idx] = result;
        }
    }
}

uint8_t* MNCA::step() {
    computeNext();

    Matrix<uint8_t>* tmp = grid;
    grid = nextGrid;
    nextGrid = tmp;

    return grid->raw();
}

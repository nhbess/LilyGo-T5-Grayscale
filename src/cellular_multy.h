#ifndef MNCA_H
#define MNCA_H

#include <Arduino.h>
#include <stdint.h>
#include <vector>
#include "cellular_automaton.h"

class MNCA {
public:
    MNCA(int w, int h);
    ~MNCA();

    uint8_t* step();
    uint8_t* state();

private:
    int width, height;
    Matrix<uint8_t>* grid;
    Matrix<uint8_t>* nextGrid;

    void computeNext();
};

#endif

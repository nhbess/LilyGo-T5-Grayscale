#ifndef CELLULAR_AUTOMATON_H
#define CELLULAR_AUTOMATON_H

#include <Arduino.h>
#include <stdint.h>

template<typename T>
class Matrix {
public:
    Matrix(int w, int h) : w(w), h(h) {
        data = new T[w * h];
    }

    ~Matrix() {
        delete[] data;
    }

    inline T& operator()(int y, int x) {
        return data[y * w + x];
    }

    inline const T& operator()(int y, int x) const {
        return data[y * w + x];
    }

    T* raw() { return data; }
    int width() const { return w; }
    int height() const { return h; }

private:
    int w, h;
    T* data;
};

class CellularAutomaton {
public:
    CellularAutomaton(int w, int h);
    ~CellularAutomaton();

    uint8_t* step();
    uint8_t* state();

private:
    int width, height;
    Matrix<uint8_t>* grid;
    Matrix<uint8_t>* nextGrid;

    void computeNext();
};

#endif

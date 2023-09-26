//
// Created by Aashiq Shaikh on 8/26/23.
//

#ifndef INT3_H
#define INT3_H

#include <cmath>
#include <iostream>

class int3 {
public:
    int3() : i{0, 0, 0} {}
    int3(int i0, int i1, int i2) : i{i0, i1, i2} {}

    int x() const { return i[0]; }
    int y() const { return i[1]; }
    int z() const { return i[2]; }
    int operator[](int j) const { return i[j]; }
    int& operator[](int j) { return i[j]; }

public:
    int i[3];
};

#endif

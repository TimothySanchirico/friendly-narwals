#pragma once

#include <array>


struct Gravity {
    static Acceleration getAcceleration() { return {0.0f, 0.098f, 0.0f}; }
};


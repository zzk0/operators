// reference https://stackoverflow.com/questions/4915462/how-should-i-do-floating-point-comparison

#ifndef CSRC_TOOLS_FLOAT_COMPARISON
#define CSRC_TOOLS_FLOAT_COMPARISON

#include <cmath>

inline bool FloatEqual(float a, float b, float epsilon) {
    float abs_a = std::abs(a);
    float abs_b = std::abs(b);
    float diff = std::abs(a - b);

    if (a == b) {
        return true;
    }
    else if (a == 0 || b == 0 || diff < epsilon) {
        return true;
    }
    else {
        return diff / (abs_a + abs_b) < epsilon;
    }
}

#endif /* CSRC_TOOLS_FLOAT_COMPARISON */

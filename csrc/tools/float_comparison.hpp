// reference https://stackoverflow.com/questions/4915462/how-should-i-do-floating-point-comparison
// https://stackoverflow.com/questions/17333/what-is-the-most-effective-way-for-float-and-double-comparison

#ifndef CSRC_TOOLS_FLOAT_COMPARISON
#define CSRC_TOOLS_FLOAT_COMPARISON

#include <cmath>
#include <limits>

inline bool FloatEqual(float a, float b, float epsilon = std::numeric_limits<float>::epsilon()) {
    float diff = std::fabs(a - b);
    if (diff <= epsilon) {
        return true;
    }
    if (diff < std::fmax(std::abs(a), std::abs(b)) * epsilon) {
        return true;
    }
    return false;
}

#endif /* CSRC_TOOLS_FLOAT_COMPARISON */

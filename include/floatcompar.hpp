#pragma once

namespace yLab {
    
namespace cmp {

static float flt_tolerance = 0.00001;

//(a < b) return 1, (a == b) return 0, (a > b) return -1; using relative epsilon comparison
inline int float_comparator(float a, float b) {
    auto diff = std::abs(a-b);

    if (diff < flt_tolerance)
        return 0;
    else if (diff < std::max(std::abs(a), std::abs(b)) * flt_tolerance)
        return 0;
    else if (a < b)
        return 1;
    else
        return -1;
}

inline bool is_zero(float a) {
    return (float_comparator(a, 0) == 0);    
}

inline bool equal(float a, float b) {
    return (float_comparator(a,b) == 0);
}

inline bool greater(float a, float b) {
    return (float_comparator(a, b) == -1);
}

inline bool greater_equal(float a, float b) {
    return ((float_comparator(a,b) == -1) || (float_comparator(a,b) == 0));
}

inline bool less(float a, float b) {
    return (float_comparator(a,b) == 1);
}

inline bool less_equal(float a, float b) {
    return ((float_comparator(a,b) == 1) || (float_comparator(a,b) == 0));
}

}
}
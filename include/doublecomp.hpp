#pragma once

namespace yLab {
namespace cmp {

static double flt_tolerance = 0.00001;

//(a < b) return 1, (a == b) return 0, (a > b) return -1; using relative epsilon comparison
inline int double_comparator(double a, double b) {
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

inline bool is_zero(double a) {
    return (double_comparator(a, 0) == 0);    
}

inline bool equal(double a, double b) {
    return (double_comparator(a,b) == 0);
}

inline bool greater(double a, double b) {
    return (double_comparator(a, b) == -1);
}

inline bool greater_equal(double a, double b) {
    return ((double_comparator(a,b) == -1) || (double_comparator(a,b) == 0));
}

inline bool less(double a, double b) {
    return (double_comparator(a,b) == 1);
}

inline bool less_equal(double a, double b) {
    return ((double_comparator(a,b) == 1) || (double_comparator(a,b) == 0));
}

}
}
#include "common.h"

std::string str_concat(const std::string first, const std::string second)
{
    return first + second;
}

double clamp(double val, double min_val, double max_val)
{
    if (val < min_val)
        return min_val;
    if (val > max_val)
        return max_val;
    return val;
}

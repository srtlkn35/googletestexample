#pragma once

#include <string>
#include <stdexcept>
#include <vector>
#include <math.h>
#include <algorithm>

namespace mymathlibnamespace 
{
    double mySqrt(double x);
    bool isPositive(int x);
    int countPositives(std::vector<int> const& inputVector);
    void toUpper(char *inputString);
    std::vector<int> generateNumbers(int n, int limit);
}
